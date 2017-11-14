#include "parsexml.h"


ParseXML::ParseXML(void)
{
}


ParseXML::~ParseXML(void)
{
}


void ParseXML::loadXML(std::string filename,int factype)
{
	std::string strxml = filename;

	file<> fdoc(strxml.c_str());
	xml_document<> doc;
	doc.parse<0>(fdoc.data());

	//获取根节点
	xml_node<>* root = doc.first_node();


	//解析基础电压信息
	parseBaseVoltages(root,factype);

	//解析厂站信息
	parseSubStation(root,factype);

	//解析电压等级信息:关联厂站对应的基础电压
	parseVoltagLevel(root,factype);
	
	//解析间隔信息
	parseBay(root,factype);

	//解析端点信息
	parseTerminal(root,factype);

	//解析连接点信息
	parseConnectivityNode(root,factype);

	//解析断路器信息
	parseBreaker(root,factype);

	//解析刀闸信息
	parseDisconnector(root,factype);

	//解析接地刀闸信息
	parseGrdDisconnector(root,factype);

	//解析母线信息
	parseBusbarSection(root,factype);

	//解析电容器信息
	parseCompensator(root,factype);

	//解析负荷信息
	parseEnergyConsumer(root,factype);

	//解析发电机信息
	parseSynchronousMachine(root,factype);

	//解析电压互感器信息
	parsePotentialTransformer(root,factype);

	//解析变压器信息
	parsePowerTransformer(root,factype);

	//解析变压器绕组信息
	parseTransformerWinding(root,factype);

	//处理变压器
	dealPowerTransformer();

	//处理变压器绕组
	dealTransformerWinding();

	//解析线路信息
	parseACLineSegment(root,factype);

	//处理端点关联关系表
	dealTerminalRelation();
/**/
	return;
}


int ParseXML::parseBaseVoltages(xml_node<>* parnode,int factype)
{

	xml_node<>* pBaseVoltage = parnode->first_node(CIM_BVOL);			//获取<cim:BaseVoltage>节点
	if (!pBaseVoltage)
	{
		return RC_FAILURE;
	}

	mapBaseVol.clear();

	for (xml_node<>* pBVol = parnode->first_node(CIM_BVOL);pBVol != NULL; pBVol=pBVol->next_sibling(CIM_BVOL))
	{
		//取基础电压CIMID
		std::string strid = pBVol->first_attribute(RDFID)->value();

		std::string strbvname = "";
		//电压等级cim文件中没有名称字段,只有南瑞厂家有
		if (factype == FACNARI)
		{
			strbvname = pBVol->first_node(CIM_NARI_BVOL_NAME)->value();
		}

		if (factype == FACJICHENGDZ)
		{
			strbvname = pBVol->first_node(CIM_IES_BVOL_NAME)->value();
		}

		//取基础电压对应值
		xml_node<>* pNalVol = pBVol->first_node(CIM_BVOL_NALVOL);
		std::string strnval = pNalVol->value();

		BaseVoltage bvol;
		bvol.bvcimid = strid;
		bvol.bvname = strbvname;
		bvol.bvval = strnval;

		mapBaseVol.insert(std::pair<std::string,BaseVoltage>(strid,bvol));

	}


	return RC_SUCCESS;
}


int ParseXML::parseSubStation(xml_node<>* parnode,int factype)
{
	xml_node<>* pSubStation = parnode->first_node(CIM_SUBSTATION);			//获取<cim:Substation>节点
	if (!pSubStation)
	{
		return RC_FAILURE;
	}

	mapSubstation.clear();

	for (xml_node<>* pStation = parnode->first_node(CIM_SUBSTATION);pStation != NULL; pStation=pStation->next_sibling(CIM_SUBSTATION))
	{
		//取厂站CIMID
		std::string strid = pStation->first_attribute(RDFID)->value();

		//取厂站名称
		xml_node<>* pStaname = pStation->first_node(CIM_NAMING_NAME);
		std::string strname = pStaname->value();

		//取厂站别名
		std::string stralsname = "";
		if (factype == FACNARI || factype == FACKEDONG || factype == FACDONGFANG)
		{
			stralsname = pStation->first_node(CIM_NAMING_ALSNAME)->value();
		}

		if (factype == FACJICHENGDZ)
		{
			stralsname = pStation->first_node(CIM_NAMING_DESCRIPTION)->value();
		}
		

		Substation subst;
		subst.faccimid = strid;
		subst.faccrname = stralsname;
		subst.facname = strname;

		mapSubstation.insert(std::pair<std::string,Substation>(strid,subst));
	}

	return RC_SUCCESS;
}


