#include "ZHString.h"
#include <tchar.h>
#include <CString>

namespace ZHStringLib
{
	namespace
	{
		template <typename T>
		void swap(T &lhs, T &rhs)
		{
			T tmp(lhs);
			lhs = rhs;
			rhs = tmp;
		}
		const wchar_t wchartable[] = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	}
    int ahtoi(char *s) {
        if ( ('0' == s[0]) && ('x' == s[1] || 'X' == s[1]) ) {
            s += 2;
        }
        
        do {
            

		} while (*++s != '\0');
		return 0;
    }
    int whtoi(wchar_t *s);

	wchar_t* getlineW(wchar_t *src, wchar_t *des)
	{
		return getstringW(src, des, L'\n');
	}
	wchar_t* getstringW(wchar_t *src, wchar_t *des, const wchar_t div)
	{
		if (L'\0' == src[0])
			return 0;
		unsigned int i = 0;
		while (src[i] != L'\0' && src[i] != div) {
			des[i] = src[i];
			++i;
		}
		des[i] = L'\0';
		if (src[i] == div)
			++i;
		return src + i;
	}
	int wcs2int(const wchar_t *src)
	{
		int len = wcslen(src);
		int i = 0,
			ans = 0;
		if (src[0] == L'-')
			++i;
		for (;i<len; ++i) {
			ans *= 10;
			ans += (src[i] - L'0');
		}
		if (src[0] == L'-')
			ans = -ans;
		return ans;
	}
	int squeezeW(wchar_t *s, wchar_t c)
	{
		int i, j, count;
		i = j = count = 0;

		for (i = j = 0; s[i] != L'\0'; ++i) {
			if (s[i] != c) {
				s[j++] = s[i];
			} else {
				++count;
			}
		}
		s[j] = L'\0';
		return count;
	}
	int squeezeW(wchar_t *src, wchar_t *des)
	{
		int count = 0;
		for (int i = 0; des[i] != L'\0'; ++i) {
			count += squeezeW(src, des[i]);
		}
		return count;
	}
	void reverseStrW(wchar_t *src)
	{
		unsigned int srcLen = wcslen(src);
		for (unsigned int i = 0;i < srcLen / 2;++i) {
			swap(src[i], src[srcLen - i - 1]);
		}
		return ;
	}
	int int2wcs(int inputInt, wchar_t* wcsRet,int Bit)
	{
		unsigned int wcsLen;
		bool lessZero = false;
		wcsLen = 0;
		if (inputInt < 0) {
			lessZero = true;
			inputInt = -inputInt;
		}
		do {
			wcsRet[wcsLen++] = wchartable[inputInt % Bit];
		} while (inputInt /= Bit);
		if (lessZero)
			wcsRet[wcsLen++] = L'-';
		wcsRet[wcsLen] = L'\0';
		reverseStrW(wcsRet);
		return wcsLen;
	}
	int expandW(const wchar_t* src, wchar_t* des)
	{
		unsigned int iLength, iPosition;
		iLength = iPosition = 0;
		wchar_t Wchar;
		while (src[iPosition] != L'\0') {
			if (L'-' == src[iPosition]) {
				des[iLength++] = src[iPosition++];
				continue;
			}
			
			Wchar = des[iLength++] = src[iPosition++];
			if (src[iPosition] != L'-') {
				++iPosition;
				continue;
			}
			++iPosition;
			while (src[iPosition] != Wchar) {
				des[iLength++] = ++Wchar;
			}
			++iPosition;
		}
		des[iLength] = L'\0';
		return iLength;
	}
	int trimW(wchar_t *lpRet)
	{
		int Length;
		for (Length = wcslen(lpRet) - 1; Length >= 0; --Length) {
			if (lpRet[Length] != L' ' && lpRet[Length] != L'\t'
				&& lpRet[Length] != L'\n' && lpRet[Length] != L'\r')
			{
				break;
			}
		}
		lpRet[++Length] = '\0';
		return Length;
	}

	int IsWhatW(const wchar_t* InputStr,const wchar_t* UnionsStr)
	{
		short bUnion[256];
		memset((void*) bUnion, 0, sizeof(bUnion));
		for(int i = 0;UnionsStr[i] != L'\0'; ++i) {
			bUnion[UnionsStr[i]] = 1;
		}
		for(int i = 0;InputStr[i] != L'\0'; ++i) {
			if (!bUnion[InputStr[i]]) {
				return 0;
			}
		}
		return 1;
	}
}