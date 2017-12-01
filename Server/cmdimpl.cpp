#include "cmdimpl.h"


void CmdImpl::exec(sClientMsg* msg){
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
	case CMD_GetAllUserID:
		getUserList(msg);
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

void CmdImpl::getUserList(sClientMsg* msg)
{
	PBNS::GetAllUserIDMsg_Request req;
	PBNS::GetAllUserIDMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("开始获取用户列表，设备IP:%s,设备编号:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;
	}
	int ret = FK_ReadAllUserID(cid);
	if (ret != RUN_SUCCESS)
	{
		LOG->message("读取用户列表到内存失败,错误信息:%s.",getResMsg(ret).c_str());
		res.set_rescode(ret);
		res.set_resmsg(getResMsg(ret));
		goto back;
	}

	long apnErollNumber,apnBackUpNumer,apnMechinePrivalige,apnEnableFlag;
	ret = RUN_SUCCESS;
	while(ret == RUN_SUCCESS)
	{
		ret = FK_GetAllUserID(cid,&apnErollNumber,&apnBackUpNumer,&apnMechinePrivalige,&apnEnableFlag);
		PBNS::UserBean  *bean = res.add_user();
		bean->set_apnenrollnumer(apnErollNumber);
		bean->set_apnbackupnumber(apnBackUpNumer);
		bean->set_apnmechineprivilege(apnMechinePrivalige);
		bean->set_apnenableflag(apnEnableFlag);
	}
	res.set_rescode(1);
	LOG->message("获取用户列表成功，用户数:%d.",res.user_size());
back:
	disConnect(cid);

	string strData = res.SerializeAsString();

	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);


}

void CmdImpl::getUser(sClientMsg* msg)
{

	PBNS::GetEnrollDataMsg_Request req;
	PBNS::GetEnrollDataMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("开始获取用户数据，用户ID:%d.",req.apnenrollnumer());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;
	}
	long apnMechinePrivalege,password;
	char apnErollData[2048];// = new char[10240];
	memset(&apnErollData,0,2048);

	int ret = FK_GetEnrollData(cid,req.apnenrollnumer(),req.apnbackupnumber(),&apnMechinePrivalege,apnErollData,&password);
	int length = 0;
	if (ret == RUN_SUCCESS)
	{
		res.mutable_user()->set_apnenrollnumer(req.apnenrollnumer());
		res.mutable_user()->set_apnbackupnumber(req.apnbackupnumber());
		res.mutable_user()->set_apnmechineprivilege(apnMechinePrivalege);
		if (apnErollData != NULL)
		{
			res.mutable_user()->set_apnenrolldata(apnErollData);
			length = strlen(apnErollData);
			length = res.mutable_user()->apnenrolldata().length();
		}
		res.mutable_user()->set_apnpassword(password);
		res.set_rescode(1);
		LOG->message("获取用户数据成功.");
	}
	else
	{
		res.set_rescode(ret);
		res.set_resmsg(getResMsg(ret));
		LOG->message("获取用户数据失败，错误信息:%s.",getResMsg(ret).c_str());
	}

	disConnect(cid);
back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);


}

void CmdImpl::putUser(sClientMsg* msg)
{
	PBNS::PutEnrollDataMsg_Request req;
	PBNS::PutEnrollDataMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("开始添加用户，用户ID:%d.",req.user().apnenrollnumer());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;
	}
	
	int ret = FK_PutEnrollData(cid,req.user().apnenrollnumer(),req.user().apnbackupnumber(),req.user().apnmechineprivilege(),(void*)req.user().apnenrolldata().c_str(),req.user().apnpassword());
	if (ret == RUN_SUCCESS)
	{	
		LOG->message("添加用户成功.");
	}
	else
	{
		LOG->message("添加用户失败，错误信息:%s.",getResMsg(ret).c_str());
	}
	res.set_rescode(ret);
	res.set_resmsg(getResMsg(ret));
	
back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
}

