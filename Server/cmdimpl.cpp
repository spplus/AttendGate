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
		LOG->error("�����豸ʧ��!�豸IP:%s,�豸���:%d.",attend.ipaddr().c_str(),attend.nmachinenumber());
	}
	return cid;
}

void CmdImpl::getUser(sClientMsg* msg)
{

	PBNS::GetEnrollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("��ʼ��ȡ�û����ݣ��豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnenrollnumer());

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

	LOG->message("��ȡ�û����ݳɹ����豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnenrollnumer());

}

void CmdImpl::putUser(sClientMsg* msg)
{
	PBNS::PutEnrollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("��ʼ����û����豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.user().apnenrollnumer());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}
	PBNS::PutEnrollDataMsg_Response res;
	int ret = FK_PutEnrollData(cid,req.user().apnenrollnumer(),req.user().apnbackupnumber(),req.user().apnmechineprivilege(),(void*)req.user().apnenrolldata().c_str(),req.user().apnpassword());
	if (ret == OP_SUCCUSS)
	{	
		LOG->message("����û��ɹ����豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.user().apnenrollnumer());
	}
	else
	{
		LOG->message("����û�ʧ�ܣ��豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.user().apnenrollnumer());
	}
	res.set_rescode(ret);
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
}

void CmdImpl::getLogData(sClientMsg* msg)
{
	PBNS::GetGeneralLogDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	LOG->message("��ʼ��ȡ�û�ǩ����¼���豸IP:%s,�豸���:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}
	PBNS::GetGeneralLogDataMsg_Response res;
	int ret = FK_LoadGeneralLogData(cid,req.readmask());
	if (ret != OP_SUCCUSS)
	{
		LOG->error("����ǩ����¼���ڴ�ʧ��.");
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
	LOG->message("��ȡ�û�ǩ����¼�������豸IP:%s,�豸���:%d����ȡ��¼����%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),res.logrec_size());
}

void CmdImpl::enableUser(sClientMsg* msg)
{
	PBNS::EnableUserMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("��ʼ�����û�״̬���豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

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

	LOG->message("�����û�״̬�ɹ����أ��豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

}

void CmdImpl::delUser(sClientMsg* msg)
{
	PBNS::DeleteErollDataMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);

	LOG->message("��ʼɾ���û����豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

	int cid = connect(req.attend());
	if (cid <= 0)
	{
		return;
	}

	int ret = FK_DeleteEnrollData(cid,req.apnenrollnumer(),req.apnbackupnumber());
	if (ret != OP_SUCCUSS)
	{
		LOG->message("ɾ���û�ʧ�ܣ��豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());
	}
	PBNS::DeleteErollDataMsg_Response res;
	res.set_rescode(ret);

	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);

	LOG->message("ɾ���û��ɹ����豸IP:%s,�豸���:%d���û�ID:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.apnbackupnumber());

}

void CmdImpl::setDoorStatus(sClientMsg* msg)
{
	PBNS::SetDoorStatusMsg_Request req;
	req.ParseFromArray(msg->data,msg->length);
	
	LOG->message("������״̬���豸IP:%s,�豸���:%d����״̬:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber(),req.status());

	int cid = connectNet(req.attend().nmachinenumber(),req.attend().ipaddr().c_str(),req.attend().nport());
	if (cid <= 0)
	{
		LOG->error("�����豸ʧ��!�豸IP:%s,�豸���:%d.",req.attend().ipaddr().c_str(),req.attend().nmachinenumber());
		return;
	}

	int ret = FK_SetDoorStatus(cid,req.status());
	disConnect(cid);

	PBNS::SetDoorStatusMsg_Response res;
	res.set_rescode(ret);
	string strData = res.SerializeAsString();
	App_ClientMgr::instance()->sendData(msg->connectId,strData,msg->type);
	
}