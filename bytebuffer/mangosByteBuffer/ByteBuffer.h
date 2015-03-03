
#ifndef __ByteBuffer_H__
#define __ByteBuffer_H__


#include "TypeDefine.h"
#include <assert.h>
#include<string.h>
#include<stdio.h>


#include <vector>
#include<string>


#include<stdint.h>


typedef   int8_t 			int8;
	typedef uint8_t 	uint8;
	typedef int16_t		int16;
	typedef uint16_t		uint16;
	typedef  int32_t		int32;
	typedef  uint32_t		uint32;
	typedef int64_t		int64;
	typedef uint64_t		uint64;
	
	
//////////////////////////////////////////////////////////////////////////
/// 字节流缓冲类，可以进行序列化和解序列化操作，并且可以缓冲字节流数据。
//////////////////////////////////////////////////////////////////////////
class ByteBuffer
{
public:
	const static size_t DEFAULT_SIZE = 0x1000;

	ByteBuffer()
		: mReadPos(0)
		, mWritePos(0)
	{
		mStorage.reserve(DEFAULT_SIZE);
	}

	ByteBuffer(size_t res)
		: mReadPos(0)
		, mWritePos(0)
	{
		mStorage.reserve(res);
	}

	ByteBuffer(const ByteBuffer &buf) 
		: mReadPos(buf.mReadPos)
		, mWritePos(buf.mWritePos)
		, mStorage(buf.mStorage)
	{}

	//////////////////////////////////////////////////////////////////////////
public:
	void clear()
	{
		mStorage.clear();
		mReadPos = mWritePos = 0;
	}

	template <typename T>
		void append(T value)
	{
		append((uint8*)&value, sizeof(value));
	}

	template <typename T>
		void put(size_t pos, T value)
	{
		put(pos, (uint8*)&value, sizeof(value));
	}

	//////////////////////////////////////////////////////////////////////////
public:
	ByteBuffer& operator<<(bool value)
	{
		append<char>((char)value);
		return *this;
	}
	ByteBuffer& operator<<(uint8 value)
	{
		append<uint8>(value);
		return *this;
	}
	ByteBuffer& operator<<(uint16 value)
	{
		append<uint16>(value);
		return *this;
	}
	ByteBuffer& operator<<(uint32 value)
	{
		append<uint32>(value);
		return *this;
	}
	ByteBuffer& operator<<(uint64 value)
	{
		append<uint64>(value);
		return *this;
	}

	ByteBuffer& operator<<(int8 value)
	{
		append<int8>(value);
		return *this;
	}
	ByteBuffer& operator<<(int16 value)
	{
		append<int16>(value);
		return *this;
	}
	ByteBuffer& operator<<(int32 value)
	{
		append<int32>(value);
		return *this;
	}
	ByteBuffer& operator<<(int64 value)
	{
		append<int64>(value);
		return *this;
	}

	ByteBuffer& operator<<(float value)
	{
		append<float>(value);
		return *this;
	}
	ByteBuffer& operator<<(double value)
	{
		append<double>(value);
		return *this;
	}
	ByteBuffer& operator<<(time_t value)
	{
		append<time_t>(value);
		return *this;
	}

