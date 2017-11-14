#include "comutils.h"
#include "ace/Date_Time.h"

int	ComUtils::str2i(string val)
{
	return ACE_OS::atoi(val.c_str());
}

string	ComUtils::i2str(int val)
{
	char temp[16];
	ACE_OS::itoa(val,temp,10);
	string strval;
	strval.append(temp);
	return strval;
}

int ComUtils::getIval(STRMAP &smap,string key)
{
	return str2i(getVal(smap,key));
}

string ComUtils::getVal(STRMAP &smap,string key)
{
	MAP_ITERATOR iter = smap.find(key);
	if (iter != smap.end())
	{
		return iter->second;
	}
	else
	{
		return "";
	}
}

void ComUtils::triggerRule(RMAP &ruleMap,int rid)
{
	R_ITERATOR iter = ruleMap.find(rid);
	if (iter != ruleMap.end())
	{
		ruleMap.erase(iter);
	}
}

string ComUtils::getCurTime()
{
	ACE_Date_Time tvTime(ACE_OS::gettimeofday());   
	ACE_DEBUG((LM_DEBUG,ACE_TEXT("%d:%d:%d:%d:%d:%d:%d\n"),tvTime.year(),tvTime.month(),tvTime.day(),   
		tvTime.hour(),tvTime.minute(),tvTime.second(),tvTime.microsec()));   
	char date[24];
	sprintf(date,"%d-%d-%d %d:%d:%d",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second());

	return string(date);
}