#ifndef __TCPSERVERMGR_H__
#define __TCPSERVERMGR_H__

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Mutex.h"
#include "ace/Singleton.h"
#include "clienthandler.h"

typedef ACE_Acceptor<ClientHandler, ACE_SOCK_ACCEPTOR> SOCKAcceptor;

class TcpServerMgr
{
public:

	// 加载配置文件
	void		init();

	// 启动服务监听
	bool		start();

	// 关闭服务监听
	void		stop();

private:
	int				m_port;
	SOCKAcceptor	m_connector;
};
typedef ACE_Singleton<TcpServerMgr, ACE_Recursive_Thread_Mutex>  App_TcpServer;
#endif