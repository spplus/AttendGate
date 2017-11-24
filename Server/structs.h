/************************************************************************/
/* 
	DESC:	定义结构体.
	DATE:	2017-11-20
	AUTHOR:YUANLS
*/
/************************************************************************/

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct sClientMsg 
{
	unsigned int	connectId;		// 连接ID
	unsigned int	length;				// 消息长度
	unsigned int	type;				// 消息类型
	char*				data;					// 消息内容

	sClientMsg()
	{
		connectId = 0;
		length = 0;
		type = 0;
		data = 0;
	}
	~sClientMsg()
	{
		if (data != 0)
		{
			delete []data;
		}
	}
};

#endif