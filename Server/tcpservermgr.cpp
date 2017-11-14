
#include "defines.h"
#include "tcpservermgr.h"
#include "clientmsgservice.h"
#include "confmgr.h"

void TcpServerMgr::init()
{
	// 加载服务器配置
	m_port = ACE_OS::atoi(App_Config::instance()->getValue(SERVER_ROOT,"ListenPort").c_str());
	
	LOG->message("load server config ok.");
}

bool TcpServerMgr::start()
{
	// 1.启动侦听
	ACE_INET_Addr port_to_listen(m_port);
	if (m_connector.open(port_to_listen) == -1)
	{
		return false;
	}
	else
	{
		LOG->message("server started on port:%d",port_to_listen.get_port_number());

		// 启动客户端消息循环
		App_CMService::instance()->start();

		return true;
	}

}

void TcpServerMgr::stop()
{
	// 关闭断口
	m_connector.close();

	// 关闭客户端消息循环
	App_CMService::instance()->stop();
}