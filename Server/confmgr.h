/************************************************************************/
/* 
	DESC:	读取配置文件.
	DATE:	2016-04-25
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __CONFMGR_H__
#define __CONFMGR_H__

#include <string>
#include <ace/Configuration.h>
#include <ace/Configuration_Import_Export.h>
#include <ace/Singleton.h>

using namespace std;
class ConfMgr
{
public:
	bool	load(string fname);
	string	getValue(string pkey,string key);

private:
	
	ACE_Configuration_Heap	m_config;
};
typedef ACE_Singleton<ConfMgr, ACE_Recursive_Thread_Mutex>  App_Config;
#endif