	ByteBuffer& operator<<(const std::string& value)
	{
		append((uint8 const *)value.c_str(), value.length());
		append((uint8)0);
		return *this;
	}
	ByteBuffer& operator<<(const char* str)
	{
		append( (uint8 const *)str, str ? strlen(str) : 0);
		append((uint8)0);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
public:
	ByteBuffer& operator>>(bool& value)
	{
		value = read<char>() > 0 ? true : false;
		return *this;
	}
	ByteBuffer& operator>>(uint8& value)
	{
		value = read<uint8>();
		return *this;
	}
	ByteBuffer& operator>>(uint16& value)
	{
		value = read<uint16>();
		return *this;
	}
	ByteBuffer& operator>>(uint32& value)
	{
		value = read<uint32>();
		return *this;
	}
	ByteBuffer& operator>>(uint64& value)
	{
		value = read<uint64>();
		return *this;
	}

	ByteBuffer& operator>>(int8& value)
	{
		value = read<int8>();
		return *this;
	}
	ByteBuffer& operator>>(int16& value)
	{
		value = read<int16>();
		return *this;
	}
	ByteBuffer& operator>>(int32& value)
	{
		value = read<int32>();
		return *this;
	}
	ByteBuffer& operator>>(int64& value)
	{
		value = read<int64>();
		return *this;
	}

	ByteBuffer& operator>>(float &value)
	{
		value = read<float>();
		return *this;
	}
	ByteBuffer& operator>>(double &value)
	{
		value = read<double>();
		return *this;
	}
	ByteBuffer& operator>>(time_t value)
	{
		value = read<time_t>();
		return *this;
	}

	ByteBuffer& operator>>(std::string& value)
	{
		value.clear();
		while (rpos() < size())
		{
			char c = read<char>();
			if (c == 0)
			{
				break;
			}
			value += c;
		}
		return *this;
	}

	ByteBuffer& operator>>(char value[])
	{
		std::string strValue;
		strValue.clear();
		while (rpos() < size())
		{
			char c = read<char>();
			if (c == 0)
			{
				break;
			}
			strValue += c;
		}
		strncpy(value, strValue.c_str(), strValue.size());
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
public:
	uint8 operator[](size_t pos)
	{
		return read<uint8>(pos);
	}

	size_t rpos() const
	{
		return mReadPos;
	};

	size_t rpos(size_t rpos_)
	{
		mReadPos = rpos_;
		return mReadPos;
	};

	size_t wpos() const
	{
		return mWritePos;
	}

	size_t wpos(size_t wpos_)
	{
		mWritePos = wpos_;
		return mWritePos;
	}

	template <typename T> T read()
	{
		T r = read<T>(mReadPos);
		mReadPos += sizeof(T);
		return r;
	};
	template <typename T> T read(size_t pos) const
	{
		assert(pos + sizeof(T) <= size() || PrintPosError(false,pos,sizeof(T)));
		return *((T const*)&mStorage[pos]);
	}

	void read(uint8 *dest, size_t len)
	{
		assert(mReadPos  + len  <= size() || PrintPosError(false, mReadPos,len));
		memcpy(dest, &mStorage[mReadPos], len);
		mReadPos += len;
	}

	const uint8* contents() const { return &mStorage[mReadPos]; }

	size_t size() const { return mStorage.size(); }

	bool empty() const { return mStorage.empty(); }

	void resize(size_t _NewSize)
	{
		mStorage.resize(_NewSize);
		mReadPos = 0;
		mWritePos = size();
	};

	void reserve(size_t _Size)
	{
		if (_Size > size()) mStorage.reserve(_Size);
	};

	void append(const std::string& str)
	{
		append((uint8 const*)str.c_str(), str.size() + 1);
	}
	void append(const char *src, size_t cnt)
	{
		return append((const uint8 *)src, cnt);
	}
	void append(const uint8 *src, size_t cnt)
	{
		if (!cnt) return;

		assert(size() < 10000000);

		if (mStorage.size() < mWritePos + cnt)
		{
			mStorage.resize(mWritePos + cnt);
		}
		memcpy(&mStorage[mWritePos], src, cnt);
		mWritePos += cnt;
	}
	void append(const ByteBuffer& buffer)
	{
		if (buffer.size()) append(buffer.contents(),buffer.size());
	}

	void put(size_t pos, const uint8 *src, size_t cnt)
	{
		assert(pos + cnt <= size() || PrintPosError(true,pos,cnt));
		memcpy(&mStorage[pos], src, cnt);
	}

	//////////////////////////////////////////////////////////////////////////
public:
	void print_storage()
	{
	}

	void textlike()
	{
	}

	void hexlike()
	{
	}

	bool PrintPosError(bool add, size_t pos, size_t esize) const
	{
		printf("ERROR: Attempt %s in ByteBuffer (pos: %u size: %u) value with size: %u",(add ? "put" : "get"), pos, size(), esize);
		return false;
	}

protected:
	size_t				mReadPos;
	size_t				mWritePos;
	std::vector<uint8>	mStorage;
};


//////////////////////////////////////////////////////////////////////////
// std::vector
//////////////////////////////////////////////////////////////////////////
#ifdef _VECTOR_
template <typename T>
ByteBuffer& operator<<(ByteBuffer& b, const std::vector<T>& v)
{
	b << (uint32)v.size();

	typename std::vector<T>::const_iterator iter	= v.begin();
	typename std::vector<T>::const_iterator& iEnd	= v.end();
	for (; iter != iEnd; ++iter)
	{
		b << *iter;
	}
	return b;
}

template <typename T>
ByteBuffer& operator>>(ByteBuffer& b, std::vector<T>& v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while (vsize--)
	{
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}
#endif

//////////////////////////////////////////////////////////////////////////
// std::list
//////////////////////////////////////////////////////////////////////////
#ifdef _LIST_
template <typename T>
ByteBuffer& operator<<(ByteBuffer& b, const std::list<T>& v)
{
	b << (uint32)v.size();

	typename std::list<T>::const_iterator iter	= v.begin();
	typename std::list<T>::const_iterator& iEnd	= v.end();
	for (; iter != iEnd; ++iter)
	{
		b << *iter;
	}
	return b;
}

template <typename T>
ByteBuffer& operator>>(ByteBuffer& b, std::list<T>& v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while (vsize--)
	{
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}
#endif

//////////////////////////////////////////////////////////////////////////
// std::map
//////////////////////////////////////////////////////////////////////////
#ifdef _MAP_
template <typename K, typename V>
ByteBuffer& operator<<(ByteBuffer& b, const std::map<K, V>& m)
{
	b << (uint32)m.size();

	typename std::map<K, V>::const_iterator iter = m.begin();
	typename std::map<K, V>::const_iterator iEnd = m.end();
	for (; iter != iEnd; ++iter)
	{
		b << iter->first << iter->second;
	}
	return b;
}

template <typename K, typename V>
ByteBuffer &operator>>(ByteBuffer& b, std::map<K, V>& m)
{
	uint32 msize;
	b >> msize;
	m.clear();
	while (msize--)
	{
		K k;
		V v;
		b >> k >> v;
		m.insert(std::make_pair(k, v));
	}
	return b;
}
#endif


//////////////////////////////////////////////////////////////////////////
// 序列化需要的宏
// @note 如果是struct里面都是POD数据，即都是原生数据，那么可以直接进行拷贝，或者完全不进行序列化。
//		序列化主要是为了用于非定长数据而存在的，相对于POD数据可以直接拷贝，非定长数据的序列化当然是会有些许的性能损失，
//		但是可以保证数据的相对安全性。
//		写操作使用ByteBuffer::<< 或者 ByteBuffer::append进行操作；
//		读操作使用ByteBuffer::>> 或者 ByteBuffer::read进行操作；
//		需要注意的是，读写操作的操作符必须是对应的，比如：ByteBuffer::<<和ByteBuffer::>>是相对应的。
// @warning 对于数组，最好是直接使用ByteBuffer::append 和ByteBuffer::read进行读写操作，否则会产生严重错误！
//////////////////////////////////////////////////////////////////////////
// 声明序列化
#define NET_APPEND(STRUCT_TYPE)\
	static ByteBuffer& operator<<(ByteBuffer& lht, const STRUCT_TYPE& rht)

// 声明解序列化
#define NET_READ(STRUCT_TYPE)\
	static ByteBuffer& operator>>(ByteBuffer& lht, STRUCT_TYPE& rht)
//////////////////////////////////////////////////////////////////////////


#endif
