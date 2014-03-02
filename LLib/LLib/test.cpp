// LLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Library\LString.h"
#include "Library\LPtr.h"
#include "LVector.h"
#include <iostream>
#include <memory>
#include <assert.h>
#include <vector>
using namespace LLib;
using std::wstring;
using std::shared_ptr;
using std::vector;
WLString test(WLString str) {
	return str;
}
extern void Test1();
extern void TestLVector();
extern void TestLVector2();
int _tmain(int argc, _TCHAR* argv[])
{
	

	TestLVector2();
	
	return 0;
}
void TestLVector()
{
	LVector<int> list;
	list.push_back(2);
	list.push_back(4);
	list.push_back(5);
	LVector<int> list2(list);
	assert(list[0] == 2);
	assert(list[2] == list2[2]);
	list2[1] = 10;
	assert(list[1] == list2[1]);
	LVector<int> list3;
	list3 = list2.CopyTo();
	assert(list3[0] == list[0]);
	for (int i = 0; i < 100; ++i)
	{
		list2.push_back(i);
	}
	list3 = list2;
	for (int i = 0; i < 100; ++i)
	{
		assert(list3[i] == i);
	}
}
void TestLVector2()
{
	LVector<LString> list;
	list.push_back(LString(_T("abcde")));
	list.push_back(LString(_T("fgh")));
	list.push_back(LString(_T("abcdefgh")));
	//assert(list[0] + list[1] == list[2]);
	LVector<LString> list2(list);
	std::vector<LString> list3;
	list3.push_back(_T("abcde"));
	list3.push_back(_T("fgh"));
	list3.push_back(_T("abcdefgh"));
	LVector<std::wstring> list4;
	list4.push_back(L"abcde");
	list4.push_back(L"fgh");
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
