/************************************************************************/
/* 
		DESC:		解析xml文件内容.
		DATE:		2016-06-04
		AUTHOR:		ABELSKY
*/
/************************************************************************/
#ifndef __PARSEXML_H__
#define __PARSEXML_H__

#include <map>

#include "definetag.h"

#include "rapidxml.hpp"
//#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

class ParseXML
{
public:
	ParseXML(void);
	~ParseXML(void);

	void	loadXML(std::string filename,int factype);							//加载CIM文件
	MAPBVOL			getBaseVoltage(){return mapBaseVol;}								//获取基础电压map
	MAPVOLEVEL		getVollevel(){return mapVollevel;}
	MAPSUBSTATION	getSubstation(){return mapSubstation;}
	MAPTML			getTerminal(){return mapTerminal;}
	MAPCNODE		getConnectivityNode(){return mapConnectivityNode;}

	MAPBREAKER		getBreaker(){return mapBreaker;}
	MAPDISCTOR		getDisconnector(){return mapDisconnector;}
	MAPGRDDISCTOR	getGroundDisconnector(){return mapGroundDisconnector;}
	MAPBUS			getBusbarSection(){return mapBusbarSection;}
	MAPCOMPTOR		getCompensator(){return mapCompensator;}
	MAPEYCONMER		getEnergyConsumer(){return mapEnergyConsumer;}
	MAPSYNCMCHINE	getSynchronousMachine(){return mapSynchronousMachine;}
	MAPPT			getPT(){return mapPT;}
	MAPPOWERTFUT	getPowerTransformerUnt(){return mapPowerTransformerUnt;}
	MAPTRANSFWUT	getTransformerWindingUnt(){return mapTransformerWindingUnt;}

	MAPACLINE		getACLineSegment(){return mapACLineSegment;}
	MAPTMLRAL		getTerminalRelation(){return mapTerminalRelation;}


private:
	int		parseBaseVoltages(xml_node<>* parnode,int factype);					//遍历解析基础电压
	int		parseSubStation(xml_node<>* parnode,int factype);					//遍历解析厂站
	int		parseVoltagLevel(xml_node<>* parnode,int factype);					//遍历电压等级
	int		parseBay(xml_node<>* parnode,int factype);							//遍历间隔信息
	int		parseTerminal(xml_node<>* parnode,int factype);						//遍历解析端点信息
	int		parseConnectivityNode(xml_node<>* parnode,int factype);				//遍历解析连接点信息
	int		parseBreaker(xml_node<>* parnode,int factype);						//遍历解析断路器
	int		parseDisconnector(xml_node<>* parnode,int factype);					//遍历解析刀闸信息
	int		parseGrdDisconnector(xml_node<>* parnode,int factype);				//遍历解析接地刀闸信息
	int		parseBusbarSection(xml_node<>* parnode,int factype);				//遍历解析母线信息
	int		parseCompensator(xml_node<>* parnode,int factype);					//遍历解析电容器信息
	int		parseEnergyConsumer(xml_node<>* parnode,int factype);				//遍历解析负荷信息
	int		parseSynchronousMachine(xml_node<>* parnode,int factype);			//遍历解析发电机信息
	int		parsePotentialTransformer(xml_node<>* parnode,int factype);			//遍历解析电压互感器信息
	int		parsePowerTransformer(xml_node<>* parnode,int factype);				//遍历解析变压器信息
	int		parseTransformerWinding(xml_node<>* parnode,int factype);			//遍历解析变压器绕组信息
	int		parseACLineSegment(xml_node<>* parnode,int factype);				//遍历解析线路信息

	int		dealPowerTransformer();												//处理变压器信息
	int		dealTransformerWinding();											//处理变压器绕组信息
	int		dealTerminalRelation();												//处理端点关联关系表


private:
	MAPBVOL				mapBaseVol;					//保存基础电压信息
	MAPVOLEVEL			mapVollevel;				//保存电压等级信息
	MAPSUBSTATION		mapSubstation;				//保存厂站信息

	MAPBAY				mapBay;						//保存间隔信息
	MAPTML				mapTerminal;				//保存端点信息
	MAPCNODE			mapConnectivityNode;		//保存连接点信息

	MAPBREAKER			mapBreaker;					//保存断路器开关信息
	MAPDISCTOR			mapDisconnector;			//保存刀闸信息
	MAPGRDDISCTOR		mapGroundDisconnector;		//保存接地刀闸信息
	MAPBUS				mapBusbarSection;			//保存母线信息
	MAPCOMPTOR			mapCompensator;				//保存电容器信息
	MAPEYCONMER			mapEnergyConsumer;			//保存负荷信息
	MAPSYNCMCHINE		mapSynchronousMachine;		//保存发电机信息
	MAPPT				mapPT;						//保存电压互感器信息

	MAPPOWERTF			mapPowerTransformer;		//保存变压器信息
	MAPTRANSFW			mapTransformerWinding;		//保存变压器绕组信息

	MAPPOWERTFUT		mapPowerTransformerUnt;		//保存变压器存储到元件表中的信息
	MAPTRANSFWUT		mapTransformerWindingUnt;	//保存变压器绕组信息对应变压器线圈信息表

	MAPACLINE			mapACLineSegment;			//保存进出线信息

	MAPTMLRAL			mapTerminalRelation;		//保存端点连接关系信息

};
#endif // __PARSEXML_H__
