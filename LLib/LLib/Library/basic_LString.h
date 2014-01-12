#include <string.h>
#ifndef _LLIB_BASIC_LSTRING_
#define _LLIB_BASIC_LSTRING_
namespace LLib {
	template<typename T>
	class basic_LString{
		friend class WLString;
		template <typename T>
		friend class LStringT;
	public:
		basic_LString() :_str(nullptr), _len(0)
		{}
		~basic_LString() {
			if (_str != nullptr)
				delete[] _str;
		}
		basic_LString(const T *src) {
			_len = tstrlen(src);
			_str = new T[_len + 1];
			memcpy_s(_str, _len*sizeof(T), src, _len*sizeof(T));
			_str[_len] = 0;
		}
		basic_LString(const T *lhs, const T* rhs) {
			size_t len1 = tstrlen(lhs);
			size_t len2 = tstrlen(rhs);
			_len = len1 + len2;
			_str = new T[_len + 1];
			memcpy_s(_str, len1*sizeof(T), lhs, len1*sizeof(T));
			memcpy_s(_str + len1, len2*sizeof(T), rhs, len2*sizeof(T));
			_str[_len] = 0;
		}
	public:
		basic_LString& operator=(const basic_LString& src) {
			if (_str != nullptr)
				delete[] _str;
			_len = src._len;
			_str = new T[_len + 1];
			memcpy_s(_str, _len*sizeof(T), src._str, _len*sizeof(T));
			_str[_len] = 0;
			return *this;
		}
		basic_LString(const basic_LString& src) :_str(nullptr), _len(0)
		{
			*this = src;
		}
		size_t Length() const { return _len; }
		operator const T*() const { return _str; }
	protected:
		//size_t _count;
		size_t _len;
		T *_str;
	private:
		size_t tstrlen(const T *src) const {
			size_t len = 0;
			while (*src != 0) {
				len++;
				src++;
			}
			return len;
		}
	};
}

#endif