// DebugAndSendMail.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#define SEND_DEBUG_OPEN
#include "SendMail.h"
#include <iostream>
int _tmain(int argc, _TCHAR* argv[])
{
	bool bRet = false;

	int iValue;
	std::cin >> iValue;
	CAT_STRING_INFO(iValue);
	CAT_STRING_INFO("This is the first Line");
	CAT_STRING_INFO("����Ǵ�˵�еĵڶ���");
	SEND_DEBUG_INFO();

	ADD_DEBUG_INFO("�ڶ����ʼ�");
	CAT_STRING_INFO("This is the first Line");
	CAT_STRING_INFO("������ǵڶ���");
	SEND_DEBUG_INFO();
	std::cout << "�������" << std::endl;
	int i;
	std::cin >> i;
	return 0;
}
