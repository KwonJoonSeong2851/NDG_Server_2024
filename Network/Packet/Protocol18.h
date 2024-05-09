#pragma once
#include "../../stdafx.h"
//#include "../Protocol.h"
//
//#include "Object.h"
//#include "StreamBuffer.h"
//#include "Operation.h"
//#include "GpType.h"
//#include "Dictionary.h"
//#include "Hashtable.h"
//#include "generic_.h"
//#include "wstring_.h"
//#include "vector_.h"




//typedef unsigned char byte;
//class StreamBuffer;
//class StreamBuffer;
//class Protocol;
class Protocol18 : public Protocol
{
private:
	float* memFloatBlock = new float[1];
	double* memDoubleBlock = new double[1];
	//byte* memCompressedUInt32 = new byte[5];
	byte* memCompressedUInt64 = new byte[10];

	const byte boolMask[8]
	{
		(byte)1,
		(byte)2,
		(byte)4,
		(byte)8,
		(byte)16,
		(byte)32,
		(byte)64,
		(byte)128,
	};


	unsigned int EncodeZigZag32(int value);
	unsigned long long EncodeZigZag64(long long value);
	int WriteCompressedUInt32(byte* buffer,  unsigned int value);
	void WriteCompressedUInt32(StreamBuffer& stream,const unsigned int& value);
	void WriteCompressedUInt64(StreamBuffer& stream,const unsigned long long& value);
	void WriteCompressedInt32(StreamBuffer& stream,const int& value, bool writeType);
	void WriteCompressedInt64(StreamBuffer& stream,const long long& value, bool writeType);
	void WriteIntLength(StreamBuffer& stream,const int& value) { this->WriteCompressedUInt32(stream, (unsigned int)value); }

	void WriteBoolean(StreamBuffer& stream, const bool& value, bool writeType);
	void WriteByte(StreamBuffer& stream, const byte& value, bool writeType);
	void WriteUShort(StreamBuffer& stream, const unsigned short& value);
	void WriteInt16(StreamBuffer& stream, const short& value, bool writeType);
	void WriteSingle(StreamBuffer& stream, const float& value, bool writeType);
	void WriteDouble(StreamBuffer& stream, const double& value, bool writeType);
	void WriteString(StreamBuffer& stream, const wstring& value, bool writeType);

	void WriteVector3(StreamBuffer& stream, const Vector3& value, bool writeType);
	void WriteQuaternion(StreamBuffer& stream, const Quaternion& value, bool writeType);
	
	//void WriteCustomType(StreamBuffer& stream, Object* value, bool writeType);
	void WriteHashTable(StreamBuffer& stream, const Hashtable& value, bool writeType);
	void WriteDictionaryElements(StreamBuffer& stream,const Dictionary& dic, GpType& keyType, GpType& valueType);
	void WriteDictionaryHeader(StreamBuffer& stream, const Dictionary& value, GpType& keyType, GpType& valueType);
	void WriteDictionary(StreamBuffer& stream, const Dictionary& value, bool writeType);
	bool WriteArrayType(StreamBuffer& stream, const Object* object, GpType& writeType);
	void WriteObjectArray(StreamBuffer& stream, const vector<Object*>& value, bool writeType);
	void WriteArrayInArray(StreamBuffer& stream, const Object* object, bool writeType);
	void WriteBoolArray(StreamBuffer& stream, const vector<bool>& value, bool writeType);
	void WriteByteArray(StreamBuffer& stream, const vector<byte>& value, bool writeType);
	void WriteInt16Array(StreamBuffer& stream, const vector<short>& value, bool writeType);
	void WriteSingleArray(StreamBuffer& stream, const vector<float>& value, bool writeType);
	void WriteDoubleArray(StreamBuffer& stream, const vector<double>& value, bool writeType);
	void WriteStringArray(StreamBuffer& stream, const vector<wstring_>& value, bool writeType);
	void WriteInt32ArrayCompressed(StreamBuffer& stream, const vector<int>& value, bool writeType);
	void WriteInt64ArrayCompressed(StreamBuffer& stream, const vector<long long>& value, bool writeType);
	//void WriteCustomTypeArray(StreamBuffer& stream, vector<CustomType>* value, bool writeType);
	void WriteDictionaryArray(StreamBuffer& stream, const vector<Dictionary>& value, bool writeType);
	void WriteHashtableArray(StreamBuffer& stream, const vector<Hashtable>& value, bool writeType);

