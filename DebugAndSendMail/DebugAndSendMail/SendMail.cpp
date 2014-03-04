#include "stdafx.h"

#include "SendMail.h"

#include <Windows.h>
#include <string>
#include <assert.h>
#include <fstream>
CSMTPSendMail::CSMTPSendMail() :
m_socket(INVALID_SOCKET),
m_delay(10000)
{
	strcpy_s(m_port, 5, "25");
}
CSMTPSendMail::~CSMTPSendMail()
{

}

int CSMTPSendMail::Base64Encode(const char* src, char* dst)
{
	int i, j, srcLen;
	srcLen = strlen(src);
	int dstLen = srcLen / 3 * 4;
	if (dst == NULL) {
		return dstLen;
	}
	for (i = 0, j = 0; i <= srcLen - 3; i += 3, j += 4)
	{
		dst[j] = (src[i] & 0xFC) >> 2;
		dst[j + 1] = ((src[i] & 0x03) << 4) + ((src[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((src[i + 1] & 0x0F) << 2) + ((src[i + 2] & 0xC0) >> 6);
		dst[j + 3] = src[i + 2] & 0x3F;
	}
	if (srcLen % 3 == 1) {
		dst[j] = (src[i] & 0xFC) >> 2;
		dst[j + 1] = ((src[i] & 0x03) << 4);
		dst[j + 2] = 64;
		dst[j + 3] = 64;
		j += 4;
	}
	else if (srcLen % 3 == 2) {
		dst[j] = (src[i] & 0xFC) >> 2;
		dst[j + 1] = ((src[i] & 0x03) << 4) + ((src[i + 1] & 0xF0) >> 4);
		dst[j + 2] = ((src[i + 1] & 0x0F) << 2);
		dst[j + 3] = 64;
		j += 4;
	}
	static unsigned char *base64 = (unsigned char*)("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=");
	for (i = 0; i < j; ++i) {
		dst[i] = base64[(int)dst[i]];
	}
	dst[j] = '\0';
	return dstLen;
}

bool CSMTPSendMail::SendEmail(const char* smtpServer, const char* username, const char* password,
	const char* toAddr, const char* subject, const char* data, bool bBase64)
{
	bool bRet = true;
	bRet = InitSocket(smtpServer, "25");
	if (!bRet) {
		return bRet;
	}
	const UINT BufferLen = strlen(data) + 1024;
	char *recvBuffer = new char[BufferLen];
	char *sendBuffer = new char[BufferLen];
	do {
		Recv(m_socket, recvBuffer, BufferLen);
		StringCchPrintfA(sendBuffer, BufferLen, 
			"HELO %s\r\n", smtpServer);
		Send(m_socket, sendBuffer);
		Recv(m_socket, recvBuffer, BufferLen);

		Send(m_socket, ("auth login\r\n"));
		Recv(m_socket, recvBuffer, BufferLen);

		if (!bBase64) {
			Base64Encode(username, sendBuffer);
		}
		else {
			strcpy_s(sendBuffer, BufferLen, username);
		}
		strcat_s(sendBuffer, BufferLen, "\r\n");
		Send(m_socket, sendBuffer);

		Recv(m_socket, recvBuffer, BufferLen);
		if (std::string(recvBuffer).substr(0, 3) != "334") {
			bRet = false;
			break;
		}

		if (!bBase64) {
			Base64Encode(password, sendBuffer);
		}
		else {
			strcpy_s(sendBuffer, BufferLen, password);
		}
		strcat_s(sendBuffer, BufferLen, "\r\n");
		Send(m_socket, sendBuffer);

		Recv(m_socket, recvBuffer, BufferLen);
		if (std::string(recvBuffer).substr(0, 3) != "235")
		{
			bRet = false;
			break;
		}
		StringCchPrintfA(sendBuffer, BufferLen, 
			"mail from:<%s>\r\n", username);
		Send(m_socket, sendBuffer);
		Recv(m_socket, recvBuffer, BufferLen);

		StringCchPrintfA(sendBuffer, BufferLen, "%s%s%s",
			"rcpt to:<", toAddr, ">\r\n");
		Send(m_socket, sendBuffer);
		Recv(m_socket, recvBuffer, BufferLen);

		Send(m_socket, "data\r\n");
		Recv(m_socket, recvBuffer, BufferLen);
		StringCchPrintfA(sendBuffer, BufferLen, 
			"to:%s\r\nsubject:%s\r\n\r\n%s\r\n.\r\n",
			toAddr, subject, data);
		Send(m_socket, sendBuffer);
		Recv(m_socket, recvBuffer, BufferLen);

		Send(m_socket, "quit\r\n");
		Recv(m_socket, recvBuffer, BufferLen);
	} while (0);

	delete[] sendBuffer;
	delete[] recvBuffer;
	CleanSocket();
	return bRet;
}
void CSMTPSendMail::Send(SOCKET& s, const char* data)
{
	send(s, data, strlen(data), 0);
}
void CSMTPSendMail::Recv(SOCKET& s, char* buffer, int len)
{
	memset(buffer, 0, len);
	recv(s, buffer, len, 0);
}
bool CSMTPSendMail::InitSocket(const char* Server, const char* Port)
{
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		return false;
	}
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo(Server, m_port, &hints, &result);
	if (iResult != 0) {
		WSACleanup();
		return false;
	}
	m_socket = INVALID_SOCKET;
	ptr = result;
	m_socket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	iResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&m_delay, sizeof(int));
	return true;
}
void CSMTPSendMail::CleanSocket()
{
	if (m_socket != INVALID_SOCKET) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	WSACleanup();
}

bool SyncSendMail(const char* smtpServer, const char* username, const char* password,
	const char* toAddr, const char* subject, const char* data)
{
	CSMTPSendMail mail;
	return mail.SendEmail(smtpServer, username, password, toAddr, subject, data);
}

using std::string;
typedef struct tagMailInfo{
	string smtpServer;
	string username;
	string password;
	string toAddr;
	string subject;
	string data;
}MailInfo, *PMailInfo;
DWORD WINAPI SendMailThread(LPVOID lpParam)
{
	PMailInfo mailInfo = (PMailInfo)lpParam;
	if (mailInfo == NULL) {
		return 1;
	}
	CSMTPSendMail mail;
	bool bRet = mail.SendEmail(mailInfo->smtpServer.c_str(),
		mailInfo->username.c_str(),
		mailInfo->password.c_str(),
		mailInfo->toAddr.c_str(),
		mailInfo->subject.c_str(),
		mailInfo->data.c_str(),
		false
		);
	delete mailInfo;
	if (bRet) {
		return 0;
	}
	else {
		return 1;
	}
}
bool AsyncSendMail(const char* smtpServer, const char* username, const char* password,
	const char* toAddr, const char* subject, const char* data)
{
	PMailInfo mailInfo		= new MailInfo;
	mailInfo->data			= data;
	mailInfo->password		= password;
	mailInfo->smtpServer		= smtpServer;
	mailInfo->subject		= subject;
	mailInfo->toAddr			= toAddr;
	mailInfo->username		= username;

	HANDLE hThread;
	DWORD dwThread;
	hThread = ::CreateThread(
		NULL,
		0,
		SendMailThread,
		(void*)mailInfo,
		0,
		&dwThread);
	if (hThread == NULL) {
		return false;
	}
	CloseHandle(hThread);
	return true;
}

CInfoSender::CInfoSender() :
CInfoSenderA(*this), 
CInfoSenderB(*this),
m_username("bicengdebug@163.com"),
m_password("051236805830KSBR"),
m_stmpServer("smtp.163.com"),
m_ToAddr("bicengdebug@163.com"),
m_subject("百润百成测试信息邮件"),
m_fileCount(0)
{
}
CInfoSender::~CInfoSender()
{
}
CInfoSender& CInfoSender::AddInfo(const char* info)
{
	assert(info != 0);
	m_buffer.append("\r\n");
	m_buffer.append(info);
	m_buffer.append("\r\n");
	return *this;
}
CInfoSender& CInfoSender::AddInfo(const wchar_t* info)
{
	assert(info != 0);
	int nLen = WideCharToMultiByte(CP_OEMCP, 0, 
		info, -1, NULL, 0, NULL, FALSE);
	char* lpTmp = new char[nLen + 1];
	WideCharToMultiByte(CP_OEMCP, NULL,
		info, -1, lpTmp, nLen, NULL, FALSE);
	AddInfo(lpTmp);
	delete[] lpTmp;
	return *this;
}
CInfoSender& CInfoSender::AddInfo(int info)
{
	char* lpTmp = new char[1024];
	lpTmp[0] = 0;
	_itoa_s(info, lpTmp, 1024, 10);
	AddInfo(lpTmp);
	delete[] lpTmp;
	return *this;
}

CInfoSender& CInfoSender::Cat(long lValue)
{
	char lBuffer[40];
	_ltoa_s(lValue, lBuffer, 40, 10);
	m_buffer.append(lBuffer);
	return *this;
}
CInfoSender& CInfoSender::Cat(const char* strValue)
{
	assert(strValue != 0);
	m_buffer.append(strValue);
	return *this;
}
CInfoSender& CInfoSender::Cat(const wchar_t* wcsValue)
{
	assert(wcsValue != 0);
	int nLen = WideCharToMultiByte(CP_OEMCP, 0,
		wcsValue, -1, NULL, 0, NULL, FALSE);
	char* lpTmp = new char[nLen + 1];
	WideCharToMultiByte(CP_OEMCP, NULL,
		wcsValue, -1, lpTmp, nLen, NULL, FALSE);
	m_buffer.append(lpTmp);
	delete[] lpTmp;
	return *this;
}

bool CInfoSender::SyncSend()
{
	bool bRet = SyncSendMail(m_stmpServer.c_str(),
		m_username.c_str(),
		m_password.c_str(),
		m_ToAddr.c_str(),
		m_subject.c_str(),
		m_buffer.c_str()
		);
	m_buffer.clear();
	return bRet;
}
bool CInfoSender::ASyncSend()
{
	bool bRet = AsyncSendMail(m_stmpServer.c_str(),
		m_username.c_str(),
		m_password.c_str(),
		m_ToAddr.c_str(),
		m_subject.c_str(),
		m_buffer.c_str()
		);
	m_buffer.clear();
	return bRet;
}
bool CInfoSender::WriteToFile(const char* filePath)
{
	std::string filename;
	if (filePath) {
		filename.append(filePath);
	}
	else {
		filename.append("C:\\bicengdebug");
		char buffer[100];
		_itoa_s(m_fileCount, buffer, 100, 10);
		filename.append(buffer);
		filename.append(".txt");
	}
	std::ofstream outfile(filename.c_str(), std::ofstream::out);
	outfile.write(m_buffer.c_str(), m_buffer.length());
	outfile.close();
	m_buffer.clear();
	++m_fileCount;
	return true;
}

CInfoSender* g_InfoSender = new CInfoSender();