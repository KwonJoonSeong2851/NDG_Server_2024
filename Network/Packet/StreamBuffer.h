#pragma once
#include "../../stdafx.h"
using namespace std;
 
class StreamBuffer
{
private:
	int pos;
	int len;
	byte* buf;

	bool CheckSize(int size);

public:
	StreamBuffer(int size = 0);
	StreamBuffer(byte* buf,int size);
	~StreamBuffer();
	int Length();
	int Position();
	void SetLength(long value);
	void SetPosition(int value);

	byte* ToArray();
	byte* ToArrayFromPos();

	void Compact();
	byte* GetBuffer();
	byte* GetBufferAndAdvance(int length, int& offset);



	
	void SetCapacityMinimum(int neededSize);

	int Read(byte* buffer, int offset, int count);
	void Write(const byte* buffer, int srcOffset, int count);

	byte ReadByte();
	void WriteByte(byte value);
	void WriteBytes(byte v0, byte v1);



};