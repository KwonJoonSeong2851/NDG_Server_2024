#include "../../stdafx.h"
#include "Protocol18.h"


const type_info* Protocol18::GetTypeOfCode(GpType gpType)
{
	switch (gpType)
	{
	case GpType::Boolean:
		return &typeid(bool);
	case GpType::Byte:
		return &typeid(byte);
	case GpType::Short:
		return &typeid(short);
	case GpType::CompressedInt:
		return &typeid(int);
	case GpType::CompressedLong:
		return &typeid(long long);
	case GpType::Float:
		return &typeid(float);
	case GpType::Double:
		return &typeid(double);
	case GpType::String:
		return &typeid(wstring);
	case GpType::Hashtable:
		return &typeid(Hashtable);
	//case GpType::Dictionary:
	//	return &typeid(Dictionary);

	//case GpType::OperationRequest:
	//	return &typeid(OperationRequest);
	//case GpType::OperationResponse:
	//	return &typeid(OperationResponse);

	default:
		//오류 처리
		break;
	}
	return nullptr;
}

GpType Protocol18::GetCodeOfType(const Object* object)
{
	cout << "GetCodeOfType : " << object->GetType()->name() << endl;
	if (object->GetType() == nullptr)
	{
		return GpType::Null;
	}

	const type_info& typeCode = *object->GetType();
	if (object->IsPrimitive() && object->IsArray() == false)
	{
		if (typeCode == typeid(bool))
			return GpType::Boolean;
		if (typeCode == typeid(byte))
			return GpType::Byte;
		if (typeCode == typeid(short))
			return GpType::Short;
		if (typeCode == typeid(int))
			return GpType::CompressedInt;
		if (typeCode == typeid(long long))
			return GpType::CompressedLong;
		if (typeCode == typeid(float))
			return GpType::Float;
		if (typeCode == typeid(double))
			return GpType::Double;

		return GpType::Unknown;
	}

	if (typeCode == typeid(wstring_) && object->IsArray() == false)
		return GpType::String;

	if (object->IsArray())
	{
		if (object->IsPrimitive())
		{
			const type_info& elementType = *object->GetType();
			if (elementType == typeid(bool))
				return GpType::BooleanArray;
			if (elementType == typeid(byte))
				return GpType::ByteArray;
			if (elementType == typeid(short))
				return GpType::ShortArray;
			if (elementType == typeid(int))
				return GpType::CompressedIntArray;
			if (elementType == typeid(long long))
				return GpType::CompressedLongArray;
			if (elementType == typeid(float))
				return GpType::FloatArray;
			if (elementType == typeid(double))
				return GpType::DoubleArray;
		}

		const Object* element = object->GetElement();
		if (element == nullptr)
		{
			runtime_error("The array type is unknown.");
			return GpType::Unknown;
		}
		if (element->IsArray())
			return GpType::Array;
		if (*element->GetType() == typeid(wstring_))
			return GpType::StringArray;
		if (*element->GetType() == typeid(Object))
			return GpType::ObjectArray;
		if (*element->GetType() == typeid(Hashtable))
			return GpType::HashtableArray;
		if (*element->GetType() == typeid(Dictionary))
			return GpType::DictionaryArray;
		return GpType::CustomTypeArray;
	}

	if (typeCode == typeid(Hashtable))
		return GpType::Hashtable;
	if (typeCode == typeid(Dictionary))
		return GpType::Dictionary;
	//if (typeCode == typeid(EventData))
	//	return GpType::EventData;
	//if (typeCode == typeid(OperationRequest))
	//	return GpType::OperationRequest;
	//if (typeCode == typeid(OperationResponse))
	//	return GpType::OperationResponse;
	
	return GpType::Unknown;
}



unsigned int Protocol18::EncodeZigZag32(int value)
{
	return (unsigned int)(value << 1 ^ value >> 31);
}

unsigned long long Protocol18::EncodeZigZag64(long long value)
{
	return (unsigned long long)(value << 1 ^ value >> 63);
}

int Protocol18::WriteCompressedUInt32(byte* buffer, unsigned int value)
{
	int index = 0;
	buffer[index] = (byte)(value & (unsigned int)127);
	for (value >>= 7; value > 0U; value >>= 7)
	{
		buffer[index] |= (byte)128;
		buffer[++index] = (byte)(value & (unsigned int)127);
	}
	return index + 1;
}

void Protocol18::WriteCompressedUInt32(StreamBuffer& stream,const unsigned int& value)
{
	//lock(this.memCompressedUint32)
	byte* memCompressedUInt32 = new byte[5];
	stream.Write(memCompressedUInt32, 0, this->WriteCompressedUInt32(memCompressedUInt32, value));
	delete[] memCompressedUInt32;
}

void Protocol18::WriteCompressedUInt64(StreamBuffer& stream,const unsigned long long& value)
{
	int index = 0;
	unsigned long long temp = value;
	//lock
	this->memCompressedUInt64[index] = (byte)(temp & (unsigned long long)127); // sbyte max
	for (temp >>= 7; temp > 0UL; temp >>= 7)
	{
		this->memCompressedUInt64[index] |= (byte)128;
		this->memCompressedUInt64[++index] = (byte)(temp & (unsigned long long)127);
	}
	int count = index + 1;
	stream.Write(this->memCompressedUInt64, 0, count);
}

