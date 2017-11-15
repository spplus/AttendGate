#include "bizbase.h"
#include "cmdbase.h"
#include "include/commands.h"
#include "buff/msgbody.pb.h"

BizBase::BizBase()
{
	m_isBusy = false;
}

void BizBase::exec(sClientMsg* msg)
{
	switch (msg->type)
	{
	case CMD_GetAllUserID:	
		getUser(msg);
		break;
	case CMD_PutErollData:
		putUser(msg);
		break;
	case CMD_GetGeneralLogData:
		getLogData(msg);
		break;
	case CMD_EnableUser:
		enableUser(msg);
		break;
	case CMD_DelErollData:
		delUser(msg);
		break;
	default:
		break;
	}
	
}

void BizBase::getUser(sClientMsg* msg)
{
	PBNS::GetEnrollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->debug("%d",req.apnenrollnumer());
	
}

void BizBase::putUser(sClientMsg* msg)
{

}

void BizBase::getLogData(sClientMsg* msg)
{

}

void BizBase::enableUser(sClientMsg* msg)
{

}

void BizBase::delUser(sClientMsg* msg)
{

}



void BizBase::sendBusyBack(sClientMsg* msg)
{
	string data = "server is busy,wait a moment please.";
	App_ClientMgr::instance()->sendData(msg->connectId,data,CMD_GetAllUserID);
}