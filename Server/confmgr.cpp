#include "confmgr.h"

bool ConfMgr::load(string fname)
{
	
	if (m_config.open() == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) %p\n"), ACE_TEXT("config.open()")),false);
	}

	ACE_Ini_ImpExp config_importer(m_config);
	if (config_importer.import_config(ACE_TEXT(fname.c_str())) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%P|%t) %p\n"), fname.c_str()),false);
	}

	return true;
}

string ConfMgr::getValue(string pkey,string key)
{
	
	ACE_Configuration_Section_Key status_section;
	if (m_config.open_section (m_config.root_section(),ACE_TEXT(pkey.c_str()), 0, status_section) == -1)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) %p\n"), ACE_TEXT ("Can't open [LOGGER] section")));
		return "";
	}
	string ret;

	ACE_TString strval;

	if (m_config.get_string_value(status_section, ACE_TEXT(key.c_str()), strval) != -1)
	{
		ret.append(strval.c_str());
		return ret;
	}

	return "";
}