
#ifndef __CCNET_BUFFER_H__
#define __CCNET_BUFFER_H__

#define CCBUFFER_ORGINAL_SIZE 512

class CCBuffer : public Ref
{
public:
	CCBuffer();
	explicit CCBuffer(unsigned int n_capacity);
	CCBuffer(const char* p_data, unsigned int u_len);
	virtual ~CCBuffer();
	static CCBuffer* create();
	static CCBuffer* create(unsigned int n_capacity);
	static CCBuffer* create(const char* p_data, unsigned int u_len);

public:
	unsigned int length() const;
	unsigned int capacity() const;
	bool empty() const;
	char* data() const;
	void clear();

public:
	unsigned int getWriterIndex() const;
	unsigned int getContentSize() const;
	unsigned int getReadableSize() const;
	unsigned int getReaderIndex() const;
	unsigned int getBufferSize() const;
	void discardReadContent();
	void skipData(unsigned int u_len);
	void moveWriterIndexToFront();
	void moveWriterIndexToBack();
	void moveReaderIndexToFront();
	void moveReaderIndexToBack();
	void markReaderIndex();
	void resetReaderIndex();
	void reset();
	void setReaderIndex(unsigned int u_pos);
	void setWriterIndex(unsigned int u_pos);
	void moveLeft(unsigned int u_len);
	void moveRight(unsigned int u_len);
	bool isReadable() const;
	bool isReadable(unsigned int u_len) const;

public:
	void writeData(const char* p_data, unsigned int u_len);
	void writeLengthAndData(const char* p_data, unsigned int u_len);
	void writeChar(char data);
	void writeUChar(unsigned char data);
	void writeShort(short data);
	void writeUShort(unsigned short data);
	void writeInt(int data);
	void writeUInt(unsigned int data);
	void writeLong(long  data);
	void writeULong(unsigned  long data);
	void writeFloat(float data);
	void writeDouble(double data);
	void writeString(const char* p_data);
	void writeLengthAndString(const char* p_data);

public:
	// warning: need to delete[]
	char* readWholeData();

public:
	CCBuffer* readData(unsigned int u_len);
	void readData(char* p_out_data, unsigned int u_len);
	char readChar();
	unsigned char readUChar();
	short readShort();
	unsigned short readUShort();
	int readInt();
	unsigned int readUInt();
	long  readLong();
	unsigned  long readULong();
	float readFloat();
	double readDouble();
	std::string readString(unsigned int u_len);
	std::string readLengthAndString();

protected:
	inline void _reallocBufferSizeInChanged(unsigned int u_len);
	inline void _reallocBufferSize();

protected:
	char* _p_buffer;
	unsigned int _u_write_pos;
	unsigned int _u_read_pos;
	unsigned int _u_mark_pos;
	unsigned int _u_content_size;
	unsigned int _u_buffer_size;
	
};



#endif //__CCNET_BUFFER_H__