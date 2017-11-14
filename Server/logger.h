
/************************************************************************/
/* 
	DESC:	提供函数,实现格式日志打印,可选择打印屏幕,文件或两者均有,支持自动清理日志功能.
	DATE:	2016-04-20
	AUTHOR:	YUANLS
*/
/************************************************************************/



#ifndef HP_LOGGER_ACE_H_
#define HP_LOGGER_ACE_H_

#include <stdarg.h>

#include <ace/FILE_Connector.h>
#include <ace/FILE_IO.h>
#include <ace/Task.h>
#include <ace/Log_Msg.h>
#include <ace/Mutex.h>
#include <string>
using namespace std;


/*默认日志保存月份*/
#define DEFAULT_SAVED_MONTH		6

/*日志级别 - 控制日志显示的级别*/
#define HP_LOGLEVEL_ERROR		0x00				/*错误*/
#define HP_LOGLEVEL_WARN		0x01				/*警告*/
#define HP_LOGLEVEL_MESSAGE		0x02				/*提示*/
#define HP_LOGLEVEL_DEBUG		0x03				/*调试*/

/*日志媒体 - 控制日志输出的媒体*/
#define HP_LOGMEDIA_SCREEN		0x00				/*屏幕*/
#define HP_LOGMEDIA_FILE		0x01				/*文件*/
#define HP_LOGMEDIA_ALL			0x02				/*全部*/

/*日志队列状态*/
#define HP_LOGCLEANER_CLOSED	0x00				/*日志清理关*/
#define HP_LOGCLEANER_OPEN		0x01				/*日志清理开*/

/*日志文件*/
#define HP_LOG_MAX_NAMESIZE		255					/*最大文件名长度*/

#define HP_LOG_FILEDIR "log"						/*默认文件根目录*/
#define HP_LOG_FILENAME "syslog"					/*默认日志文件名*/
#define HP_LOG_FILETYPE ".log"						/*默认扩展名*/
#define HP_LOG_MAX_FILESIZE (32*1024*1024)			/*最大日志文件大小32M*/

#define HP_LOG_GRP_ID			96469
#define HP_LOG_INPUT_SIZE		(400*1024)			/*单个日志最大字节数*/
#define HP_LOG_SEND_HIGHT_MARK	(1024*1024)			/*日志队列满负荷能力*/

/*配置参数定义*/
#define HP_LOG_CONF_SECTION       "LOGGER"
#define HP_LOG_CONF_LOGFILEDIR    "LOGFILEDIR"
#define HP_LOG_CONF_LOGMEDIA      "LOGMEDIA"
#define HP_LOG_CONF_LOGLEVEL      "LOGLEVEL"
#define HP_LOG_CONF_LOGCLEANER    "LOGCLEANER"
#define HP_LOG_CONF_LOGSAVEDMONTH "LOGSAVEDMONTH"


/********************************************************************************************
* 采用ACE_Task任务或主动对象处理模式，创建日志系统
********************************************************************************************/
class Logger : public ACE_Task<ACE_MT_SYNCH>
{
public:
	Logger() : active_(false)
	{
		this->media_ = HP_LOGMEDIA_ALL;
		this->level_ = HP_LOGLEVEL_DEBUG;
		this->cleaner_switch_ = HP_LOGCLEANER_OPEN;
		this->saved_month_ = DEFAULT_SAVED_MONTH;
		ACE_OS::memset(this->parentdir_, 0x00, sizeof(this->parentdir_));
		ACE_OS::memset(this->filedir_, 0x00, sizeof(this->filedir_));
		ACE_OS::memset(this->filename_, 0x00, sizeof(this->filename_));
		ACE_OS::memset(this->fullpath_, 0x00, sizeof(this->fullpath_));
		this->msg_queue()->high_water_mark( HP_LOG_SEND_HIGHT_MARK);
	}

	virtual ~Logger()
	{
		this->active_ = false;
		this->msg_queue()->close();
		/*ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t) Logger stop/n")));*/
	}

	/*-----------------------------------------------------------------------
	* name:		初始化日志系统参数
	* input:	fdir  -- 日志文件根目录
	*			_media  -- 日志媒体
	*			_level  -- 日志级别
	*			_cleaner_switch  -- 日志清理开关
	*			_saved_month  -- 日志保存月数
	* output:	NONE
	* return:	NONE
	*-----------------------------------------------------------------------*/
	void	init_logger(const char * _fdir, unsigned int _media = HP_LOGMEDIA_ALL, unsigned int _level = HP_LOGLEVEL_DEBUG, int _cleaner_switch = HP_LOGCLEANER_CLOSED, int _saved_month = DEFAULT_SAVED_MONTH);

	/*-----------------------------------------------------------------------
	* name:		初始化日志系统
	* 			每次系统调用此日志类时 需要调用此函数初始化日志系统
	* input:	NONE
	* output:	NONE
	* return:	RT_NG -- 失败
	* 			RT_OK -- 成功
	*-----------------------------------------------------------------------*/
	int		open_logger();

	/*-----------------------------------------------------------------------
	* name:		关闭日志系统
	* input:	NONE
	* output:	NONE
	* return:	NONE
	*-----------------------------------------------------------------------*/
	void	close_logger();

	/*-----------------------------------------------------------------------
	* name:		载入配置文件
	* input:	_config_filename  -- 日志文件路径
	* output:	NONE
	* return:	RT_NG -- 失败
	* 			RT_OK -- 成功
	*-----------------------------------------------------------------------*/
	int		load_config(const char * _config_filename);