int ParseXML::parseVoltagLevel(xml_node<>* parnode,int factype)
{
	xml_node<>* pVoltagLevel = parnode->first_node(CIM_VOLEVEL);			//获取<cim:VoltageLevel>节点
	if (!pVoltagLevel)
	{
		return RC_FAILURE;
	}

	mapVollevel.clear();

	for (xml_node<>* pVolevel = parnode->first_node(CIM_VOLEVEL);pVolevel != NULL; pVolevel=pVolevel->next_sibling(CIM_VOLEVEL))
	{
		//取电压等级CIMID
		std::string strid = pVolevel->first_attribute(RDFID)->value();

		std::string strname = "";
		std::string strstationid = "";
		std::string strbvid = "";

		if (factype == FACNARI || factype == FACKEDONG || factype == FACDONGFANG || factype == FACJICHENGDZ)
		{
			//取电压等级名称
			xml_node<>* pVolevelname = pVolevel->first_node(CIM_NAMING_NAME);
			strname = pVolevelname->value();

			//取关联厂站CIMID
			strstationid = pVolevel->first_node(CIM_VOLEVEL_STATION)->first_attribute(RDFRESOURCE)->value();
			strstationid = strstationid.substr(1,strstationid.length()-1);

			//取关联基础电压
			strbvid = pVolevel->first_node(CIM_VOLEVEL_BVOL)->first_attribute(RDFRESOURCE)->value();
			strbvid = strbvid.substr(1,strbvid.length()-1);
		}

		VoltageLevel volel;
		volel.vlcimid = strid;
		volel.vlname = strname;
		volel.faccimid = strstationid;
		volel.bvcimid = strbvid;

		mapVollevel.insert(std::pair<std::string,VoltageLevel>(strid,volel));
	}

	return RC_SUCCESS;
}

int ParseXML::parseBay(xml_node<>* parnode,int factype)
{
	xml_node<>* pBayy = parnode->first_node(CIM_BAY);			//获取<cim:Bay>节点
	if (!pBayy)
	{
		return RC_FAILURE;
	}

	mapBay.clear();

	for (xml_node<>* pBay = parnode->first_node(CIM_BAY);pBay != NULL; pBay=pBay->next_sibling(CIM_BAY))
	{
		//取间隔CIMID
		std::string strid = pBay->first_attribute(RDFID)->value();

		//取间隔名称
		std::string strname = pBay->first_node(CIM_NAMING_NAME)->value();

		//取间隔对应关联的厂站CIMID
		std::string strfacid = pBay->first_node(CIM_BAY_MOF_STATION)->first_attribute(RDFRESOURCE)->value();
		strfacid = strfacid.substr(1,strfacid.length()-1);

		Bay bay;
		bay.baycimid = strid;
		bay.bayname = strname;
		bay.faccimid = strfacid;

		mapBay.insert(std::pair<std::string,Bay>(strid,bay));
	}

	return RC_SUCCESS;
}

int ParseXML::parseTerminal(xml_node<>* parnode,int factype)
{
	xml_node<>* pTerminal = parnode->first_node(CIM_TERMINAL);			//获取<cim:Terminal>节点
	if (!pTerminal)
	{
		return RC_FAILURE;
	}

	mapTerminal.clear();

	for (xml_node<>* pTminal = parnode->first_node(CIM_TERMINAL);pTminal != NULL; pTminal=pTminal->next_sibling(CIM_TERMINAL))
	{
		//取端点CIMID
		std::string strid = pTminal->first_attribute(RDFID)->value();

		//取端点名称
		std::string strname = pTminal->first_node(CIM_NAMING_NAME)->value();

		std::string strcdeqment = "";
		std::string strnode = "";

		//取端点关联的连接点CIMID
		xml_node<>* pCNode = pTminal->first_node(CIM_TERMINAL_CONDNODE);
		if (pCNode)
		{
			strnode = pCNode->first_attribute(RDFRESOURCE)->value();
			strnode = strnode.substr(1,strnode.length()-1);
		}
		
		//取端点关联的元件CIMID
		xml_node<>* pCdeqment = pTminal->first_node(CIM_TERMINAL_CONDEQMENT);
		if (pCdeqment)
		{
			strcdeqment = pCdeqment->first_attribute(RDFRESOURCE)->value();
			strcdeqment = strcdeqment.substr(1,strcdeqment.length()-1);
		}
	
		Terminal terml;
		terml.tmlcimid = strid;
		terml.tmlname = strname;
		terml.nodecimid = strnode;
		terml.cdcimid = strcdeqment;
		
		mapTerminal.insert(std::pair<std::string,Terminal>(strid,terml));
	}

	return RC_SUCCESS;
}


int ParseXML::parseConnectivityNode(xml_node<>* parnode,int factype)
{
	xml_node<>* pConnectivityNode = parnode->first_node(CIM_CONNECT_NODE);			//获取<cim:ConnectivityNode>节点
	if (!pConnectivityNode)
	{
		return RC_FAILURE;
	}

	mapConnectivityNode.clear();

	for (xml_node<>* pCNode = parnode->first_node(CIM_CONNECT_NODE);pCNode != NULL; pCNode=pCNode->next_sibling(CIM_CONNECT_NODE))
	{
		//取连接点CIMID
		std::string strid = pCNode->first_attribute(RDFID)->value();

		//取连接点名称
		std::string strname = pCNode->first_node(CIM_NAMING_NAME)->value();

		std::string stvlcimid = "";

		//取连接点关联的电压等级cimid
		stvlcimid = pCNode->first_node(CIM_CONNECT_NODE_MOFEQMNET)->first_attribute(RDFRESOURCE)->value();
		stvlcimid = stvlcimid.substr(1,stvlcimid.length()-1);

		std::map<std::string,VoltageLevel>::iterator vliter;
		vliter = mapVollevel.find(stvlcimid);
		if (vliter != mapVollevel.end())
		{
			ConnectivityNode CNode;
			CNode.nodecimid = strid;
			CNode.nodename = strname;
			CNode.vlcimid = stvlcimid;
			CNode.faccimid = vliter->second.faccimid;
			CNode.bvcimid = vliter->second.bvcimid;

			mapConnectivityNode.insert(std::pair<std::string,ConnectivityNode>(strid,CNode));
		}
	}

	return RC_SUCCESS;
}



