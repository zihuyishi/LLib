// DebugAndSendMail.cpp : 定义控制台应用程序的入口点。
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
	CAT_STRING_INFO("这就是传说中的第二行");
	SEND_DEBUG_INFO();

	ADD_DEBUG_INFO("第二封邮件");
	CAT_STRING_INFO("This is the first Line");
	CAT_STRING_INFO("这次又是第二行");
	SEND_DEBUG_INFO();
	std::cout << "发送完毕" << std::endl;
	int i;
	std::cin >> i;
	return 0;
}
