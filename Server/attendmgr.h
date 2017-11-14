#ifndef __ATTENDMGR_H__
#define __ATTENDMGR_H__

#include <map>
#include "ace/Mutex.h"
#include "ace/Singleton.h"

using namespace std;

typedef 	map<long,long>	AMAP;

class AttendMgr
{
public:
	AttendMgr();
	void	add(long mechineId,long connnectId);
	long	get(long mechineId);
	void	del(long mechineId);

private:
	// 设备和连接句柄的映射关系
	AMAP		m_clients;
};

typedef ACE_Singleton<AttendMgr, ACE_Recursive_Thread_Mutex>  App_AttendMgr;
#endif