/*
* 用于发送调试信息到邮件
* 定义SEND_DEBUG_OPEN用于打开模块功能
* CAT_STRING_INFO(x)用于监视变量x，可以为字符数组
* SEND_DEBUG_INFO() 调用后将发送邮件，为异步发送
* WAIT_DEBUG_INFO() 用于等待邮件发送成功
*/

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
	const char* toAddr, const char* subject, const char* data, HANDLE finishEvent = NULL);

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
	bool WaitSendEnd(int iTime = 10000);
	bool WriteToFile(const char* filePath = NULL);
public:
	CInfoSender& SetUserLock();
	CInfoSender& ReleaseUserLock();
public:
	void SetSubject(const char* subject)
	{
		m_subject = subject;
	}
	void SetAddr(const char* addr)
	{
		m_ToAddr = addr;
	}
	void SetStmpServer(const char* stmp)
	{
		m_stmpServer = stmp;
	}
private:
	bool bufferLockedAppend(const char* str);
	bool bufferLockedClear();
private:
	std::string			m_buffer;
	std::string			m_username;
	std::string			m_password;
	std::string			m_ToAddr;
	std::string			m_stmpServer;
	std::string			m_subject;
	
	int					m_fileCount;
	HANDLE				m_hEvent;
	HANDLE				m_hBufferMutex;
	HANDLE				m_hUserMutex;
};
//全局的一个实例
extern CInfoSender*		g_InfoSender;

#ifdef SEND_DEBUG_OPEN

inline void SEND_DEBUG_INFO() {
	g_InfoSender->ASyncSend();
}

inline void WAIT_DEBUG_INFO() {
	g_InfoSender->WaitSendEnd();
}

#define ADD_DEBUG_INFO(x) g_InfoSender->AddInfo(x)

#define CAT_STRING_INFO(x) do { \
	g_InfoSender->SetUserLock(); \
	g_InfoSender->Cat("\r\nIn file : ").Cat(__FILE__).Cat("\r\nFunction : ").Cat(__FUNCTION__); \
	g_InfoSender->Cat("\r\nLine : ").Cat(__LINE__).Cat("\r\nExpr: ").Cat(#x).Cat(" = ").Cat(x).Cat("\r\n"); \
	g_InfoSender->ReleaseUserLock(); \
} while(0)

#define CAT_ENTER_FUNC() do { \
	g_InfoSender->SetUserLock(); \
	g_InfoSender->Cat("\r\nEnter Function : ").Cat(__FUNCTION__).Cat("\r\n"); \
	g_InfoSender->ReleaseUserLock(); \
} while(0)

#define CAT_EXIT_FUNC() do { \
	g_InfoSender->SetUserLock(); \
	g_InfoSender->Cat("\r\nExit Function : ").Cat(__FUNCTION__).Cat("\r\n"); \
	g_InfoSender->ReleaseUserLock(); \
} while(0)

inline void WRITE_DEBUG_INFO() {
	g_InfoSender->WriteToFile();
}

#else

inline void SEND_DEBUG_INFO() {}

inline void WAIT_DEBUG_INFO() {}

#define ADD_DEBUG_INFO(x)

#define CAT_STRING_INFO(str, len, x)

#define CAT_ENTER_FUNC()

#define CAT_EXIT_FUNC()

inline void WRITE_DEBUG_INFO() {}

#endif //SEND_DEBUG_OPEN
#endif
