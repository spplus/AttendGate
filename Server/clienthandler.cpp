
#include "ace/OS_NS_unistd.h"
#include "ace/Time_Value.h"
#include "ace/Date_Time.h"
#include "ace/Addr.h"

#include "defines.h"
#include "clienthandler.h"
#include "clientmgr.h"
#include "clientmsgservice.h"


int ClientHandler::open(void*p)
{
	int nRet = Super::open();
	if(nRet != 0)
	{
		return -1;
	}
	ACE_INET_Addr svraddr;

	//获得远程链接地址和端口
	if(this->peer().get_remote_addr(svraddr) == -1)
	{
		LOG->warn(" get_remote_addr failed!");
		return -1;
	}

	LOG->message("new client connection：%s",svraddr.get_host_addr());

	// 保存客户端连接
	m_connectId = App_ClientMgr::instance()->add(this);

	return 0;
}

int ClientHandler::handle_input(ACE_HANDLE fd )
{
 	ACE_Time_Value     nowait(RECV_TIMEOUT);
	if(fd == ACE_INVALID_HANDLE)
	{
		return -1;
	}
	char buff[FRAME_HEAD_LEN];
	int len = peer().recv(buff,FRAME_HEAD_LEN,&nowait);
	if (len <= 0)
	{
		LOG->message("recv length 0,connection closed.");
		return -1;
	}
	if (len != FRAME_HEAD_LEN)
	{
		LOG->warn("invalid frame head len:%d",len);
		return 0;
	}

	// 解析出数据包长度
	unsigned long int plen = 0;
	ACE_OS::memcpy(&plen,buff,FRAME_HEAD_LEN);

	
		// 判断数据包长度是否非法
		if (plen > MAX_PACKET_LEN || plen <0)
		{
			LOG->warn("invalid packet length:%d",plen);
			return 0;
		}
		// 接收包体内容
		ACE_Message_Block* mb = new ACE_Message_Block(plen);

		// 总接收长度
		int total = 0;

		// 继续读取包体内容
		int dlen = peer().recv(mb->wr_ptr(),plen,&nowait);
		if (dlen <= 0)
		{
			LOG->warn("recv packet failed,recived :%d",dlen);
			mb->release();
			return -1;
		}
		total = dlen;
		mb->wr_ptr(dlen);

		// 如果短读，则继续读取。
		while(total < plen)
		{
			ACE_DEBUG((LM_DEBUG,"[%D]total:%d,this length:%d,read:%d.\n",plen,dlen,total));
			dlen = peer().recv(mb->wr_ptr(),plen-total,&nowait);
			if (dlen<=0)
			{
				mb->release();
				return -1;
			}
			mb->wr_ptr(dlen);
			total += dlen;
		}

		// 接收完整包，投递到数据处理队列
		if (total == plen)
		{
			// 把消息标记上连接ID,以业务层处理完可以原路返回
			mb->msg_type(m_connectId);
			if(App_CMService::instance()->putq(mb) != -1)
			{
				LOG->debug("put one packet to message queue.");
			}
		}
		else
		{
			mb->release();

			// 接收包长度错误
			LOG->error("Invalid packet length.packet length %d,recive %d.",plen,total);
		}
	
	
	return 0;
}

int ClientHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
	if (mask == ACE_Event_Handler::WRITE_MASK)
	{
		return 0;
	}
	else
	{
		ACE_TCHAR peer_name[64];
		ACE_INET_Addr peer_addr;
		if (this->peer().get_remote_addr(peer_addr) == 0 && peer_addr.addr_to_string (peer_name, 64) == 0)
		{
			ACE_DEBUG((LM_DEBUG,"[%D]Connection closed:%s.\n",peer_name));
			LOG->debug("Connection closed:%s.",peer_name);
		}

		// 释放队列资源
		this->wait();

		// 关闭链路
		this->peer().close_reader();
		this->peer().close_writer();
		this->peer().close();

		this->closing_ = true;

		// 删除客户端连接对象
		App_ClientMgr::instance()->del(m_connectId);

		// 连接断开，通知工作线程退出
		/*ACE_Message_Block* mb = new ACE_Message_Block(1);
		mb->msg_type(ACE_Message_Block::MB_STOP);
		m_checktsk->put(mb);*/
	
		return Super::handle_close (h, mask);
	}
	return 0;
}

bool ClientHandler::SendData(const char* data,int length)
{
	ACE_Time_Value     nowait(SEND_TIMEOUT);

	if(NULL == data )
	{
		return false;
	}

	if(get_handle() == ACE_INVALID_HANDLE)
	{
		delete []data;
		return false;
	}

	const char* pData = data;
	
	//发送数据的总长度
	int nSendLen = length;

	int nIsSendSize = 0;

	//循环发送，直到数据发送完成。
	while(nIsSendSize < nSendLen)
	{

		int nDataLen = this->peer().send(data+nIsSendSize, nSendLen, &nowait);
		int nErr = ACE_OS::last_error();
		if(nDataLen <= 0)
		{
			if(nErr == EWOULDBLOCK)
			{
				//如果发送堵塞，则等10毫秒后再发送。
				ACE_Time_Value tvSleep(0, 10 * 1000);
				ACE_OS::sleep(tvSleep);
				continue;
			}
			delete []data;
			return false;
		}
		// 一次性发送完一包数据
		else if(nDataLen >= nSendLen)   
		{
			delete []data;
			return true;
		}
		// 分多次发送
		else
		{
			nIsSendSize  += nDataLen;
			nSendLen -= nDataLen;

			// 多次发送完毕
			if (nIsSendSize == nSendLen)
			{
				delete []data;
				return true;
			}
		}
	}
	return true;
}

void ClientHandler::SendBack()
{
	ACE_Date_Time tvTime(ACE_OS::gettimeofday()); 
	std::string ret;
	char temp[32];
	sprintf(temp,"%d-%02d-%02d %d:%d:%d %d",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second(),tvTime.microsec());
	ret.append(temp);

	int len = ret.length();
	ACE_Message_Block* mb = new ACE_Message_Block(len+4);
	
	ACE_OS::memcpy(mb->wr_ptr(),&len,4);
	mb->wr_ptr(4);

	ACE_OS::memcpy(mb->wr_ptr(),ret.c_str(),len);
	mb->wr_ptr(len);

	SendData(mb->rd_ptr(),mb->length());

}
