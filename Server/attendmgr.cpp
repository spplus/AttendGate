#include "attendmgr.h"

AttendMgr::AttendMgr()
{

}

void AttendMgr::add(long mechineId,long connnectId)
{
	if (get(mechineId) == -1)
	{
		m_clients.insert(AMAP::value_type(mechineId, connnectId));
	}

}

long AttendMgr::get(long mechineId)
{
	AMAP::iterator iter = m_clients.find(mechineId);
	if (iter != m_clients.end())
	{
		return iter->second;
	}
	else
	{
		return -1;
	}
}

void AttendMgr::del(long mechineId)
{
	m_clients.erase(m_clients.find(mechineId));
}