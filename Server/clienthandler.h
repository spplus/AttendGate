/************************************************************************/
/* 
	DESC:	客户端连接处理器.
	DATE:	2016-04-20
	AUTHOR:	YUANLS
*/
/************************************************************************/

#ifndef __CLIENTHANDLER_H__
#define __CLIENTHANDLER_H__

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"

class ClientHandler :public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Super;
public:
	ClientHandler();
	virtual	int		open(void*);
	virtual	int		handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	virtual	int		handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);
	bool			SendData(const char* data,int length);

private: 
	void			SendBack();

private: 
	// 连接ID
	unsigned int	m_connectId;

	// 数据头，用于后续数据包长度
	ACE_Message_Block*	m_head;
};

#endif