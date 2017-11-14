#include "packetparser.h"
#include "defines.h"

sClientMsg* PacketParser::decoder(char* data,int datalength)
{
	// 消息标识头
	unsigned short head;
	unsigned short end;
	int pos = 0;
	ACE_OS::memcpy(&head,data+pos,FRAME_TAG__LEN);
	pos += FRAME_TAG__LEN;

	// 判断消息头时候正确
	if (head != FRAM_TAG_HEAD)
	{
		LOG->warn("Invalid frame head tag :%d",head);
		return NULL;
	}

	sClientMsg* pmsg = new sClientMsg;

	// 解析消息类型
	ACE_OS::memcpy(&pmsg->type,data+pos,DATA_TYPE_LEN);
	pos += DATA_TYPE_LEN;

	// 解析数据区内容
	// 数据区长度 
	int datalen = datalength - FRAME_TAG__LEN-DATA_TYPE_LEN-FRAME_TAG__LEN;
	pmsg->data = new char[datalen];
	
	// 拷贝消息长度
	ACE_OS::memcpy(pmsg->data,data+pos,datalen);
	pos += datalen;

	pmsg->length = datalen;

	// 消息尾
	ACE_OS::memcpy(&end,data+pos,FRAME_TAG__LEN);
	pos += FRAME_TAG__LEN;

	return pmsg;
}

char* PacketParser::encoder(string data,int msgtype,int &outlengh)
{
	
	// 打包后总总长度 = 包内容总长度+消息头长度+消息类型长度+消息尾长度
	outlengh = data.length()+FRAME_HEAD_LEN+FRAME_TAG__LEN+DATA_TYPE_LEN+FRAME_TAG__LEN;

	// 分配打包后的消息缓冲区
	char * buff = new char[outlengh];
	int pos = 0;
	
	// 消息总长度
	ACE_OS::memcpy(buff+pos,&outlengh,FRAME_HEAD_LEN);
	pos += FRAME_HEAD_LEN;

	// 消息头
	ACE_OS::memcpy(buff+pos,&FRAM_TAG_HEAD,FRAME_TAG__LEN);
	pos += FRAME_TAG__LEN;

	// 消息类型
	ACE_OS::memcpy(buff+pos,&msgtype,DATA_TYPE_LEN);
	pos += DATA_TYPE_LEN;

	// 消息内容
	ACE_OS::memcpy(buff+pos,data.c_str(),data.length());
	pos += data.length();

	// 消息尾
	ACE_OS::memcpy(buff+pos,&FRAME_TAG_END,FRAME_TAG__LEN);
	pos += FRAME_TAG__LEN;

	return buff;
}

sClientMsg* PacketParser::decoderS(char* data,int datalength)
{
	return 0;
}

char* PacketParser::encoderS(char* data,int datalength,int &outlengh)
{
	return 0;
}