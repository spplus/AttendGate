#include "cmdimpl.h"


void CmdImpl::exec(sClientMsg* msg)
{
	switch (msg->type)
	{
	case CMD_GetErollData:	
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
	case CMD_SetDoorStatus:
		setDoorStatus(msg);
		break;
	default:
		break;
	}
}

void CmdImpl::getUser(sClientMsg* msg)
{

	PBNS::GetEnrollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->debug("%d",req.apnenrollnumer());

	int cid = connectNet(req.attend().nmachinenumber(),req.attend().ipaddr().c_str(),req.attend().nport());
	if (cid <= 0)
	{
		LOG->error("连接设备失败!设备IP:%s,设备编号:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());
		return;
	}
	
	long apnErollNumber,apnBackUpNumber,apnMechinePrivalege,apnErollData,password;

	int ret = FK_GetEnrollData(cid,apnErollNumber,apnBackUpNumber,&apnMechinePrivalege,&apnErollData,&password);

	PBNS::GetEnrollDataMsg_Response res;
	if (ret == OP_SUCCUSS)
	{
		res.mutable_user()->set_apnenrollnumer(apnErollNumber);
		res.mutable_user()->set_apnbackupnumber(apnBackUpNumber);
		res.mutable_user()->set_apnmechineprivilege(apnMechinePrivalege);
		res.mutable_user()->set_apnenrolldata(apnErollData);
		res.mutable_user()->set_apnpassword(password);
		res.set_rescode(1);
	}
	else
	{
		res.set_rescode(0);
	}

	disConnect(cid);

	string strData = res.SerializeAsString();

	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
}

void CmdImpl::putUser(sClientMsg* msg)
{

}

void CmdImpl::getLogData(sClientMsg* msg)
{

}

void CmdImpl::enableUser(sClientMsg* msg)
{
	PBNS::EnableUserMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("控制用户状态，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

	int cid = connectNet(req.attend().nmachinenumber(),req.attend().ipaddr().c_str(),req.attend().nport());
	if (cid <= 0)
	{
		LOG->error("连接设备失败!设备IP:%s,设备编号:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());
		return;
	}

	int ret = FK_EnableUser(cid,req.apnenrollnumer(),req.apnbackupnumber(),req.apnenableflag());
	disConnect(cid);

	PBNS::EnableUserMsg_Response res;
	res.set_rescode(ret);
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
}

void CmdImpl::delUser(sClientMsg* msg)
{

}

void CmdImpl::setDoorStatus(sClientMsg* msg)
{
	PBNS::SetDoorStatusMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	
	LOG->message("控制门状态，设备IP:%s,设备编号:%d，门状态:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.status());

	int cid = connectNet(req.attend().nmachinenumber(),req.attend().ipaddr().c_str(),req.attend().nport());
	if (cid <= 0)
	{
		LOG->error("连接设备失败!设备IP:%s,设备编号:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());
		return;
	}

	int ret = FK_SetDoorStatus(cid,req.status());
	disConnect(cid);

	PBNS::SetDoorStatusMsg_Response res;
	res.set_rescode(ret);
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
	
}