	void SerializeOperationRequest(StreamBuffer& stream, byte opCode, const unordered_map<byte, Object*>& param, bool setType);
	
	void WriteParameterTable(StreamBuffer& stream, const unordered_map<byte, Object*>& param);
	void Write(StreamBuffer& stream,const Object* value, GpType gpType, bool writeType);
	void Write(StreamBuffer& stream,const Object* object, bool writeType);



	int DecodeZigZag32(unsigned int value);
	long long DecodeZigZag64(unsigned long long value);
	bool ReadBoolean(StreamBuffer& stream);
	byte ReadByte(StreamBuffer& stream);
	short ReadInt16(StreamBuffer& stream);
	unsigned short ReadUShort(StreamBuffer& stream);
	int ReadInt32(StreamBuffer& stream);
	float ReadSingle(StreamBuffer& stream);
	double ReadDouble(StreamBuffer& stream);
	int ReadCompressedInt32(StreamBuffer& stream);
	unsigned int ReadCompressedUInt32(StreamBuffer& stream);
	long long ReadCompressedInt64(StreamBuffer& stream);
	unsigned long long ReadCompressedUInt64(StreamBuffer& stream);
	int ReadInt1(StreamBuffer& stream, bool signNagative);
	int ReadInt2(StreamBuffer& stream, bool signNagative);

	Vector3* ReadVector3(StreamBuffer& stream);
	Quaternion* ReadQuaternion(StreamBuffer& stream);

	wstring_ ReadString(StreamBuffer& stream);
	Hashtable* ReadHashtable(StreamBuffer& stream);
	void ReadDictionaryType(StreamBuffer& stream, GpType& keyType, GpType& valueType);
	bool ReadDictionaryElements(StreamBuffer& stream, GpType& keyType, GpType& valueType, Dictionary& instance);
	Dictionary* ReadDictionary(StreamBuffer& stream);

	vector_<bool>* ReadBooleanArray(StreamBuffer& stream);
	vector_<byte>* ReadByteArray(StreamBuffer& stream);
	vector_<short>* ReadInt16Array(StreamBuffer& stream);
	vector_<float>* ReadSingleArray(StreamBuffer& stream);
	vector_<double>* ReadDoubleArray(StreamBuffer& stream);
	vector_<int>* ReadCompressedInt32Array(StreamBuffer& stream);
	vector_<long long>* ReadCompressedInt64Array(StreamBuffer& stream);
	//vector_<CustomType> ReadCustomTypeArray(StreamBuffer& stream);
	vector_<Object*>* ReadObjectArray(StreamBuffer& stream);
	vector_<Object*>* ReadArrayInArray(StreamBuffer& stream);
	vector_<wstring_>* ReadStringArray(StreamBuffer& stream);
	vector_<Dictionary*>* ReadDictionaryArray(StreamBuffer& stream);
	vector_<Hashtable*>* ReadHashtableArray(StreamBuffer& stream);

	unordered_map<byte, Object*>* ReadParameterTable(StreamBuffer& stream, unordered_map<byte, Object*>* target = NULL);

	Object* Read(StreamBuffer& stream, byte gpType);
	Object* Read(StreamBuffer& stream);

	const type_info* GetAllowedDictionaryKeyType(GpType gpType);
public:
	Protocol18() {};
	virtual ~Protocol18() {};

	const type_info* GetTypeOfCode(GpType gpType);
	//GpType GetArrayType(Object* object);
	GpType GetCodeOfType(const Object* object);
	void Serialize(StreamBuffer& stream,const Object* object, bool setType);

	virtual void SerializeOperationRequest(StreamBuffer& stream, const PK_OperationRequest& serObject, bool setType) override;
	virtual void SerializeOperationResponse(StreamBuffer& stream, const PK_OperationResponse& serObject, bool setType) override;
	virtual void SerializeEventData(StreamBuffer& stream, const PK_EventData& serObject, bool setType) override;

	virtual PK_OperationRequest* DeserializeOperationRequest(StreamBuffer& stream) override;
	virtual PK_OperationResponse* DeserializeOperationResponse(StreamBuffer& stream) override;
	Object* Deserialize(StreamBuffer& stream, byte type);

};