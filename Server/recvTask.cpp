#include "ace/Log_Msg.h"
#include "ace/Reactor.h"
#include "recvTask.h"
#include "scadaclientmgr.h"
#include "structs.h"

RecvTask::RecvTask()
{
	m_stop = false;
}

RecvTask::~RecvTask()
{
	
}

void RecvTask::start()
{
	activate();
}


void RecvTask::close()
{
	m_stop = true;

	LOG->message("Close reactor.");

}
int RecvTask::svc()
{
	LOG->message("RecvTask start ok.");
	ACE_Message_Block* mb = NULL;
	ACE_Time_Value tv(0,1);
	while (!m_stop)
	{
		if (getq(mb) != -1)
		{
			ParseData(mb);
			mb->release();
		}
		ACE_OS::sleep(tv);
	}

	LOG->message("RecvTask exit");

	// 系统退出
	close();
	return 0;
}

void RecvTask::ParseData(ACE_Message_Block* mb)
{

	// 1. 数据解包
	switch (mb->msg_type())
	{
	case SYS_MSG_CONNECTED:
		{
			//tcp连接建立后,立即发送U帧建立规约链接
			App_ScadaClient::instance()->sendUFrame();

			App_ScadaClient::instance()->stopTimer();
		}
		break;
	case SYS_MSG_CLOSED:
		{
			//tcp断开后,启动重新连接服务端定时器，关闭定时发送I帧的定时器
			App_ScadaClient::instance()->startTimer();

			App_ScadaClient::instance()->stopProTimer();
		}
		
		break;
	default:
		{
			//sClientMsg*	msg = m_pack.decoderS(mb->rd_ptr(),mb->length());
			// 调用业务路径处理
			//m_biz.exec(msg);

			//解析数据区
			App_ScadaClient::instance()->parseDataFrame(mb->rd_ptr(),mb->length());

		}
		
		break;
	}

}