void CmdImpl::getLogData(sClientMsg* msg)
{
	PBNS::GetGeneralLogDataMsg_Request req;
	PBNS::GetGeneralLogDataMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("开始获取用户签到记录，设备编号:%d.",req.attend().nmachinenumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;
	}
	
	int ret = FK_LoadGeneralLogData(cid,req.readmask());
	if (ret != RUN_SUCCESS)
	{
		LOG->error("加载签到记录到内存失败,错误信息:%s.",getResMsg(ret).c_str());
		res.set_rescode(ret);
		res.set_resmsg(getResMsg(ret));
		goto back;
	}

	long erollNumber,verifyMode,inOutMode;
	DATE dateTime;
	ret = FK_GetGeneralLogData(cid,&erollNumber,&verifyMode,&inOutMode,&dateTime);
	while (ret == RUN_SUCCESS)
	{
		PBNS::LogDataBean* bean	= res.add_logrec();
		bean->set_apnenrollnumer(erollNumber);
		bean->set_apnverifymode(verifyMode);
		bean->set_apninoutmode(inOutMode);
		bean->set_apndatetime(dateTime);
		ret = FK_GetGeneralLogData(cid,&erollNumber,&verifyMode,&inOutMode,&dateTime);
	}
	if (res.logrec_size()>0)
	{
		res.set_rescode(1);
		LOG->message("获取用户签到记录成功，获取记录数：%d.",res.logrec_size());
	}
	else
	{
		res.set_rescode(0);
		LOG->message("获取用户签到记录结束，获取记录数：%d.",0);
	}
	
	res.set_resmsg(getResMsg(ret));
back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
	
}

void CmdImpl::enableUser(sClientMsg* msg)
{
	PBNS::EnableUserMsg_Request req;
	PBNS::EnableUserMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("开始控制用户状态，用户ID:%d,控制标志:%d.",req.apnenrollnumer(),req.apnenableflag());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;;
	}
	int ret = FK_EnableUser(cid,req.apnenrollnumer(),req.apnbackupnumber(),req.apnenableflag());
	if (ret == RUN_SUCCESS)
	{
		LOG->message("控制用户状态成功.");
	}
	else
	{
		LOG->message("控制用户状态失败，错误信息:%s.",getResMsg(ret).c_str());
	}
	disConnect(cid);
	res.set_rescode(ret);
	res.set_resmsg(getResMsg(ret));
	
back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);

	

}

void CmdImpl::delUser(sClientMsg* msg)
{
	PBNS::DeleteErollDataMsg_Request req;
	PBNS::DeleteErollDataMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);

	int anpErollNumber = req.apnenrollnumer();
	
	LOG->message("开始删除用户:%d",anpErollNumber);

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;
	}

	int ret = FK_DeleteEnrollData(cid,req.apnenrollnumer(),req.apnbackupnumber());
	if (ret != RUN_SUCCESS)
	{
		LOG->warn("删除用户失败,错误信息:%s.",getResMsg(ret).c_str());
	}
	else
	{
		LOG->message("删除用户成功");
	}
	
	res.set_rescode(ret);
	res.set_resmsg(getResMsg(ret));

back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);


}

void CmdImpl::setDoorStatus(sClientMsg* msg)
{
	PBNS::SetDoorStatusMsg_Request req;
	PBNS::SetDoorStatusMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);
	
	LOG->message("控制门状态，门状态:%d",req.status());
	
	int cid = connectNet(req.attend().nmachinenumber(),req.attend().ipaddr().c_str(),req.attend().nport());
	if (cid <= 0)
	{
		res.set_rescode(RUNERR_NO_OPEN_COMM);
		res.set_resmsg(getResMsg(RUNERR_NO_OPEN_COMM));
		goto back;
	}

	int ret = FK_SetDoorStatus(cid,req.status());
	ret = FK_SetDoorStatus(cid,DOOR_CONROLRESET);
	if (ret == RUN_SUCCESS)
	{
		LOG->message("控制门状态成功，门状态:%d",req.status());
	}
	else
	{
		LOG->message("控制门状态失败,错误信息:%s.",getResMsg(ret).c_str());
	}
	disConnect(cid);
	res.set_rescode(ret);
	res.set_resmsg(getResMsg(ret));

back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
	
}