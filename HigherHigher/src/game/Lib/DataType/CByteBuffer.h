/*
///written by Duy Nguyen
*/
#if !defined(_H_CBYTEBUFFER)
#define _H_CBYTEBUFFER
class Bytes{
public:
	char* Data;
	int Lenght;
	Bytes(){Data = NULL;Lenght = 0;};
	~Bytes(){if(Data != NULL)delete[] Data;Data = NULL; Lenght = 0;};
};
class CByteBuffer{
private:
	char* _data;
	int _data_len;
	int _curr_pos;
	CByteBuffer();
public:
	
	~CByteBuffer();
	static CByteBuffer* allocate(int size);
	static CByteBuffer* swap(const char* bytes, int len);
	void writeByte(char data);
	char readByte();
	void writeBytes(char* data, int len);
	char* readBytes(int len);
	char* getBinary();
	int getSize();
	const char* getCurrentDataPointer();
	void pass(int len);

	static INT8 readInt8(CByteBuffer* buf);
	static INT16 readInt16(CByteBuffer* buf);
	static INT32 readInt32(CByteBuffer* buf);
	static float readFloat(CByteBuffer* buf);
	static Bytes* readBin(CByteBuffer* buf, int len);
	static Bytes* readBBin(CByteBuffer* buf);
	static Bytes* readSBin(CByteBuffer* buf);
	static Bytes* readBString(CByteBuffer* buf);
	static Bytes* readSString(CByteBuffer* buf);

	static void writeInt8(INT8 val, CByteBuffer* buf);
	static void writeInt16(INT16 val, CByteBuffer* buf);
	static void writeInt32(INT32 val, CByteBuffer* buf);
	static void writeFloat(float val, CByteBuffer* buf);
	static void writeBin(Bytes* bin, CByteBuffer* buf);
	static void writeBBin(Bytes* bin, CByteBuffer* buf);
	static void writeSBin(Bytes* bin, CByteBuffer* buf);
	static void writeBString(Bytes* str,CByteBuffer* buf);
	static void writeSString(Bytes* str,CByteBuffer* buf);
};
#endif//_H_CBYTEBUFFER