void Protocol18::WriteCompressedInt32(StreamBuffer& stream,const int& value, bool writeType)
{
	if (writeType)
	{
		if (value == 0)
		{
			stream.WriteByte((byte)30);
			return;
		}
		if (value > 0)
		{
			if (value <= (int)255)
			{
				stream.WriteByte((byte)11);
				stream.WriteByte((byte)value);
				return;
			}
			if (value <= (int)65535) // ushort max
			{
				stream.WriteByte((byte)13);
				this->WriteUShort(stream, (unsigned short)value);
				return;
			}
		}
		else if (value >= -65535)
		{
			if (value >= -255)
			{
				stream.WriteByte((byte)12);
				stream.WriteByte((byte)-value);
				return;
			}
			if (value >= -65535)
			{
				stream.WriteByte((byte)14);
				this->WriteUShort(stream, (unsigned short)-value);
				return;
			}
		}
	}
	if (writeType)
		stream.WriteByte((byte)9);
	unsigned int num = this->EncodeZigZag32(value);
	this->WriteCompressedUInt32(stream, num);
}

void Protocol18::WriteCompressedInt64(StreamBuffer& stream,const long long& value, bool writeType)
{
	if (writeType)
	{
		if (value == 0L)
		{
			stream.WriteByte((byte)31);
			return;
		}
		if (value > 0L)
		{
			if (value <= (long long)255)
			{
				stream.WriteByte((byte)15);
				stream.WriteByte((byte)value);
				return;
			}
			if (value <= (long long)65535) // ushort max
			{
				stream.WriteByte((byte)17);
				this->WriteUShort(stream, (unsigned short)value);
				return;
			}
		}
		else if (value >= -65535L)
		{
			if (value >= -255L)
			{
				stream.WriteByte((byte)16);
				stream.WriteByte((byte)-value);
				return;
			}
			if (value >= -65535L)
			{
				stream.WriteByte((byte)18);
				this->WriteUShort(stream, (unsigned short)-value);
				return;
			}
		}
	}
	if (writeType)
		stream.WriteByte((byte)10);
	unsigned long long num = this->EncodeZigZag64(value);
	this->WriteCompressedUInt64(stream, num);

}

void Protocol18::WriteBoolean(StreamBuffer& stream,const bool& value, bool writeType)
{
	if (writeType)
	{
		if (value)
			stream.WriteByte((byte)28);
		else
			stream.WriteByte((byte)27);
	}
	else
		stream.WriteByte(value ? (byte)1 : (byte)0);
}

void Protocol18::WriteByte(StreamBuffer& stream,const byte& value, bool writeType)
{
	if (writeType)
	{
		if (value == (byte)0)
		{
			stream.WriteByte((byte)34);
			return;
		}
		stream.WriteByte((byte)3);
	}
	stream.WriteByte(value);
}

void Protocol18::WriteUShort(StreamBuffer& stream,const unsigned short& value)
{
	stream.WriteBytes((byte)value, (byte)((unsigned int)value >> 8));
}

void Protocol18::WriteInt16(StreamBuffer& stream,const short& value, bool writeType)
{
	if (writeType)
	{
		if (value == (short)0)
		{
			stream.WriteByte((byte)29);
			return;
		}
		stream.WriteByte((byte)4);
	}
	stream.WriteByte((byte)value);
	stream.WriteByte((byte)((unsigned int)value >> 8));
}

void Protocol18::WriteSingle(StreamBuffer& stream,const float& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)5);
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(4, offset);
	//lock 필요
	this->memFloatBlock[0] = value;
	memcpy_s(bufferAndAdvance + offset, stream.Length(), this->memFloatBlock, sizeof(float));
}

void Protocol18::WriteDouble(StreamBuffer& stream,const double& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)6);
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(8, offset);
	//lock필요
	this->memDoubleBlock[0] = value;
	memcpy_s(bufferAndAdvance + offset, stream.Length() , this->memDoubleBlock, sizeof(double));
}

void Protocol18::WriteString(StreamBuffer& stream, const wstring& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)7);
	
	
	int byteCount = WideCharToMultiByte(CP_UTF8, 0, value.c_str(), lstrlenW(value.c_str()), NULL, 0, NULL, NULL);

	if (byteCount > 32767)
	{
		// 글자수 오류 처리
	}

	this->WriteIntLength(stream, byteCount);

	int offset = 0;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(byteCount, offset);
	char* charBuf = new char[byteCount];
	WideCharToMultiByte(CP_UTF8, 0, value.c_str(), lstrlenW(value.c_str()), charBuf, byteCount, NULL, NULL);
	memcpy_s(bufferAndAdvance + offset, byteCount, charBuf, byteCount);

	delete[] charBuf;
}


bool Protocol18::WriteArrayType(StreamBuffer& stream, const Object* object, GpType& writeType)
{
	if (object->GetType() == nullptr)
	{
		runtime_error("cannot serialize array type");
		return false;
	}

	const Object* elementObject = object->GetElement();
	if (elementObject != nullptr && elementObject->IsArray())
	{
		const Object* arrayInElement = elementObject;
		const Object* lastElement = elementObject;
		for (; arrayInElement != nullptr && arrayInElement->IsArray(); arrayInElement = arrayInElement->GetElement())
		{
			stream.WriteByte((byte)64); // 64 = GpType::Array
			lastElement = arrayInElement;
		}
		stream.WriteByte((byte)this->GetCodeOfType(lastElement));
		writeType = GpType::Array;
		return true;
	}

	if (object->IsPrimitive())
	{
		byte num = (byte)this->GetCodeOfType(object);
		stream.WriteByte(num);
		writeType = (GpType)num;
		if (writeType != GpType::Unknown)
			return true;
		else
		{
			runtime_error("the array type is Unkown");
			return false;
		}
	}

	if (*object->GetType() == typeid(wstring) || *object->GetType() == typeid(wstring_))
	{
		stream.WriteByte((byte)71); // GpType : StringArray
		writeType = GpType::StringArray;
		return true;
	}

	if (*object->GetType() == typeid(Object))
	{
		stream.WriteByte((byte)23); // GpType : ObjectArray
		writeType = GpType::ObjectArray;
		return true;
	}

	if (*object->GetType() == typeid(Hashtable))
	{
		stream.WriteByte((byte)85);
		writeType = GpType::HashtableArray; // GpType : HashtableArray;
		return true;
	}

	writeType = GpType::Unknown;
	return false;
}

