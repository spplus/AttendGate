/************************************************************************/
/* 
	DESC:	服务器入口.
	DATE:	2017-11-14
	AUTHOR:	YUANLS	
*/
/************************************************************************/
#include "defines.h"
#include "logger.h"
#include "tcpservermgr.h"
#include "confmgr.h"
#include <WTypes.h>
#include "include/FKAttend.h"
#include "include/PreDefine.h"

int ACE_TMAIN (int, ACE_TCHAR *[]) 
{
	ACE::init();

	// 启动日志
	App_Logger::instance()->load_config(CONFIG_FILE);
	App_Logger::instance()->set_logname(LOGNAME);
	App_Logger::instance()->open_logger();

	// 加载服务器配置
	if (!App_Config::instance()->load(SERVER_CONFIG))
	{
		LOG->error("加载服务器配置失败，服务器自动退出.");
		return 0;
	}

	long m_vnMachineNumber = 1;
	int m_vnLicense = 1262;
	char* m_vStrIPAddr = "103.213.245.4";// 3s.net579.com
	long m_vnPortNum = 21545;
	long vnProtocolType = PROTOCOL_TCPIP;
	long m_vnTelNo = 806;
	long m_vnTimeOut = 5000;
	long m_vnPassword = 0;
	int m_vnWaitTIme = 10;
	int m_vnComTimeOut = 3000;
	
	// 测试连接
	long gnCommHandleIndex = FK_ConnectNet(m_vnMachineNumber, m_vStrIPAddr, m_vnPortNum, m_vnTimeOut, vnProtocolType, m_vnPassword, m_vnLicense);
	if (gnCommHandleIndex>0)
	{
		LOG->message("连接成功:%s:%d.",m_vStrIPAddr,m_vnPortNum);
	}
	else
	{
		LOG->warn("连接失败:%s:%d.",m_vStrIPAddr,m_vnPortNum);
	}
	
	
	// 启动服务器
	App_TcpServer::instance()->init();
	App_TcpServer::instance()->start();

	// 启动事件循环
	ACE_Reactor::instance()->run_reactor_event_loop();
	
	ACE::fini();
	return 0;
}