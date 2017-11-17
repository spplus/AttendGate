#include "bizbase.h"
#include "cmdbase.h"
#include "include/commands.h"
#include "buff/msgbody.pb.h"
#include "cmdimpl.h"
BizBase::BizBase()
{
	m_isBusy = false;
}

void BizBase::exec(sClientMsg* msg)
{
	CmdBase * pbase = new CmdImpl();
	
	if (pbase != NULL)
	{
		pbase->exec(msg);
	}
	
}





void BizBase::sendBusyBack(sClientMsg* msg)
{

	string data = "server is busy,wait a moment please.";
	App_ClientMgr::instance()->sendData(msg->connectId,data,CMD_GetErollData);
}