void Protocol18::WriteHashTable(StreamBuffer& stream,const Hashtable& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)21);
	this->WriteIntLength(stream, value.Count());

	for (auto it = value.begin(); it != value.end(); ++it)
	{
		this->Write(stream, it->first, true);
		this->Write(stream, it->second, true);
	}
}

void Protocol18::WriteDictionaryElements(StreamBuffer& stream,const Dictionary& dic, GpType& keyType, GpType& valueType)
{
	this->WriteIntLength(stream, dic.Count());
	for (auto it = dic.begin(); it != dic.end(); ++it)
	{
		this->Write(stream, it->first, keyType == GpType::Unknown);
		this->Write(stream, it->second, valueType == GpType::Unknown);
	}
}

void Protocol18::WriteDictionaryHeader(StreamBuffer& stream,const Dictionary& dic, GpType& keyType, GpType& valueType)
{

	//Key head write
	if (dic.Count() == 0)
		return;

	if (dic.begin()->first->GetType()->hash_code() == typeid(Object).hash_code())
	{
		stream.WriteByte((byte)0);
		keyType = GpType::Unknown;
	}
	else
	{
		if (dic.begin()->first->IsArray() || (dic.begin()->first->IsPrimitive() == false && dic.begin()->first->GetType()->raw_name() != typeid(wstring_).raw_name()))
		{
			runtime_error("This is an unsupported key type.");
			return;
		}
		keyType = GetCodeOfType(dic.begin()->first);
		stream.WriteByte((byte)keyType);
	}
	////////////clear
	//element head write
	
	if (*dic.begin()->second->GetType() == typeid(Object))
	{
		stream.WriteByte((byte)0);
		valueType = GpType::Unknown;
	}
	else if (dic.begin()->second->IsArray())
	{
		if (!this->WriteArrayType(stream, dic.begin()->second, valueType))
			runtime_error("This dictionary value cannot be serialized.");
		//오류 처리
	}
	else
	{
		valueType = this->GetCodeOfType(dic.begin()->second);
		if (valueType == GpType::Unknown)
			runtime_error("This dictionary value cannot be serialized.");
		if (valueType == GpType::Dictionary)
		{
			stream.WriteByte((byte)valueType);
			GpType _;
			this->WriteDictionaryHeader(stream, *((Dictionary*)dic.begin()->second), _, _);
		}
		else
			stream.WriteByte((byte)valueType);
	}

	cout << "DIC KEY : " << (int)keyType << endl;
	cout << "DIC VALUE : " << (int)valueType << endl;
}


void Protocol18::WriteDictionary(StreamBuffer& stream,const Dictionary& dic, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)20);
	GpType keyType;
	GpType valueType;
	this->WriteDictionaryHeader(stream, dic,keyType, valueType);
	this->WriteDictionaryElements(stream, dic, keyType, valueType);
}

void Protocol18::WriteObjectArray(StreamBuffer& stream, const vector<Object>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)23);
	this->WriteIntLength(stream, value.size());
	for (int i = 0; i < value.size(); ++i)
	{
		const Object* obj = &value[i];
		this->Write(stream, obj, true);
	}
}

void Protocol18::WriteArrayInArray(StreamBuffer& stream, const Object* object, bool writeType)
{
	vector_<Object*>* obj = (vector_<Object*>*)object;
	stream.WriteByte((byte)64);
	this->WriteIntLength(stream, obj->GetSize());
	for (int i = 0; i < obj->GetSize(); ++i)
	{
		this->Write(stream, (*obj)[i], true);
	}
}

void Protocol18::WriteBoolArray(StreamBuffer& stream, const vector<bool>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)66);
	this->WriteIntLength(stream, value.size());
	int num1 = value.size() >> 3;
	byte* buffer = new byte[num1 + 1];
	int count = 0;
	int index1 = 0;
	while (num1 > 0)
	{
		byte num2 = (byte)0;
		int index2 = index1;
		int num3 = index2 + 1;
		if (value[index2])
			num2 |= (byte)1;
		int index3 = num3;
		int num4 = index3 + 1;
		if (value[index3])
			num2 |= (byte)2;
		int index4 = num4;
		int num5 = index4 + 1;
		if (value[index4])
			num2 |= (byte)4;
		int index5 = num5;
		int num6 = index5 + 1;
		if (value[index5])
			num2 |= (byte)8;
		int index6 = num6;
		int num7 = index6 + 1;
		if (value[index6])
			num2 |= (byte)16;
		int index7 = num7;
		int num8 = index7 + 1;
		if (value[index7])
			num2 |= (byte)32;
		int index8 = num8;
		int num9 = index8 + 1;
		if (value[index8])
			num2 |= (byte)64;
		int index9 = num9;
		index1 = index9 + 1;
		if (value[index9])
			num2 |= (byte)128;
		buffer[count] = num2;
		--num1;
		++count;
	}
	if (index1 < value.size())
	{
		byte num10 = byte(0);
		int num11 = 0;
		for (; index1 < value.size(); ++index1)
		{
			if (value[index1])
				num10 |= (byte)(1 << num11);
			++num11;
		}
		buffer[count] = num10;
		++count;
	}
	stream.Write(buffer, 0, count);
}

