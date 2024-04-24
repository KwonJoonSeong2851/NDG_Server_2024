#pragma once
#include "../stdafx.h"

class Session;
class SessionManager;
class Package;
class Packet;


typedef enum
{
	IO_READ,
	IO_WRITE,
	IO_ERROR,
} IO_OPERATION;



static const byte tcpPacketHead[9]
{
	(byte)251,
	(byte)0, //s
	(byte)0, //i
	(byte)0, //z
	(byte)0, //e 4byte
	(byte)0, //channel ID
	(byte)0, // deliver Mode 
	(byte)243,
	(byte)2, // EgMessageType
};

//static const byte* pingResponse = new byte[5]
//{
//	240,
//	0,
//	0,
//	0,
//	0,
//};

#define IO_DATA_MAX (2)

/// <summary>
/// Session���� ó���Ǵ� Data���� ���� ó���ϴ� Ŭ�����Դϴ�.
/// </summary>
class IoData
{
private:
	OVERLAPPED m_overlapped;
	IO_OPERATION m_ioType;
	size_t m_totalBytes;
	size_t m_currentBytes;

	std::array<char, SOCKET_BUF_SIZE> m_buffer;


public:
	IoData();
	~IoData() {};

	/// <summary>buffer �� �������� �ʱ�ȭ�մϴ�. </summary>
	void Clear();

	/// <summary>���� ������ �����Ͱ� ��ü ��Ŷũ��� �������� üũ�մϴ�.</summary>
	bool NeedMoreIo(size_t transferSize);

	/// <summary> m_buffer�� �� byte ũ�⸦ �����մϴ�.</summary>
	int32_t SetupTotalBytes();

	size_t GetTotalBytes();

	/// <summary> m_ioType�� ���� ��ȯ�մϴ�.. </summary>
	IO_OPERATION& GetType();

	/// <summary> m_ioType�� ���� �����մϴ�.</summary>
	void SetType(IO_OPERATION type);

	WSABUF GetWSABuf();

	char* GetData();

	/// <summary>�ش� stream�� m_buffer�� �����͸� �����մϴ�. /summary>
	bool SetData(StreamBuffer& stream);

	LPWSAOVERLAPPED GetOverlapped();

};


class Session;
/// <summary>
///  Session Ŭ������ ��ӹ޾� Client�� �ۼ����ϴ� ����� ���� ������ Ŭ�����Դϴ�.
/// </summary>
class IOCPSession : public Session
{

private:
	unsigned char* m_pingResponsePacket = new byte[9]{ 240,0,0,0,0,0,0,0 };


public:
	std::array<IoData, IO_DATA_MAX> m_ioData;

private:
	void Initialize();

	/// <summary>�ۼ��Ž� ������ �����մϴ�.</summary>
	void CheckErrorIO(DWORD ret);


	void Recv(WSABUF wsaBuf);
	bool IsRecving(size_t transferSize);
	void Send(WSABUF wsaBuf);

	void ReadPing(char* buf);
	void SendPing();

public:
	IOCPSession();

	void OnSend(size_t transferSize);
	void SendPacket(Packet* packet);

	Package* OnRecv(size_t transferSize);
	void RecvStandBy();
};