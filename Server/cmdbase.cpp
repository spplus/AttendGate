#include "cmdbase.h"
#include "defines.h"

int CmdBase::connectNet(long mechineId,const char* ipaddr,int port)
{
	// 连接
	m_connectId = FK_ConnectNet(mechineId, (char*)ipaddr, port, NTIMEOUT, PROTOCOL_TCPIP, NETPASSWORD, LICENSE);
	if (m_connectId>0)
	{
		LOG->message("连接成功:%s:%d.",ipaddr,port);
	}
	else
	{
		LOG->warn("连接失败:%s:%d.",ipaddr,port);
	}
	return m_connectId;
}


void CmdBase::disConnect(int connectId)
{
	FK_DisConnect(m_connectId);

	LOG->message("断开连接");

}