void Protocol18::WriteByteArray(StreamBuffer& stream, const vector<byte>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)67);
	this->WriteIntLength(stream, value.size());
	stream.Write(value.data(), 0, value.size());
}

void Protocol18::WriteInt16Array(StreamBuffer& stream, const vector<short>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)68);
	this->WriteIntLength(stream, value.size());
	for (int i = 0; i < value.size(); ++i)
	{
		this->WriteInt16(stream, value[i], false);
	}
}

void Protocol18::WriteSingleArray(StreamBuffer& stream, const vector<float>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)69);
	this->WriteIntLength(stream, value.size());
	int num = value.size() * 4;
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(num, offset);
	memcpy_s(bufferAndAdvance + offset, stream.Length() , value.data(), num);
}

void Protocol18::WriteDoubleArray(StreamBuffer& stream, const vector<double>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)70);
	this->WriteIntLength(stream, value.size());
	int num = value.size() * 8;
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(num, offset);
	memcpy_s(bufferAndAdvance + offset, stream.Length(), value.data(), num);
}

void Protocol18::WriteStringArray(StreamBuffer& stream, const vector<wstring_>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)71);
	this->WriteIntLength(stream, value.size());
	for (int i = 0; i < value.size(); ++i)
	{
		this->WriteString(stream, (const wstring)value[i], false);
	}
}

void Protocol18::WriteInt32ArrayCompressed(StreamBuffer& stream, const vector<int>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)73);
	this->WriteIntLength(stream, value.size());
	for (int i = 0; i < value.size(); ++i)
	{
		this->WriteCompressedInt32(stream, value[i], false);
	}
}

void Protocol18::WriteInt64ArrayCompressed(StreamBuffer& stream, const vector<long long>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)73);
	this->WriteIntLength(stream, value.size());
	for (int i = 0; i < value.size(); ++i)
	{
		this->WriteCompressedInt64(stream, value[i], false);
	}
}

void Protocol18::WriteDictionaryArray(StreamBuffer& stream, const vector<Dictionary>& value, bool writeType)
{
	stream.WriteByte((byte)84);
	GpType keyWriteType;
	GpType valueWriteType;
	this->WriteDictionaryHeader(stream, value.front(), keyWriteType, valueWriteType);
	for (int i = 0; i < value.size(); ++i)
	{
		this->WriteDictionaryElements(stream, value[i], keyWriteType, valueWriteType);
	}
}

void Protocol18::WriteHashtableArray(StreamBuffer& stream, const vector<Hashtable>& value, bool writeType)
{
	if (writeType)
		stream.WriteByte((byte)85);
	this->WriteIntLength(stream, value.size());
	for (int i = 0; i < value.size(); ++i)
	{
		this->WriteHashTable(stream, value[i], false);
	}
}

void Protocol18::WriteParameterTable(StreamBuffer& stream,const unordered_map<byte, Object*>& param)
{
	if (param.empty() || param.size() == 0)
	{
		this->WriteByte(stream, (byte)0, false);
	}
	else
	{
		this->WriteByte(stream, (byte)param.size(), false);

		for(auto it = param.begin(); it != param.end(); ++it)
		{
			stream.WriteByte(it->first);
			this->Write(stream, it->second, true);
		}
	}
}

int Protocol18::DecodeZigZag32(unsigned int value)
{
	return int(((long)(value >> 1) ^ (-(long)(value & 1U))));
}

long long Protocol18::DecodeZigZag64(unsigned long long value)
{
	return (long long)(value >> 1) ^ -((long long)value & 1L);
}

bool Protocol18::ReadBoolean(StreamBuffer& stream)
{
	return stream.ReadByte() > (byte)0;
}

byte Protocol18::ReadByte(StreamBuffer& stream)
{
	return stream.ReadByte();
}

short Protocol18::ReadInt16(StreamBuffer& stream)
{
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(2, offset);
	return (short)((int)bufferAndAdvance[offset] | (int)bufferAndAdvance[offset + 1] << 8);
}

unsigned short Protocol18::ReadUShort(StreamBuffer& stream)
{
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(2, offset);
	return (unsigned short)((unsigned int)bufferAndAdvance[offset] | (unsigned int)bufferAndAdvance[offset + 1] << 8);
}

int Protocol18::ReadInt32(StreamBuffer& stream)
{
	int offset;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(4, offset);
	int num1 = (int)bufferAndAdvance[offset] << 24;
	int num2 = (int)bufferAndAdvance[offset + 1] << 16;
	int num3 = (int)bufferAndAdvance[offset + 2] << 8;
	return  num1 | num2 | num3 | (int)bufferAndAdvance[offset + 3];
}

float Protocol18::ReadSingle(StreamBuffer& stream)
{
	int offset;
	float value;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(4, offset);
	memcpy_s(&value, 4, bufferAndAdvance + offset, 4);
	return value;
}

double Protocol18::ReadDouble(StreamBuffer& stream)
{
	int offset;
	double value;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(8, offset);
	memcpy_s(&value, 8, bufferAndAdvance + offset, 8);
	return value;
}

