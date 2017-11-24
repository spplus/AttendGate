#include "cmdbase.h"
#include "defines.h"

int CmdBase::connectNet(long mechineId,const char* ipaddr,int port)
{
	// ����
	m_connectId = FK_ConnectNet(mechineId, (char*)ipaddr, port, NTIMEOUT, PROTOCOL_TCPIP, NETPASSWORD, LICENSE);
	if (m_connectId>0)
	{
		LOG->message("�����豸�ɹ�:%s:%d.",ipaddr,port);
	}
	else
	{
		LOG->warn("�����豸ʧ��:%s:%d.",ipaddr,port);
	}
	return m_connectId;
}


void CmdBase::disConnect(int connectId)
{
	FK_DisConnect(m_connectId);

	LOG->message("�Ͽ��豸����");

}

string CmdBase::getResMsg(int resCode)
{
	string resMsg = "";
	switch (resCode)
	{
	case RUN_SUCCESS:
		resMsg = "ָ��ɹ���֪ͨ";
		break;
	case RUNERR_NOSUPPORT:
		resMsg = "������֧�ֶ�Ӧָ��Ĵ������";
		break;
	case RUNERR_UNKNOWNERROR:
		resMsg = "�����������";
		break;
	case RUNERR_NO_OPEN_COMM:
		resMsg = "����û�����ӵĴ������";
		break;
	case RUNERR_WRITE_FAIL:
		resMsg = "�����ϲ��ܴ������ݵĴ������";
		break;
	case RUNERR_READ_FAIL:
		resMsg = "�ӻ����ϲ��ܶ����ݵĴ������";
		break;
	case RUNERR_INVALID_PARAM:
		resMsg = "���Ĳ�������ȷ�Ĵ������";
		break;
	case RUNERR_NON_CARRYOUT:
		resMsg = "ָ��ʧ�ܵĴ������";
		break;
	case RUNERR_DATAARRAY_END:
		resMsg = "�������ݵ�֪ͨ";
		break;
	case RUNERR_DATAARRAY_NONE:
		resMsg = "���ݲ����ڵĴ������";
		break;
	case RUNERR_MEMORY:
		resMsg = "PC ���ڴ治��Ĵ������";
		break;
	case RUNERR_MIS_PASSWORD:
		resMsg = "���ӻ�����ɺ����ǲ���ȷ�Ĵ������";
		break;
	case RUNERR_MEMORYOVER:
		resMsg = "�����ϲ���洢���Ĵ������";
		break;
	case RUNERR_DATADOUBLE:
		resMsg = "Ҫ�Ǽǵĺ����Ѿ��ڻ�����������ڵĴ������";
		break;
	case RUNERR_MANAGEROVER:
		resMsg = "������û�пռ䣬δ�ܵǼǹ�����.���ݵĴ������";
		break;
	case RUNERR_FPDATAVERSION:
		resMsg = "Ҫ�õ�ָ�����ݵİ汾����ȷ�Ĵ������";
		break;
	default:
		resMsg = "δ֪����";
		break;
	}
	return resMsg;
}