int ParseXML::parseBreaker(xml_node<>* parnode,int factype)
{
	xml_node<>* pBreaker = parnode->first_node(CIM_BREAKER);			//获取<cim:Breaker>节点
	if (!pBreaker)
	{
		return RC_FAILURE;
	}

	mapBreaker.clear();

	for (xml_node<>* pBker = parnode->first_node(CIM_BREAKER);pBker != NULL; pBker=pBker->next_sibling(CIM_BREAKER))
	{
		//取断路器CIMID
		std::string strid = pBker->first_attribute(RDFID)->value();

		//取断路器名称
		std::string strname = pBker->first_node(CIM_NAMING_NAME)->value();

		std::string strbvid = "";

		std::string strbayid = "";
		std::string strvolevid = "";

		if (factype == FACNARI)
		{
			//取关联基础电压
			strbvid = pBker->first_node(CIM_CONDTEQPMENT_BVAL)->first_attribute(RDFRESOURCE)->value();
			strbvid = strbvid.substr(1,strbvid.length()-1);

			//取断路器关联BAY的cimid
			strbayid = pBker->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
			strbayid = strbayid.substr(1,strbayid.length()-1);

			std::map<std::string,Bay>::iterator bayiter;
			bayiter = mapBay.find(strbayid);
			if (bayiter != mapBay.end())
			{
				Breaker brker;
				brker.brkcimid = strid;
				brker.brkname = strname;
				brker.faccimid = bayiter->second.faccimid;
				brker.bvcimid = strbvid;

				mapBreaker.insert(std::pair<std::string,Breaker>(strid,brker));
			}
		}

		if (factype == FACKEDONG || factype == FACDONGFANG || factype == FACJICHENGDZ)
		{
			//取断路器关联电压等级的cimid
			strvolevid = pBker->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
			strvolevid = strvolevid.substr(1,strvolevid.length()-1);

			std::map<std::string,VoltageLevel>::iterator vliter;
			vliter = mapVollevel.find(strvolevid);
			if (vliter != mapVollevel.end())
			{
				Breaker brker;
				brker.brkcimid = strid;
				brker.brkname = strname;
				brker.faccimid = vliter->second.faccimid;
				brker.bvcimid = vliter->second.bvcimid;

				mapBreaker.insert(std::pair<std::string,Breaker>(strid,brker));
			}		

		}
	}

	return RC_SUCCESS;
}


int ParseXML::parseDisconnector(xml_node<>* parnode,int factype)
{
	xml_node<>* pDiscnntor = parnode->first_node(CIM_DISCTOR);			//获取<cim:Disconnector>节点
	if (!pDiscnntor)
	{
		return RC_FAILURE;
	}

	mapDisconnector.clear();

	for (xml_node<>* pDisctor = parnode->first_node(CIM_DISCTOR);pDisctor != NULL; pDisctor=pDisctor->next_sibling(CIM_DISCTOR))
	{
		//取刀闸CIMID
		std::string strid = pDisctor->first_attribute(RDFID)->value();

		//取刀闸名称
		std::string strname = pDisctor->first_node(CIM_NAMING_NAME)->value();

		std::string strbvid = "";

		std::string strbayid = "";
		std::string strvolevid = "";

		if (factype == FACNARI)
		{
			//取关联基础电压
			strbvid = pDisctor->first_node(CIM_CONDTEQPMENT_BVAL)->first_attribute(RDFRESOURCE)->value();
			strbvid = strbvid.substr(1,strbvid.length()-1);

			//取刀闸关联BAY的cimid
			strbayid = pDisctor->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
			strbayid = strbayid.substr(1,strbayid.length()-1);

			std::map<std::string,Bay>::iterator bayiter;
			bayiter = mapBay.find(strbayid);
			if (bayiter != mapBay.end())
			{
				Disconnector Discntor;
				Discntor.distcimid = strid;
				Discntor.distname = strname;
				Discntor.faccimid = bayiter->second.faccimid;
				Discntor.bvcimid = strbvid;

				mapDisconnector.insert(std::pair<std::string,Disconnector>(strid,Discntor));
			}
		}

		if (factype == FACKEDONG || factype == FACDONGFANG || factype == FACJICHENGDZ)
		{
			//取关联电压等级
			strvolevid = pDisctor->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
			strvolevid = strvolevid.substr(1,strvolevid.length()-1);

			std::map<std::string,VoltageLevel>::iterator vliter;
			vliter = mapVollevel.find(strvolevid);
			if (vliter != mapVollevel.end())
			{
				Disconnector Discntor;
				Discntor.distcimid = strid;
				Discntor.distname = strname;
				Discntor.faccimid = vliter->second.faccimid;
				Discntor.bvcimid = vliter->second.bvcimid;

				mapDisconnector.insert(std::pair<std::string,Disconnector>(strid,Discntor));
			}		
		}
	}

	return RC_SUCCESS;
}