wstring_ Protocol18::ReadString(StreamBuffer& stream)
{
    unsigned int size = this->ReadCompressedUInt32(stream);
	wstring value = L"";
	if (size == 0)
		return value;
	int offset = 0;
	byte* bufferAndAdvance = stream.GetBufferAndAdvance(size, offset);
	char* utf8Buf = new char[size];
	memcpy_s(utf8Buf, size, bufferAndAdvance + offset, size);

	int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8Buf, size, nullptr, 0);

	value.resize(wideLen);
	MultiByteToWideChar(CP_UTF8, 0, utf8Buf, size, &value[0], wideLen);

	delete[] utf8Buf;
	return value;
}

int Protocol18::ReadCompressedInt32(StreamBuffer& stream)
{
	return this->DecodeZigZag32(this->ReadCompressedUInt32(stream));
}

unsigned int Protocol18::ReadCompressedUInt32(StreamBuffer& stream)
{
	unsigned int num1 = 0;
	int num2 = 0;
	byte* buffer = stream.GetBuffer();
	int position = stream.Position();
	while (num2 != 35)
	{
		if (position >= _msize(buffer))
		{
			//오류처리
		}
		byte num3 = buffer[position];
		++position;
		num1 |= (unsigned int)(((int)num3 & (int)127) << num2);
		num2 += 7;
		if (((int)num3 & 128) == 0)
			break;
	}
	stream.SetPosition(position);
	return num1;
}

long long Protocol18::ReadCompressedInt64(StreamBuffer& stream)
{
	return this->DecodeZigZag64(this->ReadCompressedUInt64(stream));
}

unsigned long long Protocol18::ReadCompressedUInt64(StreamBuffer& stream)
{
	unsigned long long num1 = 0;
	int num2 = 0;
	byte* buffer = stream.GetBuffer();
	int position = stream.Position();
	while (num2 != 70)
	{
		if (position >= _msize(buffer))
		{
			runtime_error("Failed to read full unsigned long");
		}
		byte num3 = buffer[position];
		++position;
		num1 |= (unsigned long long)((int)num3 & (int)127) << num2;
		num2 += 7;
		if (((int)num3 & 128) == 0)
			break;
	}
	stream.SetPosition(position);
	return num1;
}

int Protocol18::ReadInt1(StreamBuffer& stream, bool signNagative)
{
	return signNagative ? -((int)stream.ReadByte()) : (int)stream.ReadByte();
}

int Protocol18::ReadInt2(StreamBuffer& stream, bool signNagative)
{
	return signNagative ? -(this->ReadUShort(stream)) : this->ReadUShort(stream);
}

Hashtable* Protocol18::ReadHashtable(StreamBuffer& stream)
{
	int count = this->ReadCompressedUInt32(stream);
	Hashtable* hash = new Hashtable();
	for (int i = 0; i < count; ++i)
	{
		Object* key = this->Read(stream);
		Object* obj = this->Read(stream);
		if (key->GetSize() != 0)
		{
			hash->Insert(make_pair(key,obj));
		}
		else
			std::runtime_error("Hashtable : key is Null");
	}
	return hash;
}

void Protocol18::ReadDictionaryType(StreamBuffer& stream, GpType& keyType, GpType& valueType)
{
	keyType = (GpType)stream.ReadByte();
	valueType = (GpType)stream.ReadByte();
	//keyType = keyGpType != GpType::Unknown ? this->GetAllowedDictionaryKeyType(keyGpType) : &typeid(Object);
	switch (valueType)
	{
		//case GpType::Unknown:
		   // break;
	case GpType::Dictionary:
		GpType _;
		this->ReadDictionaryType(stream, _, _);
		break;
	case GpType::Array:
		//현재 Dictionary에 형식을 지정하는 기능이 없기때문에 정보를 날립니다.
		GpType gpType = (GpType)stream.ReadByte();
		for (; gpType == GpType::Array; gpType = (GpType)stream.ReadByte());
		valueType = GpType::Unknown;
		break;
		//case GpType::HashtableArray:
		   // break;
		//default:
		   // //this->GetClrArrayType(gpType); ㄴ
		   // break;
	}
}

bool Protocol18::ReadDictionaryElements(StreamBuffer& stream, GpType& keyType, GpType& valueType, Dictionary& dic)
{
	unsigned int count = this->ReadCompressedUInt32(stream);

	for (int i = 0; (long)i < (long)count; ++i)
	{
		Object* key;
		cout << "READ KEY TYPE2 : " << (int)keyType << endl;
		key = keyType == GpType::Unknown ? this->Read(stream) : this->Read(stream, (byte)keyType);
		Object* value;
		value = valueType == GpType::Unknown ? this->Read(stream) : this->Read(stream, (byte)valueType);
		if (key != nullptr)
			dic.Insert(make_pair(key, value));
		else
			runtime_error("Dictionary key is null");
	}
	return true;
}


Dictionary* Protocol18::ReadDictionary(StreamBuffer& stream)
{
    GpType keyType;
	GpType valueType;

	this->ReadDictionaryType(stream, keyType, valueType);

	Dictionary* dic = new Dictionary();
	this->ReadDictionaryElements(stream, keyType, valueType, *dic);
	return dic;
}

vector_<Object*>* Protocol18::ReadArrayInArray(StreamBuffer& stream)
{
	unsigned int num = this->ReadCompressedUInt32(stream);
	vector_<Object*>* value = new vector_<Object*>(num);
	for (int i = 0; i < num; ++i)
	{
		(**value)[i] = this->Read(stream);
	}
	return value;
}

