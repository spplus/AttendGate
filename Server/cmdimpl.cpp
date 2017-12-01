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
		LOG->error("�����豸ʧ��!�豸IP:%s,�豸���:%d.",attend.ipaddr().c_str(),attend.nmachinenumber());
	}
	return cid;
}

void CmdImpl::getUserList(sClientMsg* msg)
{
	PBNS::GetAllUserIDMsg_Request req;
	PBNS::GetAllUserIDMsg_Response res;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("��ʼ��ȡ�û��б��豸IP:%s,�豸���:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());

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
		LOG->message("��ȡ�û��б��ڴ�ʧ��,������Ϣ:%s.",getResMsg(ret).c_str());
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
	LOG->message("��ȡ�û��б�ɹ����û���:%d.",res.user_size());
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
	LOG->message("��ʼ��ȡ�û����ݣ��û�ID:%d.",req.apnenrollnumer());

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
		LOG->message("��ȡ�û����ݳɹ�.");
	}
	else
	{
		res.set_rescode(ret);
		res.set_resmsg(getResMsg(ret));
		LOG->message("��ȡ�û�����ʧ�ܣ�������Ϣ:%s.",getResMsg(ret).c_str());
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

	LOG->message("��ʼ����û����û�ID:%d.",req.user().apnenrollnumer());

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
		LOG->message("����û��ɹ�.");
	}
	else
	{
		LOG->message("����û�ʧ�ܣ�������Ϣ:%s.",getResMsg(ret).c_str());
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
	LOG->message("��ʼ��ȡ�û�ǩ����¼���豸���:%d.",req.attend().nmachinenumber());

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
		LOG->error("����ǩ����¼���ڴ�ʧ��,������Ϣ:%s.",getResMsg(ret).c_str());
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
		LOG->message("��ȡ�û�ǩ����¼�ɹ�����ȡ��¼����%d.",res.logrec_size());
	}
	else
	{
		res.set_rescode(0);
		LOG->message("��ȡ�û�ǩ����¼��������ȡ��¼����%d.",0);
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

	LOG->message("��ʼ�����û�״̬���û�ID:%d,���Ʊ�־:%d.",req.apnenrollnumer(),req.apnenableflag());

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
		LOG->message("�����û�״̬�ɹ�.");
	}
	else
	{
		LOG->message("�����û�״̬ʧ�ܣ�������Ϣ:%s.",getResMsg(ret).c_str());
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
	
	LOG->message("��ʼɾ���û�:%d",anpErollNumber);

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
		LOG->warn("ɾ���û�ʧ��,������Ϣ:%s.",getResMsg(ret).c_str());
	}
	else
	{
		LOG->message("ɾ���û��ɹ�");
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
	
	LOG->message("������״̬����״̬:%d",req.status());
	
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
		LOG->message("������״̬�ɹ�����״̬:%d",req.status());
	}
	else
	{
		LOG->message("������״̬ʧ��,������Ϣ:%s.",getResMsg(ret).c_str());
	}
	disConnect(cid);
	res.set_rescode(ret);
	res.set_resmsg(getResMsg(ret));

back:
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
	
}