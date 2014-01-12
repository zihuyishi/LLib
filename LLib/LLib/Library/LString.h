
#include "basic_LString.h"
#include "LPtr.h"
#ifndef _LLIB_LSTRING_
#define _LLIB_LSTRING_

#ifndef _UNICODE
#define LString LStringA
#else
#define LString LStringW
#endif

namespace LLib {
	class WLString {
	private:
		typedef basic_LString<wchar_t> bstring;
		typedef LPtr<basic_LString<wchar_t>> point;
	public:
		WLString() :_str(0)
		{}
		WLString(wchar_t *src)
		{
			_str = make_LPtr(bstring(src));
		}
		WLString(const WLString &src) {
			_str = src._str;
		}
		WLString& operator=(const WLString &src) {
			_str = src._str;
			return *this;
		}
	public:
		size_t Length() const {
			return _str->_len;
		}
		operator const wchar_t*() const{
			return _str->_str;
		}
		wchar_t operator[](int index) const {
			return _str->_str[index];
		}
		WLString operator+(const WLString &right) {
			size_t newlen = right.Length() + this->Length();
			wchar_t *temp = new wchar_t[newlen + 1];
			wcscpy_s(temp, newlen + 1, *this);
			wcscpy_s(temp + Length(), newlen + 1 - Length(), right);
			WLString ret(temp);
			return ret;
		}
		WLString& operator+=(const WLString &right) {
			*this = *this + right;
			return *this;
		}
	private:
		point _str;
	};
	template<typename Tchar>
	class LStringT {
	private:
		typedef basic_LString<Tchar> bstring;
		typedef LPtr<bstring> point;
		typedef LStringT<Tchar> tstring;
	public:
		LStringT() : _str(new point())
		{}
		LStringT(Tchar *str) {
			_str = make_LPtr(bstring(str));
		}
		LStringT(const tstring &str) {
			_str = str._str;
		}
		tstring& operator=(const tstring &str) {
			_str = str._str;
			return *this;
		}
	public:
		size_t Length() const { return _str->Length(); }
		
		operator const Tchar*() const { return *(_str.Get()); }
		Tchar operator[](int index) const { return _str->_str[index]; }
		tstring operator+(const tstring &rhs) {
			tstring tempstr(*this);
			tempstr += rhs;
			return tempstr;
		}
		tstring& operator+=(const tstring &rhs) {
			const Tchar *str1 = *(_str.Get());
			const Tchar *str2 = *(rhs._str.Get());
			_str = make_LPtr( bstring(str1, str2) );
			return *this;
		}

	private:

	private:
		point _str;
	};
	typedef LStringT<wchar_t> LStringW;
	typedef LStringT<char> LStringA;
}
#endif