vector_<bool>* Protocol18::ReadBooleanArray(StreamBuffer& stream)
{
	unsigned int num1 = this->ReadCompressedUInt32(stream);
	vector_<bool>* boolArray = new vector_<bool>((int)num1);
	int num2 = (int)num1 / 8;
	int num3 = 0;
	for (; num2 > 0; --num2);
	{
		byte num4 = stream.ReadByte();
		int index1 = num3;
		int num5 = index1 + 1;
		int num6 = ((int)num4 & 1) == 1 ? 1 : 0;
		(**boolArray)[index1] = num6 != 0;
		
		int index2 = num5;
		int num7 = index2 + 1;
		int num8 = ((int)num4 & 2) == 2 ? 1 : 0;
		(**boolArray)[index2] = num8 != 0;

		int index3 = num7;
		int num9 = index3 + 1;
		int num10 = ((int)num4 & 4) == 4 ? 1 : 0;
		(**boolArray)[index3] = num10 != 0;

		int index4 = num9;
		int num11 = index4 + 1;
		int num12 = ((int)num4 & 8) ? 1 : 0;
		(**boolArray)[index4] = num12 != 0;

		int index5 = num11;
		int num13 = index5 + 1;
		int num14 = ((int)num4 & 16) == 16 ? 1 : 0;
		(**boolArray)[index5] = num14 != 0;

		int index6 = num13;
		int num15 = index6 + 1;
		int num16 = ((int)num4 & 32) == 32 ? 1 : 0;
		(**boolArray)[index6] = num16 != 0;

		int index7 = num15;
		int num17 = index7 + 1;
		int num18 = ((int)num4 & 64) == 64 ? 1 : 0;
		(**boolArray)[index7] = num18 != 0;

		int index8 = num17;
		num3 = index8 + 1;
		int num19 = ((int)num4 & 128) == 128 ? 1 : 0;
		(**boolArray)[index8] = num19 != 0;
	}
	if ((long long)num3 < (long long)num1)
	{
		byte num20 = stream.ReadByte();
		int index = 0;
		while ((long long)num3 < (long long)num1)
		{
			(**boolArray)[num3++] = ((int)num20 & (int)this->boolMask[index]) == (int)this->boolMask[index];
			++index;
		}
	}
	return boolArray;
}

vector_<byte>* Protocol18::ReadByteArray(StreamBuffer& stream)
{
	unsigned int num = this->ReadCompressedUInt32(stream);
	vector_<byte>* buffer = new vector_<byte>(num);
	stream.Read((**buffer).data(), 0, (int)num);
	return buffer;
}

vector_<short>* Protocol18::ReadInt16Array(StreamBuffer& stream)
{
	vector_<short>* numArray = new vector_<short>((int)this->ReadCompressedUInt32(stream));
	for (int i = 0; i < numArray->GetSize(); ++i)
	{
		(**numArray)[i] = this->ReadInt16(stream);
	}
	return numArray;
}

vector_<float>* Protocol18::ReadSingleArray(StreamBuffer& stream)
{
	int length = (int)this->ReadCompressedUInt32(stream);
	int num = length * 4;
	vector_<float>* floatArray = new vector_<float>(length);
	int offset = 0;
	byte* buffer = stream.GetBufferAndAdvance(num, offset);
	memcpy_s((**floatArray).data(), num, buffer + offset, num);
	return floatArray;
}

vector_<double>* Protocol18::ReadDoubleArray(StreamBuffer& stream)
{
	int length = (int)this->ReadCompressedUInt32(stream);
	int num = length * 8;
	vector_<double>* doubleArray = new vector_<double>(length);
	int offset = 0;
	byte* buffer = stream.GetBufferAndAdvance(num, offset);
	memcpy_s((**doubleArray).data(), num, buffer + offset, num);
	return doubleArray;
}

vector_<int>* Protocol18::ReadCompressedInt32Array(StreamBuffer& stream)
{
	vector_<int>* numArray = new vector_<int>(this->ReadCompressedUInt32(stream));
	for (int i = 0; i < numArray->GetSize(); ++i)
	{
		(**numArray)[i] = this->ReadCompressedInt32(stream);
	}
	return numArray;
}

vector_<long long>* Protocol18::ReadCompressedInt64Array(StreamBuffer& stream)
{
	vector_<long long>* numArray = new vector_<long long>(this->ReadCompressedUInt32(stream));
	for (int i = 0; i < numArray->GetSize(); ++i)
	{
		(**numArray)[i] = this->ReadCompressedInt64(stream);
	}
	return numArray;
}

vector_<wstring_>* Protocol18::ReadStringArray(StreamBuffer& stream)
{
	vector_<wstring_>* strArray = new vector_<wstring_>(this->ReadCompressedUInt32(stream));
	for (int i = 0; i < strArray->GetSize(); ++i)
	{
		(**strArray)[i] = this->ReadString(stream);
	}
	return strArray;
}

vector_<Dictionary*>* Protocol18::ReadDictionaryArray(StreamBuffer& stream)
{
	GpType keyReadType;
	GpType valueReadType;
	this->ReadDictionaryType(stream, keyReadType, valueReadType);
	unsigned int length = this->ReadCompressedUInt32(stream);
	vector_<Dictionary*>* dicArray = new vector_<Dictionary*>(length);
	for (int i = 0; i < length; ++i)
	{
		this->ReadDictionaryElements(stream, keyReadType, valueReadType, *(**dicArray)[i]);
	}
	return dicArray;
}

