/************************************************************************/
/* 
		DESC:		定义标签等常量.
		DATE:		2016-06-04
		AUTHOR:		ABELSKY
*/
/************************************************************************/


#ifndef __DEFINETAG_H__
#define __DEFINETAG_H__
#include <string>
//配置文件路径. 
//#define INIPATHNAME "cimin.ini"
////日志名称和路径配置.
//#define LOGPATH		"LOGPATH" 
//#define LOGNAME		"logname"
//#define LOGPATHNAME "logpath"
////数据库连接配置.
//#define DBINFO		"DBINFO" 
//#define DBADDR		"DBAddr"
//#define DBPORT		"DBPort"
//#define DBUSER		"DBUser"
//#define USERPWD		"UserPwd"
//#define DBNAME		"DBName"

//定义返回值
#define RC_FAILURE      -1
#define RC_SUCCESS		0

//定义支持解析CIM文件厂家:目前只支持南瑞、科东、东方电子、积成电子四种类型
#define FACNARI			1
#define FACKEDONG		2
#define FACDONGFANG		3
#define FACJICHENGDZ	4

//定义变压器三层高中低
#define PRIMARY			1
#define	SECONDARY		2
#define	TERTIARY		3

#define STR_PRIMARY				"primary"
#define STR_SECONDARY			"secondary"
#define STR_TERTIARY			"tertiary"

// 定义设备类型
enum eUnitType{eBreaker,eSwitch,eBus,eCapacitor,eLoad,eGenerator,ePt,eCT,eTransformer,eLine,eGrdSwitch};

/************************************标签定义***********************************************/
//定义公共标签
#define RDFID						"rdf:ID"
#define RDFRESOURCE					"rdf:resource"

#define CIM_NAMING_NAME				"cim:Naming.name"
#define CIM_NAMING_ALSNAME			"cim:Naming.aliasName"
#define CIM_NAMING_DESCRIPTION		"cim:Naming.description"

//定义基础电压类型标签
#define CIM_BVOL					"cim:BaseVoltage"
#define CIM_NARI_BVOL_NAME			"cimNARI:BaseVoltage.name"
#define CIM_BVOL_NALVOL				"cim:BaseVoltage.nominalVoltage"
#define CIM_IES_BVOL_NAME			"cimIESLAB:BaseVoltage.name"

//定义厂站标签
#define CIM_SUBSTATION				"cim:Substation"
#define CIM_NARI_SUBSTATION_BVOL	"cimNARI:Substation.MaxBaseVoltage"

//定义电压等级标签
#define CIM_VOLEVEL					"cim:VoltageLevel"
#define CIM_VOLEVEL_STATION			"cim:VoltageLevel.MemberOf_Substation"
#define CIM_VOLEVEL_BVOL			"cim:VoltageLevel.BaseVoltage"

//定义间隔标签
#define CIM_BAY						"cim:Bay"
#define CIM_BAY_MOF_VOLEVEL			"cim:Bay.MemberOf_VoltageLevel"
#define CIM_BAY_MOF_STATION			"cim:Bay.MemberOf_Substation"

//定义开关标签
#define CIM_BREAKER					"cim:Breaker"
#define CIM_CONDTEQPMENT_BVAL		"cim:ConductingEquipment.BaseVoltage"
#define CIM_EQPMENT_MOF_QPTCONT		"cim:Equipment.MemberOf_EquipmentContainer"

//定义刀闸标签
#define CIM_DISCTOR					"cim:Disconnector"
//定义接地刀闸标签
#define CIM_GRD_DISCTOR				"cim:GroundDisconnector"

//定义母线标签
#define CIM_BUS						"cim:BusbarSection"

//定义电容器标签
#define CIM_COMPENSATOR				"cim:Compensator"

//定义负荷标签
#define CIM_ENERGY_CONSUMER			"cim:EnergyConsumer"

//定义发电机标签
#define CIM_SYNC_HRUS_MACHINE		"cim:SynchronousMachine"

//定义电压互感器标签
#define CIM_PT						"cim:PotentialTransformer"

//定义变压器标签
#define CIM_POWER_TRANSFMER			"cim:PowerTransformer"

//定义变压器绕组标签
#define CIM_TRANSFMER_WINDING		"cim:TransformerWinding"
#define CIM_TRANSFMER_WINDING_TYPE	"cim:TransformerWinding.windingType"
#define CIM_TRANSFW_MOF_POWERTF		"cim:TransformerWinding.MemberOf_PowerTransformer"


//定义线路标签
#define CIM_ACLINESGT				"cim:ACLineSegment"
#define CIM_NARI_ACLINSGT_ST		"cimNARI:ACLineSegment.StartST"
#define CIM_NARI_ACLINSGT_END		"cimNARI:ACLineSegment.EndST"

#define CIM_ACLINSGT_FROM			"cim:ACLineSegment.FromSubstation"
#define CIM_ACLINSGT_TO				"cim:ACLineSegment.FromSubstation"
#define CIM_CDEQMENT_TERMINALS		"cim:ConductingEquipment.Terminals"

