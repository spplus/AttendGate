/************************************************************************/
/* 
	DESC:	公共方法类.
	DATE:	2016-06-25
	AUTHOR:	YUANLS
*/
/************************************************************************/
#ifndef __COMUTILS_H__
#define __COMUTILS_H__

#include "defines.h"
#include "structs.h"
#include "clientmgr.h"
#include "buff/msgbody.pb.h"
#include "clientmgr.h"
#include "include/commands.h"

class ComUtils
{
public:
	int		str2i(string val);
	string	i2str(int val);
	string	getVal(STRMAP &smap,string key);
	int		getIval(STRMAP &smap,string key);
	void		triggerRule(RMAP &ruleMap,int rid);
	string	getCurTime();
};


typedef ACE_Singleton<ComUtils, ACE_Thread_Mutex>  App_Com;
#endif