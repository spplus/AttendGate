#include "cmdbase.h"
#include "defines.h"

int CmdBase::connectNet(long mechineId,const char* ipaddr,int port)
{
	// 连接
	m_connectId = FK_ConnectNet(mechineId, (char*)ipaddr, port, NTIMEOUT, PROTOCOL_TCPIP, NETPASSWORD, LICENSE);
	if (m_connectId>0)
	{
		LOG->message("连接设备成功:%s:%d.",ipaddr,port);
	}
	else
	{
		LOG->warn("连接设备失败:%s:%d.",ipaddr,port);
	}
	return m_connectId;
}


void CmdBase::disConnect(int connectId)
{
	FK_DisConnect(m_connectId);

	LOG->message("断开设备连接");

}

string CmdBase::getResMsg(int resCode)
{
	string resMsg = "";
	switch (resCode)
	{
	case RUN_SUCCESS:
		resMsg = "指令成功的通知";
		break;
	case RUNERR_NOSUPPORT:
		resMsg = "机器不支持对应指令的错误代码";
		break;
	case RUNERR_UNKNOWNERROR:
		resMsg = "不明错误代码";
		break;
	case RUNERR_NO_OPEN_COMM:
		resMsg = "机器没有连接的错误代码";
		break;
	case RUNERR_WRITE_FAIL:
		resMsg = "机器上不能传送数据的错误代码";
		break;
	case RUNERR_READ_FAIL:
		resMsg = "从机器上不能读数据的错误代码";
		break;
	case RUNERR_INVALID_PARAM:
		resMsg = "读的参数不正确的错误代码";
		break;
	case RUNERR_NON_CARRYOUT:
		resMsg = "指令失败的错误代码";
		break;
	case RUNERR_DATAARRAY_END:
		resMsg = "读完数据的通知";
		break;
	case RUNERR_DATAARRAY_NONE:
		resMsg = "数据不存在的错误代码";
		break;
	case RUNERR_MEMORY:
		resMsg = "PC 的内存不足的错误代码";
		break;
	case RUNERR_MIS_PASSWORD:
		resMsg = "连接机器许可号码是不正确的错误代码";
		break;
	case RUNERR_MEMORYOVER:
		resMsg = "机器上不足存储器的错误代码";
		break;
	case RUNERR_DATADOUBLE:
		resMsg = "要登记的号码已经在机器数据里存在的错误代码";
		break;
	case RUNERR_MANAGEROVER:
		resMsg = "机器里没有空间，未能登记管理者.数据的错误代码";
		break;
	case RUNERR_FPDATAVERSION:
		resMsg = "要用的指纹数据的版本不正确的错误代码";
		break;
	default:
		resMsg = "未知代码";
		break;
	}
	return resMsg;
}