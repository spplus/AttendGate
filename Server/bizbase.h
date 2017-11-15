/************************************************************************/
/* 
		DESC:	业务逻辑处理管理器.根据不同的消息类型，分别创建不同的业务
					处理单元进行处理.
		DATE:	2017-11-20
		AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef		__BIZBASE_H__
#define		__BIZBASE_H__

#include "structs.h"

class BizBase
{
public:
	BizBase();

	// 请求获取用户
	void		getUser(sClientMsg* msg);
	
	// 添加用户
	void		putUser(sClientMsg* msg);

	// 请求打卡记录
	void		getLogData(sClientMsg* msg);

	// 设置用户使能
	void		enableUser(sClientMsg* msg);

	// 删除用户
	void		delUser(sClientMsg* msg);

	// 执行业务逻辑
	void		exec(sClientMsg* msg);
private:
	// 返回服务器忙碌
	void		sendBusyBack(sClientMsg* msg);

private:
	
	// 是否在处理中
	bool		m_isBusy;
};

#endif