int ParseXML::parseGrdDisconnector(xml_node<>* parnode,int factype)
{
	xml_node<>* pGrdDiscnntor = parnode->first_node(CIM_GRD_DISCTOR);			//获取<cim:GroundDisconnector>节点
	if (!pGrdDiscnntor)
	{
		return RC_FAILURE;
	}

	mapGroundDisconnector.clear();

	for (xml_node<>* pGrdDisctor = parnode->first_node(CIM_GRD_DISCTOR);pGrdDisctor != NULL; pGrdDisctor=pGrdDisctor->next_sibling(CIM_GRD_DISCTOR))
	{
		//取刀闸CIMID
		std::string strid = pGrdDisctor->first_attribute(RDFID)->value();

		//取刀闸名称
		std::string strname = pGrdDisctor->first_node(CIM_NAMING_NAME)->value();

		std::string strbvid = "";

		std::string strbayid = "";
		std::string strvolevid = "";

		if (factype == FACNARI)
		{
			//取关联基础电压
			strbvid = pGrdDisctor->first_node(CIM_CONDTEQPMENT_BVAL)->first_attribute(RDFRESOURCE)->value();
			strbvid = strbvid.substr(1,strbvid.length()-1);

			//取断路器关联BAY的cimid
			strbayid = pGrdDisctor->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
			strbayid = strbayid.substr(1,strbayid.length()-1);

			std::map<std::string,Bay>::iterator bayiter;
			bayiter = mapBay.find(strbayid);
			if (bayiter != mapBay.end())
			{
				GroundDisconnector GDiscntor;
				GDiscntor.gdistcimid = strid;
				GDiscntor.gdistname = strname;
				GDiscntor.faccimid = bayiter->second.faccimid;
				GDiscntor.bvcimid = strbvid;

				mapGroundDisconnector.insert(std::pair<std::string,GroundDisconnector>(strid,GDiscntor));
			}
		}

		if (factype == FACKEDONG || factype == FACDONGFANG || factype == FACJICHENGDZ)
		{
			//取关联电压等级
			strvolevid = pGrdDisctor->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
			strvolevid = strvolevid.substr(1,strvolevid.length()-1);

			std::map<std::string,VoltageLevel>::iterator vliter;
			vliter = mapVollevel.find(strvolevid);
			if (vliter != mapVollevel.end())
			{
				GroundDisconnector GDiscntor;
				GDiscntor.gdistcimid = strid;
				GDiscntor.gdistname = strname;
				GDiscntor.faccimid = vliter->second.faccimid;
				GDiscntor.bvcimid = vliter->second.bvcimid;

				mapGroundDisconnector.insert(std::pair<std::string,GroundDisconnector>(strid,GDiscntor));
			}		
		}
	}


	return RC_SUCCESS;
}


int ParseXML::parseBusbarSection(xml_node<>* parnode,int factype)
{
	xml_node<>* pBusbar = parnode->first_node(CIM_BUS);			//获取<cim:BusbarSection>节点
	if (!pBusbar)
	{
		return RC_FAILURE;
	}

	mapBusbarSection.clear();

	for (xml_node<>* pBus = parnode->first_node(CIM_BUS);pBus != NULL; pBus=pBus->next_sibling(CIM_BUS))
	{
		//取母线CIMID
		std::string strid = pBus->first_attribute(RDFID)->value();

		//取母线名称
		std::string strname = pBus->first_node(CIM_NAMING_NAME)->value();

		std::string strvolevid = "";

		//取关联电压等级
		strvolevid = pBus->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
		strvolevid = strvolevid.substr(1,strvolevid.length()-1);

		std::map<std::string,VoltageLevel>::iterator vliter;
		vliter = mapVollevel.find(strvolevid);
		if (vliter != mapVollevel.end())
		{
			BusbarSection Busbar;
			Busbar.buscimid = strid;
			Busbar.busname = strname;
			Busbar.faccimid = vliter->second.faccimid;
			Busbar.bvcimid = vliter->second.bvcimid;

			mapBusbarSection.insert(std::pair<std::string,BusbarSection>(strid,Busbar));
		}
	}

	return RC_SUCCESS;
}


int ParseXML::parseCompensator(xml_node<>* parnode,int factype)
{
	xml_node<>* pCompensator = parnode->first_node(CIM_COMPENSATOR);			//获取<cim:Compensator>节点
	if (!pCompensator)
	{
		return RC_FAILURE;
	}

	mapCompensator.clear();

	for (xml_node<>* pCompstor = parnode->first_node(CIM_COMPENSATOR);pCompstor != NULL; pCompstor=pCompstor->next_sibling(CIM_COMPENSATOR))
	{
		//取母线CIMID
		std::string strid = pCompstor->first_attribute(RDFID)->value();

		//取母线名称
		std::string strname = pCompstor->first_node(CIM_NAMING_NAME)->value();

		std::string strvolevid = "";

		//取关联电压等级
		strvolevid = pCompstor->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
		strvolevid = strvolevid.substr(1,strvolevid.length()-1);

		std::map<std::string,VoltageLevel>::iterator vliter;
		vliter = mapVollevel.find(strvolevid);
		if (vliter != mapVollevel.end())
		{
			Compensator Comptor;
			Comptor.cptorcimid = strid;
			Comptor.cptorname = strname;
			Comptor.faccimid = vliter->second.faccimid;
			Comptor.bvcimid = vliter->second.bvcimid;

			mapCompensator.insert(std::pair<std::string,Compensator>(strid,Comptor));
		}
	}

	return RC_SUCCESS;
}


