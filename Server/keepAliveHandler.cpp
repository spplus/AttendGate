#include "keepAliveHandler.h"
#include "defines.h"


void KeepAliveHandler::setClient(TcpClient *client)
{
	m_client = client;
}

int KeepAliveHandler::handle_timeout(const ACE_Time_Value &current_time,const void * /* = 0 */)
{
	LOG->debug("begin reconnect to server");
	
	if (!m_client->isConnected())
	{
		m_client->connect();
	}

	return 0;
}