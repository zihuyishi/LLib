// LLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Library\LString.h"
#include "Library\LPtr.h"
#include <iostream>
#include <memory>
#include <assert.h>
using namespace LLib;
using std::wstring;
using std::shared_ptr;

WLString test(WLString str) {
	return str;
}
extern void Test1();
int _tmain(int argc, _TCHAR* argv[])
{
	
	wchar_t wstr1[] = L"abcd";
	wchar_t wstr2[] = L"efgh";
	wchar_t wstr3[] = L"abcdefgh";
	WLString str1(wstr1);
	WLString str2(wstr2);
	str2 = str1 + str2;
	WLString str3(wstr1);
	str3 += str2;
	const wchar_t *p = str3;
	assert(!wcscmp(str1, wstr1));
	assert(!wcscmp(str2, wstr3));
	assert(!wcscmp(str3, L"abcdabcdefgh"));
	std::make_shared<int>();
	Test1();
	
	return 0;
}

void Test1() {
	wchar_t wstr1[] = L"abcd";
	wchar_t wstr2[] = L"efgh";
	wchar_t wstr3[] = L"abcdefgh";
	LString str1(wstr1);
	LString str2(wstr2);
	str2 = str1 + str2;
	LString str3(wstr1);
	str3 += str2;
	const wchar_t *p = str3;
	wchar_t wchar1 = str1[1];
	wchar_t wchar2 = str3[4];
	for (size_t i = 0; i < wcslen(wstr1); ++i) {
		wchar1 = str1[i];
		assert(wchar1 == wstr1[i]);
	}
	assert(wchar2 == L'a');
	assert(!wcscmp(str1, wstr1));
	assert(!wcscmp(str2, wstr3));
	assert(!wcscmp(str3, L"abcdabcdefgh"));
	assert(!wcscmp(p, str3));
}
void Test2() {
	char wstr1[] = "abcd";
	char wstr2[] = "efgh";
	char wstr3[] = "abcdefgh";
	LStringA str1(wstr1);
	assert(!strcmp(str1, wstr1));
	LStringA str2(wstr2);
	assert(!strcmp(str2, wstr2));
	str2 = str1 + str2;
	LStringA str3(wstr3);
	assert(!strcmp(str2, str3));
	char char1;
	for (size_t i = 0; i < strlen(str1); i++) {
		char1 = str1[i];
		assert(char1 == wstr1[i]);
	}
	str3 += str1;
	const char *p = str3;
	assert(!strcmp(str3, "abcdefghabcd"));
	assert(!strcmp(str3, p));
}