int ParseXML::parseEnergyConsumer(xml_node<>* parnode,int factype)
{
	xml_node<>* pEnergy = parnode->first_node(CIM_ENERGY_CONSUMER);			//获取<cim:EnergyConsumer>节点
	if (!pEnergy)
	{
		return RC_FAILURE;
	}

	mapEnergyConsumer.clear();

	for (xml_node<>* pEngy = parnode->first_node(CIM_ENERGY_CONSUMER);pEngy != NULL; pEngy=pEngy->next_sibling(CIM_ENERGY_CONSUMER))
	{
		//取发电机CIMID
		std::string strid = pEngy->first_attribute(RDFID)->value();

		//取发电机名称
		std::string strname = pEngy->first_node(CIM_NAMING_NAME)->value();

		std::string strvolevid = "";

		//取关联电压等级
		strvolevid = pEngy->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
		strvolevid = strvolevid.substr(1,strvolevid.length()-1);

		std::map<std::string,VoltageLevel>::iterator vliter;
		vliter = mapVollevel.find(strvolevid);
		if (vliter != mapVollevel.end())
		{
			EnergyConsumer Energycmer;
			Energycmer.eyccimid = strid;
			Energycmer.eycname = strname;
			Energycmer.faccimid = vliter->second.faccimid;
			Energycmer.bvcimid = vliter->second.bvcimid;

			mapEnergyConsumer.insert(std::pair<std::string,EnergyConsumer>(strid,Energycmer));
		}
	}

	return RC_SUCCESS;
}


int ParseXML::parseSynchronousMachine(xml_node<>* parnode,int factype)
{
	xml_node<>* pSynchronousMachine = parnode->first_node(CIM_SYNC_HRUS_MACHINE);			//获取<cim:SynchronousMachine>节点
	if (!pSynchronousMachine)
	{
		return RC_FAILURE;
	}

	mapSynchronousMachine.clear();

	for (xml_node<>* pSyncMachine = parnode->first_node(CIM_SYNC_HRUS_MACHINE);pSyncMachine != NULL; pSyncMachine=pSyncMachine->next_sibling(CIM_SYNC_HRUS_MACHINE))
	{
		//取发电机CIMID
		std::string strid = pSyncMachine->first_attribute(RDFID)->value();

		//取发电机名称
		std::string strname = pSyncMachine->first_node(CIM_NAMING_NAME)->value();

		std::string strvolevid = "";

		//取关联电压等级
		xml_node<>* pVolev = pSyncMachine->first_node(CIM_EQPMENT_MOF_QPTCONT);
		if (pVolev)
		{
			strvolevid = pVolev->first_attribute(RDFRESOURCE)->value();
			strvolevid = strvolevid.substr(1,strvolevid.length()-1);
		}

		std::map<std::string,VoltageLevel>::iterator vliter;
		vliter = mapVollevel.find(strvolevid);
		if (vliter != mapVollevel.end())
		{
			SynchronousMachine Syncmachine;
			Syncmachine.symhcimid = strid;
			Syncmachine.symhname = strname;
			Syncmachine.faccimid = vliter->second.faccimid;
			Syncmachine.bvcimid = vliter->second.bvcimid;

			mapSynchronousMachine.insert(std::pair<std::string,SynchronousMachine>(strid,Syncmachine));
		}
	}

	return RC_SUCCESS;
}

int ParseXML::parsePotentialTransformer(xml_node<>* parnode,int factype)
{
	xml_node<>* pPtrans = parnode->first_node(CIM_PT);			//获取<cim:PotentialTransformer>节点
	if (!pPtrans)
	{
		return RC_FAILURE;
	}

	mapPT.clear();

	for (xml_node<>* pPT = parnode->first_node(CIM_PT);pPT != NULL; pPT=pPT->next_sibling(CIM_PT))
	{
		//取电压互感器CIMID
		std::string strid = pPT->first_attribute(RDFID)->value();

		//取电压互感器名称
		std::string strname = pPT->first_node(CIM_NAMING_NAME)->value();

		std::string strvolevid = "";

		//取关联电压等级
		strvolevid = pPT->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
		strvolevid = strvolevid.substr(1,strvolevid.length()-1);

		std::map<std::string,VoltageLevel>::iterator vliter;
		vliter = mapVollevel.find(strvolevid);
		if (vliter != mapVollevel.end())
		{
			PT Pt;
			Pt.ptcimid = strid;
			Pt.ptname = strname;
			Pt.faccimid = vliter->second.faccimid;
			Pt.bvcimid = vliter->second.bvcimid;

			mapPT.insert(std::pair<std::string,PT>(strid,Pt));
		}
	}

	return RC_SUCCESS;
}


int ParseXML::parsePowerTransformer(xml_node<>* parnode,int factype)
{
	xml_node<>* pPowerTrasf = parnode->first_node(CIM_POWER_TRANSFMER);			//获取<cim:PowerTransformer>节点
	if (!pPowerTrasf)
	{
		return RC_FAILURE;
	}

	mapPowerTransformer.clear();

	for (xml_node<>* pPowertf = parnode->first_node(CIM_POWER_TRANSFMER);pPowertf != NULL; pPowertf=pPowertf->next_sibling(CIM_POWER_TRANSFMER))
	{
		//取变压器CIMID
		std::string strid = pPowertf->first_attribute(RDFID)->value();

		//取变压器名称
		std::string strname = pPowertf->first_node(CIM_NAMING_NAME)->value();

		std::string strstationid = "";

		//取变压器对应厂站cimid
		strstationid = pPowertf->first_node(CIM_EQPMENT_MOF_QPTCONT)->first_attribute(RDFRESOURCE)->value();
		strstationid = strstationid.substr(1,strstationid.length()-1);

		PowerTransformer ptformer;
		ptformer.ptfcimid = strid;
		ptformer.ptfname = strname;
		ptformer.faccimid = strstationid;

		mapPowerTransformer.insert(std::pair<std::string,PowerTransformer>(strid,ptformer));

	}

	return RC_SUCCESS;
}