//定义端点信息
#define CIM_TERMINAL				"cim:Terminal"
#define CIM_TERMINAL_CONDEQMENT		"cim:Terminal.ConductingEquipment"
#define CIM_TERMINAL_CONDNODE		"cim:Terminal.ConnectivityNode"

//定义连接点
#define CIM_CONNECT_NODE			 "cim:ConnectivityNode"
#define CIM_CONNECT_NODE_MOFEQMNET	 "cim:ConnectivityNode.MemberOf_EquipmentContainer"

/************************************结构体定义***********************************************/
//电压基础信息
struct BaseVoltage
{
	std::string bvcimid;
	std::string bvname;
	std::string bvval;

};

//电压等级信息
struct VoltageLevel
{
	std::string vlcimid;
	std::string vlname;
	std::string faccimid;
	std::string bvcimid;
};

//站点信息
struct Substation
{
	std::string faccimid;
	std::string facname;
	std::string faccrname;
};

//间隔信息
struct Bay
{
	std::string baycimid;
	std::string bayname;
	std::string faccimid;
};

//端点信息
struct Terminal
{
	std::string tmlcimid;
	std::string tmlname;
	std::string cdcimid;
	std::string nodecimid;
};

//连接点信息
struct ConnectivityNode
{
	std::string nodecimid;
	std::string nodename;
	std::string vlcimid;
	std::string bvcimid;
	std::string faccimid;
};

//断路器信息
struct Breaker
{
	std::string brkcimid;
	std::string brkname;
	std::string faccimid;
	std::string bvcimid;
};


//刀闸信息
struct Disconnector
{
	std::string distcimid;
	std::string distname;
	std::string faccimid;
	std::string bvcimid;
};


//接地刀闸器信息
struct GroundDisconnector
{
	std::string gdistcimid;
	std::string gdistname;
	std::string faccimid;
	std::string bvcimid;
};

//母线信息
struct BusbarSection
{
	std::string buscimid;
	std::string busname;
	std::string faccimid;
	std::string bvcimid;
};

//电容器信息
struct Compensator
{
	std::string cptorcimid;
	std::string cptorname;
	std::string faccimid;
	std::string bvcimid;
};

//负荷信息
struct EnergyConsumer
{
	std::string eyccimid;
	std::string eycname;
	std::string faccimid;
	std::string bvcimid;
};

//水平发电机信息
struct SynchronousMachine
{
	std::string symhcimid;
	std::string symhname;
	std::string faccimid;
	std::string bvcimid;
};

//电压互感器信息
struct PT
{
	std::string ptcimid;
	std::string ptname;
	std::string faccimid;
	std::string bvcimid;
};


//变压器信息
struct PowerTransformer
{
	std::string ptfcimid;
	std::string ptfname;
	std::string faccimid;
};

//变压器信息
struct PowerTransformer_Units
{
	std::string ptfcimid;
	std::string ptfname;
	std::string faccimid;
	std::string bvcimid;
};

//变压器绕组信息
struct TransformerWinding
{
	std::string tfwcimid;
	std::string tfwname;
	std::string tfwtype;
	std::string vlcimid;
	std::string bvcimid;
	std::string ptfcimid;
};

//变压器绕组信息
struct TransformerWinding_Units
{
	std::string tfwcimid;
	std::string ptfcimid;
	std::string bvcimid;
	int wdgrade;
	int wdindex;	
};


//交流线段信息
struct ACLineSegment
{
	std::string aclcimid;
	std::string aclname;
	std::string fac1cimid;
	std::string fac2cimid;
	std::string bvcimid;
};


//端点信息
struct TerminalRelation
{
	std::string tmlcimid;
	std::string tmlname;
	std::string faccimid;
	std::string unitcimid;
	int tmlport;
	std::string nodecimid;

};

/************************************map定义***********************************************/
typedef std::map<std::string,BaseVoltage>		MAPBVOL;

typedef std::map<std::string,VoltageLevel>		MAPVOLEVEL;

typedef std::map<std::string,Substation>		MAPSUBSTATION;
typedef std::map<std::string,Bay>				MAPBAY;

typedef std::map<std::string,Terminal>			MAPTML;

typedef std::map<std::string,ConnectivityNode>	MAPCNODE;

typedef std::map<std::string,Breaker>			MAPBREAKER;

typedef	std::map<std::string,Disconnector>		MAPDISCTOR;

typedef std::map<std::string,GroundDisconnector> MAPGRDDISCTOR;

typedef std::map<std::string,BusbarSection>		MAPBUS;

typedef std::map<std::string,Compensator>		MAPCOMPTOR;

typedef std::map<std::string,EnergyConsumer>	MAPEYCONMER;

typedef std::map<std::string,SynchronousMachine> MAPSYNCMCHINE;

typedef std::map<std::string,PT>				MAPPT;

typedef std::map<std::string,PowerTransformer>	MAPPOWERTF;
typedef std::map<std::string,PowerTransformer_Units>	MAPPOWERTFUT;

typedef std::map<std::string,TransformerWinding>	MAPTRANSFW;
typedef std::map<std::string,TransformerWinding_Units>	MAPTRANSFWUT;

typedef std::map<std::string,ACLineSegment>		MAPACLINE;

typedef std::map<std::string,TerminalRelation>	MAPTMLRAL;
	




#endif

