#include "cmdbase.h"
#include "buff/msgbody.pb.h"
#include "defines.h"
class CmdImpl	:public CmdBase
{
public:
	void		exec(sClientMsg* msg);
private:
	// 请求获取用户
	void		getUser(sClientMsg* msg);

	// 获取用户列表
	void		getUserList(sClientMsg* msg);

	// 添加用户
	void		putUser(sClientMsg* msg);

	// 请求打卡记录
	void		getLogData(sClientMsg* msg);

	// 设置用户使能
	void		enableUser(sClientMsg* msg);

	// 删除用户
	void		delUser(sClientMsg* msg);

	// 设置门状态
	void		setDoorStatus(sClientMsg* msg);

	// 连接设备
	int			connect(PBNS::AttendBean attend);
};