vector_<Hashtable*>* Protocol18::ReadHashtableArray(StreamBuffer& stream)
{
	unsigned int length = ReadCompressedUInt32(stream);
	vector_<Hashtable*>* hashtableArray = new vector_<Hashtable*>(length);
	for (int i = 0; i < length; ++i)
	{
		(**hashtableArray)[i] = this->ReadHashtable(stream);
	}
	return hashtableArray;
}

unordered_map<byte, Object*>* Protocol18::ReadParameterTable(StreamBuffer& stream, unordered_map<byte, Object*>* target)
{
	short num = (short)stream.ReadByte();
	unordered_map<byte, Object*>* map = target != NULL ? target : new unordered_map<byte, Object*>((int)num);
	for (int i = 0; i < (int)num; ++i)
	{
		byte key = stream.ReadByte();
		byte gpType = stream.ReadByte();
		Object* obj = this->Read(stream, gpType);
		map->insert(make_pair(key, obj));
	}
	return map;
}


void Protocol18::SerializeOperationRequest(StreamBuffer& stream, byte opCode,const unordered_map<byte, Object*>& param, bool setType)
{
	if (setType)
		stream.WriteByte((byte)24);
	stream.WriteByte(opCode);
	this->WriteParameterTable(stream, param);
}




void Protocol18::SerializeOperationRequest(StreamBuffer& stream,const OperationRequest& serObject, bool setType)
{
	this->SerializeOperationRequest(stream, serObject.OperationCode, *serObject.Parameters, setType);
}

void Protocol18::SerializeOperationResponse(StreamBuffer& stream, const OperationResponse& serObject, bool setType)
{
	if (setType)
		stream.WriteByte((byte)25);
	stream.WriteByte(serObject.OperatoinCode);
	this->WriteInt16(stream, serObject.ReturnCode, false);
	if (serObject.DebugMessage.empty())
	{
		stream.WriteByte((byte)0);
	}
	else
	{
		this->WriteString(stream, serObject.DebugMessage, true);
	}
	this->WriteParameterTable(stream, *serObject.Parameters);
}


void Protocol18::Write(StreamBuffer& stream,const Object* object, bool writeType)
{

	if (object->GetData() == NULL)
		this->Write(stream, object, GpType::Null, writeType);
	else
		this->Write(stream, object, this->GetCodeOfType(object), writeType);
}


void Protocol18::Write(StreamBuffer& stream, const Object* value, GpType gpType, bool writeType)
{
	switch (gpType)
	{
	case GpType::Unknown:
		//미구현
		break;
	case GpType::Boolean:
		this->WriteBoolean(stream, **(generic_<bool>*)value, writeType);
		break;
	case GpType::Byte:
		this->WriteByte(stream, **(generic_<byte>*)value, writeType);
		break;
	case GpType::Short:
		this->WriteInt16(stream, **(generic_<short>*)value, writeType);
		break;
	case GpType::Float:
		this->WriteSingle(stream, **(generic_<float>*)value, writeType);
		break;
	case GpType::Double:
		this->WriteDouble(stream, **(generic_<double>*)value, writeType);
		break;
	case GpType::String:
		this->WriteString(stream, **(wstring_*)value, writeType);
		break;
	case GpType::Null:
		if (!writeType)
			break;
		stream.WriteByte((byte)8);
		break;
	case GpType::CompressedInt:
		this->WriteCompressedInt32(stream, **(generic_<int>*)value, writeType);
		break;
	case GpType::CompressedLong:
		this->WriteCompressedInt64(stream, **(generic_<long long>*)value, writeType);
		break;
	case GpType::Dictionary:
		this->WriteDictionary(stream, *(Dictionary*)value, writeType);
		break;
	case GpType::Hashtable:
		this->WriteHashTable(stream, *(Hashtable*)value, writeType);
		break;
	//case GpType::ObjectArray:
	//	break;
	//case GpType::OperationRequest:
	//	this->SerializeOperationRequest(stream, *(OperationRequest*)value, writeType);
	//	break;
	case GpType::Array:
		this->WriteArrayInArray(stream, value, writeType);
		break;
	case GpType::BooleanArray:
		this->WriteBoolArray(stream, **(vector_<bool>*)value, writeType);
		break;
	case GpType::ByteArray:
		this->WriteByteArray(stream, **(vector_<byte>*)value, writeType);
		break;
	case GpType::ShortArray:
		this->WriteInt16Array(stream, **(vector_<short>*)value, writeType);
		break;
	case GpType::FloatArray:
		this->WriteSingleArray(stream, **(vector_<float>*)value, writeType);
		break;
	case GpType::DoubleArray:
		this->WriteDoubleArray(stream, **(vector_<double>*)value, writeType);
		break;
	case GpType::StringArray:
		this->WriteStringArray(stream, **(vector_<wstring_>*) value, writeType);
		break;
	case GpType::CompressedIntArray:
		this->WriteInt32ArrayCompressed(stream, **(vector_<int>*)value, writeType);
		break;
	case GpType::CompressedLongArray:
		this->WriteInt64ArrayCompressed(stream, **(vector_<long long>*)value, writeType);
		break;
	case GpType::CustomTypeArray:
		break;
	case GpType::DictionaryArray:
		this->WriteDictionaryArray(stream, **(vector_<Dictionary>*)value, writeType);
		break;
	case GpType::HashtableArray:
		this->WriteHashtableArray(stream, **(vector_<Hashtable>*)value, writeType);
		break;


	default:
		runtime_error("Uknown type");
		break;
	}
}

void Protocol18::Serialize(StreamBuffer& stream,const Object* object, bool setType)
{
	this->Write(stream, object, setType);
}


