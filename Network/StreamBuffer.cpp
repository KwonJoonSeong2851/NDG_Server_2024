#include "StreamBuffer.h"

bool StreamBuffer::CheckSize(int size)
{
	if (size <= _msize(this->buf))
		return false;
	int length = _msize(this->buf);
	if (length == 0)
		length = 1;
	while (size > length)
		length *= 2;
	byte* numArray = new byte[length];
	memcpy_s(numArray, length, this->buf, _msize(this->buf));
	
	delete[] this->buf;
	this->buf = numArray;
	this->SetLength(length);
	return true;
}

StreamBuffer::StreamBuffer(int size)
{
	this->pos = 0;
	this->len = size;
	this->buf = new byte[size];
}

StreamBuffer::StreamBuffer(byte* buf,int size)
{
	this->pos = 0;
	this->len = size;
	byte* tempBuf = new byte[size];
	memcpy_s(tempBuf, size, buf, size);
	this->buf = tempBuf;
}

StreamBuffer::~StreamBuffer()
{
	delete[] this->buf;
}

int StreamBuffer::Length()
{
	return this->len;
}

int StreamBuffer::Position()
{
	return this->pos;
}

void StreamBuffer::SetLength(long value)
{
	this->len = (int)value;
	this->CheckSize(this->len);
	if (this->pos <= this->len)
		return;
	this->pos = this->len;
}

void StreamBuffer::SetPosition(int value)
{
	this->pos = value;
	if (this->len >= this->pos)
		return;
	this->len = this->pos;
	this->CheckSize(this->len);
}

byte* StreamBuffer::ToArray()
{
	byte* numArray = new byte[this->len];
	memcpy_s(numArray, this->len, this->buf, this->len);
	return numArray;
}

byte* StreamBuffer::ToArrayFromPos()
{
	int count = this->len - this->pos;
	if (count <= 0)
		return new byte[0];
	byte* numArray = new byte[count];
	memcpy_s(numArray, count, this->buf + pos, count);
	return numArray;
}

void StreamBuffer::Compact()
{
	long num = (long)(this->Length() - this->Position());
	if (num > 0L)
		memcpy_s(this->buf, num, this->buf + pos, num);
	this->SetPosition(0);
	this->SetLength(0);
}

byte* StreamBuffer::GetBuffer()
{
	return this->buf;
}

byte* StreamBuffer::GetBufferAndAdvance(int length, int& offset)
{
	offset = this->pos;
	this->SetPosition(this->Position() + length);
	return this->buf;
}

void StreamBuffer::SetCapacityMinimum(int neededSize)
{
	this->CheckSize(neededSize);
}

int StreamBuffer::Read(byte* buffer, int offset, int count)
{
	int num = this->len - this->pos;
	if (num <= 0)
		return 0;
	if (count > num)
		count = num;
	memcpy_s(buffer + offset, count, this->buf + this->pos, count);
	this->pos += count;
	return count;
}

void StreamBuffer::Write(const byte* buffer, int srcOffset, int count)
{
	int size = this->pos + count;
	this->CheckSize(size);
	if (size > this->len)
		this->len = size;
	memcpy_s(this->buf + this->pos, this->len, buffer + srcOffset, count);
	this->pos = size;
}

byte StreamBuffer::ReadByte()
{
	//std::cout << "Pos " << this->pos << " len " << len << endl;
	return this->pos < this->len ? this->buf[this->pos++] : throw std::runtime_error("StreamBuffer.ReadByte() failed.");
}

void StreamBuffer::WriteByte(byte value)
{
	if (this->pos >= this->len)
	{
		this->len = this->pos + 1;
		this->CheckSize(this->len);
	}
	this->buf[this->pos++] = value;
}

void StreamBuffer::WriteBytes(byte v0, byte v1)
{
	int num = this->pos + 2;;
	if (this->len < num)
	{
		this->len = num;
		this->CheckSize(this->len);
	}
	this->buf[this->pos++] = v0;
	this->buf[this->pos++] = v1;

}
