/************************************************************************/
/* 
		DESC:	客户端和服务器之间协议解析.
		DATE:	2016-04-20
		AUTHOR:	YUANLS
*/
/************************************************************************/
#ifndef __PACKETPARSER_H__
#define __PACKETPARSER_H__

#include <string>
#include "structs.h"
using namespace std;

class PacketParser
{
public:
	
	// 客户端  <--> 服务器 打包
	sClientMsg*		decoder(char* data,int datalength);

	// 客户端  <--> 服务器 解包
	char*				encoder(string data,int msgtype,int &outlengh);

	// 服务器  <--> SCADA 打包
	sClientMsg*		decoderS(char* data,int datalength);

	// 服务器  <--> SCADA 解包
	char*				encoderS(char* data,int datalength,int &outlengh);

};

#endif