int ParseXML::parseTransformerWinding(xml_node<>* parnode,int factype)
{
	xml_node<>* pTransfwind = parnode->first_node(CIM_TRANSFMER_WINDING);			//获取<cim:TransformerWinding>节点
	if (!pTransfwind)
	{
		return RC_FAILURE;
	}

	mapTransformerWinding.clear();

	for (xml_node<>* pTransfw = parnode->first_node(CIM_TRANSFMER_WINDING);pTransfw != NULL; pTransfw=pTransfw->next_sibling(CIM_TRANSFMER_WINDING))
	{
		//取变压器绕组CIMID
		std::string strid = pTransfw->first_attribute(RDFID)->value();

		//取变压器绕组名称
		std::string strname = pTransfw->first_node(CIM_NAMING_NAME)->value();

		std::string strtype = "";
		std::string strstationid = "";
		std::string strbvid = "";
		std::string strptf = "";
		std::string strvolevid = "";

		//区变压器绕组对应的当前侧(高、中、低)
		xml_node<>* pType = pTransfw->first_node(CIM_TRANSFMER_WINDING_TYPE);
		if (pType)
		{
			strtype = pType->first_attribute(RDFRESOURCE)->value();
			std::size_t nsize = strtype.find_last_of(".");
			strtype = strtype.substr(nsize+1,strtype.length()-nsize-1);
		}	

		//取变压器绕组对应所属变压器cimid
		xml_node<>* pPtformer = pTransfw->first_node(CIM_TRANSFW_MOF_POWERTF);
		if (pPtformer)
		{
			strptf = pPtformer->first_attribute(RDFRESOURCE)->value();
			strptf = strptf.substr(1,strptf.length()-1);
		}

		//取变压器绕组对应电压等级
		xml_node<>* pVolev = pTransfw->first_node(CIM_EQPMENT_MOF_QPTCONT);
		if (pVolev)
		{
			strvolevid = pVolev->first_attribute(RDFRESOURCE)->value();
			strvolevid = strvolevid.substr(1,strvolevid.length()-1);
		}

		std::map<std::string,VoltageLevel>::iterator voliter = mapVollevel.find(strvolevid);
		if (voliter != mapVollevel.end())
		{
			strstationid = voliter->second.faccimid;
			strbvid = voliter->second.bvcimid;
		}
		
		TransformerWinding tfwinding;
		tfwinding.tfwcimid = strid;
		tfwinding.tfwname = strname;
		tfwinding.tfwtype = strtype;
		tfwinding.ptfcimid = strptf;
		tfwinding.vlcimid = strvolevid;
		tfwinding.bvcimid = strbvid;

		mapTransformerWinding.insert(std::pair<std::string,TransformerWinding>(strid,tfwinding));

	}

	return RC_SUCCESS;
}


int ParseXML::dealPowerTransformer()
{
	mapPowerTransformerUnt.clear();

	for (std::map<std::string,TransformerWinding>::iterator iter = mapTransformerWinding.begin();iter != mapTransformerWinding.end();iter++)
	{
		iter->second.tfwcimid;

		if (iter->second.tfwtype == STR_PRIMARY)
		{
			std::map<std::string,PowerTransformer>::iterator ptfiter = mapPowerTransformer.find(iter->second.ptfcimid);
			if (ptfiter != mapPowerTransformer.end())
			{
				PowerTransformer_Units ptformerunt;
				ptformerunt.ptfcimid = ptfiter->second.ptfcimid;
				ptformerunt.ptfname = ptfiter->second.ptfname;
				ptformerunt.faccimid = ptfiter->second.faccimid;
				ptformerunt.bvcimid = iter->second.bvcimid;

				mapPowerTransformerUnt.insert(std::pair<std::string,PowerTransformer_Units>(ptfiter->second.ptfcimid,ptformerunt));
			}
		}
		
	}

	return RC_SUCCESS;
}


