#include "buffer.h"

char CBuffer::retval[20001];

CBuffer::CBuffer()
{
	BuffSize = 30;
	data = static_cast<char*>(malloc(BuffSize));
	count = 0;
	readpos = 0;
	writepos = 0;
}

CBuffer::~CBuffer()
{
	if (data != nullptr)
		free(data);
}

void CBuffer::StreamWrite(void* in, int size)
{
	if (writepos + size >= BuffSize)
	{
		BuffSize = writepos + size + 30;
		if ((data = static_cast<char*>(realloc(data, BuffSize))) == nullptr)return;
	}
	memcpy(data + writepos, in, size);
	writepos += size;
	if (writepos > count)count = writepos;
}

void CBuffer::StreamRead(void* out, int size)
{
	if (readpos + size > count) size = count - readpos;
	if (size <= 0)return;
	memcpy(out, data + readpos, size);
	readpos += size;
}

int CBuffer::writebyte(unsigned char val)
{
	StreamWrite(&val, 1);
	return 1;
}

int CBuffer::writeshort(short a)
{
	StreamWrite(&a, 2);
	return 2;
}

int CBuffer::writeushort(unsigned short a)
{
	StreamWrite(&a, 2);
	return 2;
}

int CBuffer::writeint(int a)
{
	StreamWrite(&a, 4);
	return 4;
}

int CBuffer::writeuint(unsigned int a)
{
	StreamWrite(&a, 4);
	return 4;
}

int CBuffer::writefloat(float a)
{
	StreamWrite(&a, 4);
	return 4;
}

int CBuffer::writedouble(double a)
{
	StreamWrite(&a, 8);
	return 8;
}

int CBuffer::writechars(char* str)
{
	auto len = static_cast<int>(strlen(str));
	StreamWrite(str, len);
	return len;
}

int CBuffer::writestring(std::string str)
{
	auto txtChar = new char[str.size() + 1];
	txtChar[str.size()] = 0;
	memcpy(txtChar, str.c_str(), str.size());

	auto len = writechars(txtChar);
	return len + writebyte('\0');
}

unsigned char CBuffer::readbyte()
{
	unsigned char b;
	StreamRead(&b, 1);
	return b;
}

short CBuffer::readshort()
{
	short b;
	StreamRead(&b, 2);
	return b;
}

unsigned short CBuffer::readushort()
{
	unsigned short b;
	StreamRead(&b, 2);
	return b;
}

int CBuffer::readint()
{
	int b;
	StreamRead(&b, 4);
	return b;
}

unsigned int CBuffer::readuint()
{
	unsigned int b;
	StreamRead(&b, 4);
	return b;
}

float CBuffer::readfloat()
{
	float b;
	StreamRead(&b, 4);
	return b;
}

double CBuffer::readdouble()
{
	double b;
	StreamRead(&b, 8);
	return b;
}

char* CBuffer::readchars(int len)
{
	if (len < 0)return nullptr;
	StreamRead(&retval, len);
	retval[len] = '\0';
	return retval;
}

char* CBuffer::readstring()
{
	int i;
	for (i = readpos; i < count; i++)
		{
			if (data[i] == '\0')
				break;
		}
	if (i == count)return nullptr;
	i -= readpos;
	i = std::min(20000, i);
	StreamRead(&retval, i + 1);
	return retval;
}

int CBuffer::bytesleft() const
{
	return count - readpos;
}

char* CBuffer::readsep(char* sep)
{
	int i, ii;
	auto len = static_cast<int>(strlen(sep));
	auto len2 = -1;
	for (i = readpos; i < count; i++)
		{
			for (ii = 0; ii < len; ii++)
				{
					if (data[i + ii] != sep[ii])
						break;
				}
			if (ii == len)
			{
				len2 = i - readpos;
				break;
			}
		}
	if (len2 == -1)
		return nullptr;
	len2 = std::min(len2, 20000);
	memcpy(retval, data + readpos, len2);
	retval[len2] = '\0';
	readpos += len2 + len;
	return retval;
}

int CBuffer::addBuffer(CBuffer* buffer)
{
	StreamWrite(buffer->data, buffer->count);
	return buffer->count;
}

int CBuffer::addBuffer(char* data, int len)
{
	StreamWrite(data, len);
	return len;
}

void CBuffer::clear()
{
	if (BuffSize > 30)
	{
		free(data);
		BuffSize = 30;
		data = static_cast<char*>(malloc(BuffSize));
	}
	count = 0;
	readpos = 0;
	writepos = 0;
}

void CBuffer::StreamSet(int pos)
{
	readpos = 0;
	writepos = 0;
}

char CBuffer::operator [](int i) const
{
	if (i < 0 || i >= count)return '\0';
	return data[i];
}
