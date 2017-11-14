
#include "recvTask.h"
#include "tcpClient.h"

bool TcpClient::connect(ACE_INET_Addr svrAddr)
{
	m_srvaddr = svrAddr;
	return connect();
}

bool TcpClient::connect()
{
	ScadaHandler* pc = &m_handler;
	return m_connector.connect(pc,m_srvaddr) == -1?false:true;
}

void TcpClient::close()
{
	m_handler.close();
}

int TcpClient::send(ACE_Message_Block* mb)
{
	return m_handler.sendData(mb);
}

void TcpClient::setRecvTask(RecvTask* task)
{
	m_handler.setRecvTask(task);
}

bool TcpClient::isConnected()
{
	return m_handler.isConnected();
}