Object* Protocol18::Read(StreamBuffer& stream, byte gpType)
{
	cout << "STREM GPTYPE : " << (int)gpType << endl;
	//if customType
	switch (gpType)
	{
	case (byte)2:
		return new generic_<bool>(this->ReadBoolean(stream));
	case (byte)3:
		return new generic_<byte>(this->ReadByte(stream));
	case (byte)4:
		return new generic_<short>(this->ReadInt16(stream));
	case (byte)5:
		return new generic_<float>(this->ReadSingle(stream));
	case (byte)6:
		return new generic_<double>(this->ReadDouble(stream));
	case (byte)7:
		return  new wstring_(this->ReadString(stream));
	case (byte)9:
		return new generic_<int>(this->ReadCompressedInt32(stream));
	case (byte)10:
		return new generic_<long long>(this->ReadCompressedInt64(stream));
	case (byte)11:
		return new generic_<int>(this->ReadInt1(stream, false));
	case (byte)12:
		return new generic_<int>(this->ReadInt1(stream, true));
	case (byte)13:
		return new generic_<int>(this->ReadInt2(stream, false));
	case (byte)14:
		return new generic_<int>(this->ReadInt2(stream, true));
	case (byte)15:
		return new generic_<long long>(this->ReadInt1(stream, false));
	case (byte)16:
		return new generic_<long long>(this->ReadInt1(stream, true));
	case (byte)17:
		return new generic_<long long>(this->ReadInt2(stream, false));
	case (byte)18:
	    return new generic_<long long>(this->ReadInt2(stream, true));
	case (byte)20:
		return this->ReadDictionary(stream);
	case (byte)21:
		return this->ReadHashtable(stream);

	//case (byte)24:
	//	return this->DeserializeOperationRequest(stream);
	//case (byte)25:
	//	return this->DeserializeOperationResponse(stream);

	case (byte)27:
	{
		return new generic_<bool>(false);
	}
	case (byte)28:
	{
		return new generic_<bool>(true);
	}
	case (byte)29:
	{
		return new generic_<short>((short)0);
	}
	case (byte)30:
	{
		return new generic_<int>((int)0);
	}
	case (byte)31:
	{
		return new generic_<long long>(0L);
	}
	case (byte)32:
	{
		return new generic_<float>(0.0f);
	}
	case (byte)33:
	{
		return new generic_<double>(0.0);
	}
	case (byte)34:
	{
		return new generic_<byte>((byte)0);
	}
	case (byte)64:
		return this->ReadArrayInArray(stream);
	case (byte)66:
		return this->ReadBooleanArray(stream);
	case (byte)67:
		return this->ReadByteArray(stream);
	case (byte)68:
		return this->ReadInt16Array(stream);
	case (byte)69:
		return this->ReadSingleArray(stream);
	case (byte)70:
		return this->ReadDoubleArray(stream);
	case (byte)71:
		return this->ReadStringArray(stream);
	case (byte)73:
		return this->ReadCompressedInt32Array(stream);
	case (byte)74:
		return this->ReadCompressedInt64Array(stream);
	//case (byte)83:
	//	return this->ReadCustomTypeArray(stream);
	case (byte)84:
		return this->ReadDictionaryArray(stream);
	case (byte)85:
		return this->ReadHashtableArray(stream);

	default:
		runtime_error("Read Fail");
			break;
	}
}


Object* Protocol18::Read(StreamBuffer& stream)
{
	return this->Read(stream, stream.ReadByte());
}

const type_info* Protocol18::GetAllowedDictionaryKeyType(GpType gpType)
{
	switch (gpType)
	{
	case GpType::Byte:
	case GpType::ByteZero:
		return &typeid(byte);
	case GpType::Short:
	case GpType::ShortZero:
		return &typeid(short);
	case GpType::Float:
	case GpType::FloatZero:
		return &typeid(float);
	case GpType::Double:
	case GpType::DoubleZero:
		return &typeid(double);
	case GpType::String:
		return &typeid(wstring);
	case GpType::CompressedInt:
	case GpType::Int1:
	case GpType::Int1_:
	case GpType::Int2:
	case GpType::Int2_:
	case GpType::IntZero:
		return &typeid(int);
	case GpType::CompressedLong:
	case GpType::L1:
	case GpType::L1_:
	case GpType::L2:
	case GpType::L2_:
	case GpType::LongZero:
		return &typeid(long long);

	default:
		break;
		//오류 처리
		
	}
	return nullptr;
}

OperationRequest* Protocol18::DeserializeOperationRequest(StreamBuffer& stream)
{
	OperationRequest* op = new OperationRequest();
	op->OperationCode = stream.ReadByte();
	op->Parameters = this->ReadParameterTable(stream);

	for (auto it = op->Parameters->begin(); it != op->Parameters->end(); ++it)
	{
		cout << "Pram Code : " << (int)it->first << endl;
		cout << "TYPE : " << it->second->GetType()->name() << endl;
	}
	return op;
	//return &OperationRequest();
}

OperationResponse* Protocol18::DeserializeOperationResponse(StreamBuffer& stream)
{
	OperationResponse* op = new OperationResponse();
	op->OperatoinCode = this->ReadByte(stream);
	op->ReturnCode = this->ReadInt16(stream);
	this->ReadByte(stream);
	op->DebugMessage = this->ReadString(stream);
	op->Parameters = this->ReadParameterTable(stream);
	return op;
	//return& OperationResponse();
}

Object* Protocol18::Deserialize(StreamBuffer& stream, byte type)
{
	return this->Read(stream, type);
}