	/*-----------------------------------------------------------------------
	* name:		error日志
	* input:	fmt  -- 带格式字符串
	* 			...  -- 不定参数列表
	* output:
	* return:
	*-----------------------------------------------------------------------*/
	void	error(const char * fmt, ...);

	/*-----------------------------------------------------------------------
	* name:		warn日志
	* input:	fmt  -- 带格式字符串
	* 			...  -- 不定参数列表
	* output:
	* return:
	*-----------------------------------------------------------------------*/
	void	warn(const char * fmt, ...);

	/*-----------------------------------------------------------------------
	* name:		message日志
	* input:	fmt  -- 带格式字符串
	* 			...  -- 不定参数列表
	* output:
	* return:
	*-----------------------------------------------------------------------*/
	void	message(const char * fmt, ...);

	/*-----------------------------------------------------------------------
	* name:		debug日志
	* input:	fmt  -- 带格式字符串
	* 			...  -- 不定参数列表
	* output:
	* return:
	*-----------------------------------------------------------------------*/
	void	debug(const char * fmt, ...);

	/*-----------------------------------------------------------------------
	* name:		删除目录或文件
	* input:	_fdir  -- 目录或文件路劲
	* output:
	* return:	>0  -- 删除成功
	* 			=0  -- 目录不存在
	* 			<0  -- 删除失败
	*-----------------------------------------------------------------------*/
	int		rmdir(const char * _fdir);

	// 设置日志文件名称
	void	set_logname(const char* logname);


	void	set_active(bool b);
	bool	get_active();



private:
	private:
	/*-----------------------------------------------------------------------
	* name:		以指定格式打印，不增加其他信息
	* input:	fmt  -- 带格式字符串
	* 			...  -- 不定参数列表
	* output:
	* return:
	*-----------------------------------------------------------------------*/

	void printf(const char * fmt, ...);

	/*-----------------------------------------------------------------------
	* name:		枚举缓存
	* input:	_buff  -- 缓存
	* 			_size  -- 缓存大小
	*			_type  -- 输出类型，1：接收，2：发送，其他：无
	* output:
	* return:
	*-----------------------------------------------------------------------*/
	void dump(const char * _buf, size_t _size, int _type);

	/*-----------------------------------------------------------------------
	* name:		建立目录
	* input:	_fdir  -- 目录
	* output:
	* return:	>0  -- 新建目录
	* 			=0  -- 目录已存在
	* 			<0  -- 创建失败
	*-----------------------------------------------------------------------*/
	int mkdir(const char * _fdir);
	virtual int svc(void);

	/*-----------------------------------------------------------------------
	* name:		检测目录
	* input:	_fdir  -- 日志根目录
	* output:
	* return:	>0  -- 新建目录
	* 			=0  -- 目录无变化
	* 			<0  -- 失败
	*-----------------------------------------------------------------------*/
	int check_subdir(const char * _fdir);

	/*-----------------------------------------------------------------------
	* name:		创建一个日志文件子目录名
	* input:	_level  -- 目录级别
	* output:	_subdir  -- 子目录名字符串
	* return:
	*-----------------------------------------------------------------------*/
	void create_subdirname(char * _subdir, int _level);

	/*-----------------------------------------------------------------------
	* name:		创建日志文件名后续时间字符串
	* input:
	* output:	_filename  -- 日志文件名+后续时间字符串
	* return:
	*-----------------------------------------------------------------------*/
	void create_filename(char * _filename);

	/*-----------------------------------------------------------------------
	* name:		取日志级别
	* input:	_var_log_level  -- 日志级别
	* output:	_level  -- 日志级别
	* return:
	*-----------------------------------------------------------------------*/
	void getloglevel(int _var_log_level, char * _level);

	/*-----------------------------------------------------------------------
	* name:		取当前时间，输出长度23
	* input:
	* output:	_stime  -- 时间字符串
	* return:
	*-----------------------------------------------------------------------*/
	void currenttime(char * _stime);

	/*-----------------------------------------------------------------------
	* name:		屏幕打印
	* input:	fmt  -- 带格式字符串
	* 			
	* output:
	* return:	 -1 -- 失败
	* 			>=0 -- 输出字符数
	*-----------------------------------------------------------------------*/
	int to_screen(const char * fmt);

	/*-----------------------------------------------------------------------
	* name:		文件打印
	* input:	fmt  -- 带格式字符串
	* 			
	* output:
	* return:	 -1 -- 失败
	* 			>=0 -- 输出字符数
	*-----------------------------------------------------------------------*/
	int to_file(const char * fmt);

public:
	int media_; /*日志媒体 默认= HP_LOGMEDIA_SCREEN*/
	int level_; /*日志级别 默认= HP_LOGLEVEL_MESSAGE*/

	int cleaner_switch_; /*日志清理开关，=1时有效*/
	int saved_month_;    /*日志保存月数，=0表示不删除，要求小于120*/

	char parentdir_[HP_LOG_MAX_NAMESIZE];   /*日志文件根目录*/
	char filedir_  [HP_LOG_MAX_NAMESIZE];   /*日志文件目录, 包括根目录、子目录*/
	char filename_ [HP_LOG_MAX_NAMESIZE];   /*日志文件名,不包含目录*/
	char fullpath_ [HP_LOG_MAX_NAMESIZE*2]; /*日志文件全路径名，包括目录和文件名*/

	ACE_FILE_IO fp_;

private:
	bool		active_;
	string		logname_;

	ACE_FILE_Connector fp_conn_;
};

typedef ACE_Singleton<Logger, ACE_Mutex> App_Logger;
#endif