int ParseXML::dealTransformerWinding()
{
	mapTransformerWindingUnt.clear();

	std::map<std::string,PowerTransformer>::iterator ptfiter;
	std::map<std::string,TransformerWinding>::iterator tfwiter;

	for (ptfiter = mapPowerTransformer.begin(); ptfiter != mapPowerTransformer.end(); ptfiter++)
	{
		std::string strprimcimid = "";
		std::string strpscndcimid = "";
		std::string strptertcimid = "";

		std::string strprimbvid = "";
		std::string strpscndbvid = "";
		std::string strptertbvid = "";
		int nCont = 0;

		for (tfwiter=mapTransformerWinding.begin(); tfwiter != mapTransformerWinding.end(); tfwiter++)
		{
			if (ptfiter->first == tfwiter->second.ptfcimid)
			{
				//高压
				if (tfwiter->second.tfwtype == STR_PRIMARY)
				{
					strprimcimid = tfwiter->second.tfwcimid;
					strprimbvid = tfwiter->second.bvcimid;
				}

				//中压
				if (tfwiter->second.tfwtype == STR_SECONDARY)
				{
					strpscndcimid = tfwiter->second.tfwcimid;
					strpscndbvid = tfwiter->second.bvcimid;
				}

				//低压
				if (tfwiter->second.tfwtype == STR_TERTIARY)
				{
					strptertcimid = tfwiter->second.tfwcimid;
					strptertbvid = tfwiter->second.bvcimid;
				}

				//对变压器下有几个变压器绕组进行计数
				nCont++;
			}
		}

		//变压器为两圈变
		if (nCont == 2)
		{
			//高压侧对应值
			TransformerWinding_Units tfwdingg;
			tfwdingg.tfwcimid = strprimcimid;
			tfwdingg.ptfcimid = ptfiter->first;
			tfwdingg.bvcimid = strprimbvid;
			tfwdingg.wdindex = 0;
			tfwdingg.wdgrade = 3;
			mapTransformerWindingUnt.insert(std::pair<std::string,TransformerWinding_Units>(strprimcimid,tfwdingg));

			//低压侧对应值
			TransformerWinding_Units tfwdingd;
			tfwdingd.tfwcimid = strpscndcimid;
			tfwdingd.ptfcimid = ptfiter->first;
			tfwdingd.bvcimid = strpscndbvid;
			tfwdingd.wdindex = 1;
			tfwdingd.wdgrade = 1;
			mapTransformerWindingUnt.insert(std::pair<std::string,TransformerWinding_Units>(strpscndcimid,tfwdingd));
		}

		//变压器为两圈变
		if (nCont == 3)
		{
			//高压侧对应值
			TransformerWinding_Units tfwdingg;
			tfwdingg.tfwcimid = strprimcimid;
			tfwdingg.ptfcimid = ptfiter->first;
			tfwdingg.bvcimid = strprimbvid;
			tfwdingg.wdindex = 0;
			tfwdingg.wdgrade = 3;
			mapTransformerWindingUnt.insert(std::pair<std::string,TransformerWinding_Units>(strprimcimid,tfwdingg));

			//中压侧对应值
			TransformerWinding_Units tfwdingz;
			tfwdingz.tfwcimid = strpscndcimid;
			tfwdingz.ptfcimid = ptfiter->first;
			tfwdingz.bvcimid = strpscndbvid;
			tfwdingz.wdindex = 1;
			tfwdingz.wdgrade = 2;
			mapTransformerWindingUnt.insert(std::pair<std::string,TransformerWinding_Units>(strpscndcimid,tfwdingz));

			//低压侧对应值
			TransformerWinding_Units tfwdingd;
			tfwdingd.tfwcimid = strptertcimid;
			tfwdingd.ptfcimid = ptfiter->first;
			tfwdingd.bvcimid = strptertbvid;
			tfwdingd.wdindex = 2;
			tfwdingd.wdgrade = 1;
			mapTransformerWindingUnt.insert(std::pair<std::string,TransformerWinding_Units>(strptertcimid,tfwdingd));
		}

	}

	return RC_SUCCESS;
}


