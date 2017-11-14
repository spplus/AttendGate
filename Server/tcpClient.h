#ifndef __MYTCPCLIENT__H__
#define __MYTCPCLIENT__H__

#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "scadahandler.h"

typedef ACE_Connector<ScadaHandler, ACE_SOCK_CONNECTOR> SOCKConnector;

class TcpClient
{
public:

	void	close();
	int		send(ACE_Message_Block* mb);
	bool	connect();
	bool	connect(ACE_INET_Addr svrAddr);
	void	setRecvTask(RecvTask* task);
	bool	isConnected();
private:

	ACE_INET_Addr	m_srvaddr;
	ScadaHandler	m_handler;
	SOCKConnector	m_connector;
};

#endif