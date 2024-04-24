
#include "IOCPSession.h"

IoData::IoData()
{
	ZeroMemory(&m_overlapped, sizeof(m_overlapped));
	m_ioType = IO_ERROR;

	this->Clear();
}


void IoData::Clear()
{
	m_buffer.fill(0);
	m_totalBytes = 0;
	m_currentBytes = 0;
}

bool IoData::NeedMoreIo(size_t transferSize)
{
	m_currentBytes += transferSize;
	if (m_currentBytes < m_totalBytes)
	{
		return true;
	}
	return false;
}

int32_t IoData::SetupTotalBytes()
{
	packet_size_t offset = 1;
	packet_size_t packetLen = 0;

	if (m_totalBytes == 0)
	{
		char* buffer = m_buffer.data();
		int num1 = (int)buffer[offset++] << 24;
		int num2 = (int)buffer[offset++] << 16;
		int num3 = (int)buffer[offset++] << 8;
		packetLen = num1 | num2 | num3 | (int)buffer[offset++];
		//return  num1 | num2 | num3 | (int)bufferAndAdvance[offset + 3];
		//memcpy_s((void*)packetLen, sizeof(packetLen), (void*)(m_buffer.data()), sizeof(packetLen));
		//m_totalBytes = ntohl((size_t)packetLen[0]);
		m_totalBytes = packetLen;
	}
	
	return offset;
}

size_t IoData::GetTotalBytes()
{
	return m_totalBytes;
}

IO_OPERATION& IoData::GetType()
{
	return m_ioType;
}

void IoData::SetType(IO_OPERATION type)
{
	m_ioType = type;
}

char* IoData::GetData()
{
	return m_buffer.data();
}

bool IoData::SetData(StreamBuffer& stream)
{
	this->Clear();

	if (m_buffer.max_size() <= stream.Length())
	{
		SLOG(L"IoData : packet size too big [%d]byte", stream.Length());
		return false;
	}

	char* buf = m_buffer.data();
	memcpy_s(buf, m_buffer.max_size(), (void*)stream.GetBuffer(), stream.Length());

	packet_size_t size = stream.Length();
	packet_size_t offset = 1;
	
	buf[offset++] = (unsigned char)(size >> 24);
	buf[offset++] = (unsigned char)(size >> 16);
	buf[offset++] = (unsigned char)(size >> 8);
	buf[offset++] = (unsigned char)(size);

	

	m_totalBytes = stream.Length();
	return true;
}

LPWSAOVERLAPPED IoData::GetOverlapped()
{
	return &m_overlapped;
}

WSABUF IoData::GetWSABuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = m_buffer.data() + m_currentBytes;
	wsaBuf.len = (ULONG)(m_totalBytes - m_currentBytes);
	return wsaBuf;
}






//-------------------------------------------------//



 


IOCPSession::IOCPSession() : Session()
{
	this->Initialize();
}

void IOCPSession::Initialize()
{
	ZeroMemory(&m_socketData, sizeof(SOCKET_DATA));
	m_ioData[IO_READ].SetType(IO_READ);
	m_ioData[IO_WRITE].SetType(IO_WRITE);
	
	m_protocol = new Protocol18();
}

void IOCPSession::CheckErrorIO(DWORD ret)
{
	if (ret == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		SLOG(L"IOCPSession : socket error1: %d", WSAGetLastError());
	}
}

void IOCPSession::Recv(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes;
	DWORD errorCode = WSARecv(m_socketData.socket_, &wsaBuf, 1, &recvBytes, &flags, m_ioData[IO_READ].GetOverlapped(),NULL);

	this->CheckErrorIO(errorCode);
}

bool IOCPSession::IsRecving(size_t transferSize)
{
	if (m_ioData[IO_READ].NeedMoreIo(transferSize))
	{
		this->Recv(m_ioData[IO_READ].GetWSABuf());
		return true;
	}
	return false;
}

void IOCPSession::RecvStandBy()
{
	m_ioData[IO_READ].Clear();

	WSABUF wsaBuf;
	wsaBuf.buf = m_ioData[IO_READ].GetData();
	wsaBuf.len = SOCKET_BUF_SIZE;

	this->Recv(wsaBuf);
}

void IOCPSession::Send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(m_socketData.socket_, &wsaBuf, 1, &sendBytes, flags, m_ioData[IO_WRITE].GetOverlapped(), NULL);
	this->CheckErrorIO(errorCode);

}

void IOCPSession::ReadPing(char* buf)
{
	int offset = 1;
	m_sessionLocalTime = buf[offset++] >> 24 | buf[offset++] >> 16 | buf[offset++] >> 8 | buf[offset++];
	SLOG(L"Recv Ping : session time = %d", m_sessionLocalTime);
	UpdateHeartBeat();
	SendPing();
}

void IOCPSession::SendPing()
{
	int offset = 1;
	m_protocol->Serialize(NOW_MILLISEC_INT32, m_pingResponsePacket, offset);
	m_protocol->Serialize(m_sessionLocalTime, m_pingResponsePacket, offset);

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)m_pingResponsePacket;
	wsaBuf.len = 9;
	this->Send(wsaBuf);
	this->RecvStandBy();

}

void IOCPSession::OnSend(size_t transferSize)
{
	if (m_ioData[IO_WRITE].NeedMoreIo(transferSize))
	{
		this->Send(m_ioData[IO_WRITE].GetWSABuf());
	}
}

void IOCPSession::SendPacket(Packet* packet) // DeliverMode, channelID, opMessage 추가 필요
{
	StreamBuffer stream;
	stream.Write(tcpPacketHead, 0, sizeof(tcpPacketHead));
	GetProtocol()->SerializePacket(stream, packet);


	if (!m_ioData[IO_WRITE].SetData(stream))
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = m_ioData[IO_WRITE].GetData();
	wsaBuf.len = (ULONG)stream.Length();

	this->Send(wsaBuf);
	this->RecvStandBy();
}

Package* IOCPSession::OnRecv(size_t transferSize)
{
	m_ioData[IO_READ].SetupTotalBytes();

	if (this->IsRecving(transferSize))
	{
		return nullptr;
	}

	//handling ping
	if (m_ioData[IO_READ].GetData()[0] == 240)
	{
		ReadPing(m_ioData[IO_READ].GetData());
		this->RecvStandBy();
		return nullptr;
	}


	//handling packet head 

	Packet* packet = PacketAnalyzer::GetInstance().Analyzer(m_ioData[IO_READ].GetData(), m_ioData[IO_READ].GetTotalBytes());

	if (packet == nullptr)
	{
		SLOG(L"IOCPSession : Invalid Packet !!!");
		this->OnClose();
		return nullptr;
	}

	this->RecvStandBy();
	Package* package = new Package(this,packet);
	return package;
}