int ParseXML::parseACLineSegment(xml_node<>* parnode,int factype)
{
	xml_node<>* pACLineSegment = parnode->first_node(CIM_ACLINESGT);			//获取<cim:ACLineSegment>节点
	if (!pACLineSegment)
	{
		return RC_FAILURE;
	}

	mapACLineSegment.clear();

	for (xml_node<>* pACLine = parnode->first_node(CIM_ACLINESGT);pACLine != NULL; pACLine=pACLine->next_sibling(CIM_ACLINESGT))
	{
		//取交流线段CIMID
		std::string strid = pACLine->first_attribute(RDFID)->value();

		//取交流线段名称
		std::string strname = pACLine->first_node(CIM_NAMING_NAME)->value();

		std::string strbvid = "";

		std::string strscimid = "";
		std::string strecimid = "";

		//取交流线段关联的基础电压cimid
		strbvid = pACLine->first_node(CIM_CONDTEQPMENT_BVAL)->first_attribute(RDFRESOURCE)->value();
		strbvid = strbvid.substr(1,strbvid.length()-1);

		if (factype == FACNARI)
		{
			//取交流线段一端对应厂站cimid
			strscimid = pACLine->first_node(CIM_NARI_ACLINSGT_ST)->first_attribute(RDFRESOURCE)->value();
			strscimid = strscimid.substr(1,strscimid.length()-1);

			//取交流线段另一端对应厂站cimid
			strecimid = pACLine->first_node(CIM_NARI_ACLINSGT_END)->first_attribute(RDFRESOURCE)->value();
			strecimid = strecimid.substr(1,strecimid.length()-1);

			ACLineSegment acline;
			acline.aclcimid = strid;
			acline.aclname = strname;
			acline.bvcimid = strbvid;
			acline.fac1cimid = strscimid;
			acline.fac2cimid = strecimid;

			mapACLineSegment.insert(std::pair<std::string,ACLineSegment>(strid,acline));
		}

		if (factype == FACJICHENGDZ)
		{
			//取交流线段一端对应厂站cimid
			strscimid = pACLine->first_node(CIM_ACLINSGT_FROM)->first_attribute(RDFRESOURCE)->value();
			strscimid = strscimid.substr(1,strscimid.length()-1);

			//取交流线段另一端对应厂站cimid
			strecimid = pACLine->first_node(CIM_ACLINSGT_TO)->first_attribute(RDFRESOURCE)->value();
			strecimid = strecimid.substr(1,strecimid.length()-1);

			ACLineSegment acline;
			acline.aclcimid = strid;
			acline.aclname = strname;
			acline.bvcimid = strbvid;
			acline.fac1cimid = strscimid;
			acline.fac2cimid = strecimid;

			mapACLineSegment.insert(std::pair<std::string,ACLineSegment>(strid,acline));
		}

		std::string strtml1cimid = "";
		std::string strtml2cimid = "";

		std::string strnd1cimid = "";
		std::string strnd2cimid = "";

		if (factype==FACKEDONG || factype==FACDONGFANG)
		{
			//取交流线段一端对应端点cimid
			strtml1cimid = pACLine->first_node(CIM_CDEQMENT_TERMINALS)->first_attribute(RDFRESOURCE)->value();
			strtml1cimid = strtml1cimid.substr(1,strtml1cimid.length()-1);

			//取交流线段另一端对应端点cimid
			strtml2cimid = pACLine->last_node(CIM_CDEQMENT_TERMINALS)->first_attribute(RDFRESOURCE)->value();
			strtml2cimid = strtml2cimid.substr(1,strtml2cimid.length()-1);

			std::map<std::string,Terminal>::iterator tmliter;
			std::map<std::string,ConnectivityNode>::iterator nodeiter;
			std::map<std::string,VoltageLevel>::iterator vliter;

			//取关联一侧端点
			tmliter = mapTerminal.find(strtml1cimid);
			if (tmliter != mapTerminal.end())
			{
				strnd1cimid = tmliter->second.nodecimid;

				nodeiter = mapConnectivityNode.find(strnd1cimid);
				if (nodeiter != mapConnectivityNode.end())
				{
					strscimid = nodeiter->second.faccimid;
				}
			}
			
			//取关联另一侧端点
			tmliter = mapTerminal.find(strtml2cimid);
			if (tmliter != mapTerminal.end())
			{
				strnd2cimid = tmliter->second.nodecimid;

				nodeiter = mapConnectivityNode.find(strnd2cimid);
				if (nodeiter != mapConnectivityNode.end())
				{
					strecimid = nodeiter->second.faccimid;
				}
			}

			ACLineSegment acline;
			acline.aclcimid = strid;
			acline.aclname = strname;
			acline.bvcimid = strbvid;
			acline.fac1cimid = strscimid;
			acline.fac2cimid = strecimid;

			mapACLineSegment.insert(std::pair<std::string,ACLineSegment>(strid,acline));

		}

	}

	return RC_SUCCESS;
}


int ParseXML::dealTerminalRelation()
{
	mapTerminalRelation.clear();

	std::map<std::string,Terminal>::iterator tmliter;
	std::map<std::string,ConnectivityNode>::iterator nodeiter;
	std::map<std::string,TerminalRelation>::iterator trlaiter;

	std::map<std::string,std::string> mapTemp;
	std::map<std::string,std::string>::iterator tempiter;

	for (tmliter=mapTerminal.begin();tmliter!=mapTerminal.end();tmliter++)
	{
		//判断端点是否关联元件cimid若没有跳出当前循环
		if (tmliter->second.cdcimid == "")
		{
			continue;
		}

		std::string nodecimid = "";
		std::string faccimid = "";
		
		nodecimid = tmliter->second.nodecimid;

		nodeiter = mapConnectivityNode.find(nodecimid);
		if (nodeiter != mapConnectivityNode.end())
		{
			faccimid = nodeiter->second.faccimid;

			TerminalRelation Tmlrelat;
			Tmlrelat.tmlcimid = tmliter->second.tmlcimid;
			Tmlrelat.tmlname = tmliter->second.tmlname;
			Tmlrelat.faccimid = faccimid;
			Tmlrelat.unitcimid = tmliter->second.cdcimid;
			Tmlrelat.nodecimid = nodecimid;
			Tmlrelat.tmlport = 0;

			//当端点中对应关联的元件cimid相等时，说明当前map中有了同元件中的一侧端点，所以此时再插入表示此元件的另一端
			/* //此方法效率差
			trlaiter = mapTerminalRelation.begin();
			for (;trlaiter != mapTerminalRelation.end();trlaiter++)
			{
				if (tmliter->second.cdcimid == trlaiter->second.unitcimid)
				{
					Tmlrelat.tmlport = 1;
					continue;
				}
			}*/

			//查找临时map中是否已经保存了端点中对应关联的元件cimid，若没有则插入进去为以后端点另一侧的关联的元件cimid相等时用到
			//当端点中对应关联的元件cimid相等时，说明当前map中有了同元件中的一侧端点，所以此时再插入表示此元件的另一端
			tempiter = mapTemp.find(tmliter->second.cdcimid);
			if (tempiter != mapTemp.end())
			{
				Tmlrelat.tmlport = 1;
			}
			else
			{
				mapTemp.insert(std::pair<std::string,std::string>(tmliter->second.cdcimid,tmliter->second.cdcimid));
			}

			mapTerminalRelation.insert(std::pair<std::string,TerminalRelation>(tmliter->second.tmlcimid,Tmlrelat));

		}

	}


	return RC_SUCCESS;
}


