#include "bizbase.h"
#include "cmdbase.h"
#include "include/commands.h"


BizBase::BizBase()
{
	m_isBusy = false;
}

void BizBase::exec(sClientMsg* msg)
{
	CmdBase* pbase = NULL;
	switch (msg->type)
	{
	case CMD_GetAllUserID:	
	case CMD_PutErollData:
	case CMD_GetGeneralLogData:
	case CMD_EnableUser:
	case CMD_DelErollData:
		break;
	default:
		break;
	}
	if (pbase != NULL)
	{
		pbase->exec(msg);
		delete pbase;
		pbase = NULL;
		m_isBusy = false;
	}
}

void BizBase::sendBusyBack(sClientMsg* msg)
{
	string data = "server is busy,wait a moment please.";
	App_ClientMgr::instance()->sendData(msg->connectId,data,CMD_SERVER_BUSY);
}