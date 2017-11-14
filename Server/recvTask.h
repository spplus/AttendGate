
#ifndef __RECVTASK_H__
#define __RECVTASK_H__
#include "ace/Task.h"
#include "packetparser.h"
#include "bizbase.h"

class RecvTask	:public ACE_Task<ACE_MT_SYNCH>
{
public:
	// 构造函数
	RecvTask();
	~RecvTask();

	// 启动线程
	void	start();

	// 关闭线程
	void	close();

	// 线程入口
	int		svc();

private:
	// 解析数据
	void	ParseData(ACE_Message_Block* mb);

private:
	BizBase			m_biz;
	bool			m_stop;
	PacketParser	m_pack;

};


#endif