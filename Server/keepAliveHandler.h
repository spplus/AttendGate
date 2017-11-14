#ifndef __KEEPALIVEHANDLER_H__
#define __KEEPALIVEHANDLER_H__

#include "ace/Event_Handler.h"
#include "tcpClient.h"

class KeepAliveHandler : public ACE_Event_Handler
{
public:

	void	setClient(TcpClient *client);
	int		handle_timeout (const ACE_Time_Value &current_time,const void * = 0);

private:
	TcpClient*	m_client;
};

#endif