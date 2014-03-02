#ifndef LL_SENDMAILLIB_HEADER
#define LL_SENDMAILLIB_HEADER

#include <iostream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#pragma comment(lib, "Ws2_32.lib")

#include <strsafe.h>

class CSMTPSendMail
{
public:
	CSMTPSendMail();
	~CSMTPSendMail();
public:
	bool SendEmail(const char* smtpServer, const char* username, const char* password,
		const char* toAddr, const char* subject, const char* data, bool bBase64 = false);
public:
	void SetDelay(int delay) {
		m_delay = delay;
	}
private:
	bool InitSocket(const char* Server, const char* Port);
	void CleanSocket();
	void Send(SOCKET& s, const char* data);
	void Recv(SOCKET& s, char* buffer, int len);
	int Base64Encode(const char* src, char* des);
	
private:
	char m_port[5];
	SOCKET m_socket;
	int	m_delay;
};

//同步的发邮件
bool SyncSendMail(const char* smtpServer, const char* username, const char* password,
	const char* toAddr, const char* subject, const char* data);
//异步发邮件
bool AsyncSendMail(const char* smtpServer, const char* username, const char* password,
	const char* toAddr, const char* subject, const char* data);

class CInfoSender
{
public:
	CInfoSender();
	~CInfoSender();

	CInfoSender& CInfoSenderA;
	CInfoSender& CInfoSenderB;
public:
	CInfoSender& AddInfo(const char* info);
	CInfoSender& AddInfo(const wchar_t* info);
	CInfoSender& AddInfo(int info);
public:
	CInfoSender& Cat(long lValue);
	CInfoSender& Cat(const char* strValue);
	CInfoSender& Cat(const wchar_t* wcsValue);
	CInfoSender& CatEnd();
public:
	bool SyncSend();
	bool ASyncSend();
public:
	void SetSubject(const char* subject)
	{
		m_subject = subject;
	}

private:
	std::string m_buffer;
	std::string m_username;
	std::string m_password;
	std::string m_ToAddr;
	std::string m_stmpServer;
	std::string m_subject;
};
extern CInfoSender* g_InfoSender;

#ifdef SEND_DEBUG_OPEN

#define SEND_DEBUG_INFO() g_InfoSender->ASyncSend()

#define ADD_DEBUG_INFO(x) g_InfoSender->AddInfo(x)

#define CAT_STRING_INFO(x) do { \
	g_InfoSender->Cat("\r\nIn file : ").Cat(__FILE__).Cat("\r\nLine : ").Cat(__LINE__); \
	g_InfoSender->Cat("\r\nExpr: ").Cat(#x).Cat(" = ").Cat(x).Cat("\r\n"); \
	} while(0)

#else

#define SEND_DEBUG_INFO()

#define ADD_DEBUG_INFO(x)

#define CAT_STRING_INFO(str, len, x)



#endif //SEND_DEBUG_OPEN
#endif
