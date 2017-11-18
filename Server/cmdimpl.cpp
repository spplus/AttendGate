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

int CmdImpl::connect(PBNS::AttendBean attend)
{
	int cid = connectNet(attend.nmachinenumber(),attend.ipaddr().c_str(),attend.nport());
	if (cid <= 0)
	{
		LOG->error("连接设备失败!设备IP:%s,设备编号:%d.",attend.ipaddr().c_str(),attend.nmachinenumber());
	}
	return cid;
}

void CmdImpl::getUser(sClientMsg* msg)
{

	PBNS::GetEnrollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("开始获取用户数据，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnenrollnumer());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}
	long apnMechinePrivalege,password;
	char *apnErollData;
	int ret = FK_GetEnrollData(cid,req.apnenrollnumer(),req.apnbackupnumber(),&apnMechinePrivalege,apnErollData,&password);

	PBNS::GetEnrollDataMsg_Response res;
	if (ret == OP_SUCCUSS)
	{
		res.mutable_user()->set_apnenrollnumer(req.apnenrollnumer());
		res.mutable_user()->set_apnbackupnumber(req.apnbackupnumber());
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

	LOG->message("获取用户数据成功，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnenrollnumer());

}

void CmdImpl::putUser(sClientMsg* msg)
{
	PBNS::PutEnrollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("开始添加用户，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.user().apnenrollnumer());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}
	PBNS::PutEnrollDataMsg_Response res;
	int ret = FK_PutEnrollData(cid,req.user().apnenrollnumer(),req.user().apnbackupnumber(),req.user().apnmechineprivilege(),(void*)req.user().apnenrolldata().c_str(),req.user().apnpassword());
	if (ret == OP_SUCCUSS)
	{	
		LOG->message("添加用户成功，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.user().apnenrollnumer());
	}
	else
	{
		LOG->message("添加用户失败，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.user().apnenrollnumer());
	}
	res.set_rescode(ret);
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
}

void CmdImpl::getLogData(sClientMsg* msg)
{
	PBNS::GetGeneralLogDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("开始获取用户签到记录，设备IP:%s,设备编号:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}
	PBNS::GetGeneralLogDataMsg_Response res;
	int ret = FK_LoadGeneralLogData(cid,req.readmask());
	if (ret != OP_SUCCUSS)
	{
		LOG->error("加载签到记录到内存失败.");
		res.set_rescode(ret);
	}

	long erollNumber,verifyMode,inOutMode;
	DATE dateTime;
	ret = FK_GetGeneralLogData(cid,&erollNumber,&verifyMode,&inOutMode,&dateTime);
	while (ret != OP_SUCCUSS)
	{
		PBNS::LogDataBean* bean	= res.add_logrec();
		bean->set_apnenrollnumer(erollNumber);
		bean->set_apnverifymode(verifyMode);
		bean->set_apninoutmode(inOutMode);
		bean->set_apndatetime(dateTime);
	}
	if (res.logrec_size()>0)
	{
		res.set_rescode(1);
	}
	else
	{
		res.set_rescode(0);
	}
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
	LOG->message("获取用户签到记录结束，设备IP:%s,设备编号:%d，获取记录数：%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),res.logrec_size());
}

void CmdImpl::enableUser(sClientMsg* msg)
{
	PBNS::EnableUserMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("开始控制用户状态，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}
	int ret = FK_EnableUser(cid,req.apnenrollnumer(),req.apnbackupnumber(),req.apnenableflag());
	disConnect(cid);

	PBNS::EnableUserMsg_Response res;
	res.set_rescode(ret);
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);

	LOG->message("控制用户状态成功返回，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

}

void CmdImpl::delUser(sClientMsg* msg)
{
	PBNS::DeleteErollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("开始删除用户，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}

	int ret = FK_DeleteEnrollData(cid,req.apnenrollnumer(),req.apnbackupnumber());
	if (ret != OP_SUCCUSS)
	{
		LOG->message("删除用户失败，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());
	}
	PBNS::DeleteErollDataMsg_Response res;
	res.set_rescode(ret);

	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);

	LOG->message("删除用户成功，设备IP:%s,设备编号:%d，用户ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

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