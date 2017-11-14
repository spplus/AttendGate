
/******************************************************************/
/*                            Constant                            */
/******************************************************************/
//                Protocol Type                //
#define PROTOCOL_TCPIP   0               // TCP/IP
#define PROTOCOL_UDP   1                 // UDP

//				Enroll information write in Database 
#define ENROLL_SAVEOK		1
#define ENROLL_SAVEDOUBLE   2

#define	NEWS_EXTEND			2
#define	NEWS_STANDARD		1

//                Backup Number Constant                //
#define BACKUP_FP_0   0                  // Finger 0
#define BACKUP_FP_1   1                  // Finger 1
#define BACKUP_FP_2   2                  // Finger 2
#define BACKUP_FP_3   3                  // Finger 3
#define BACKUP_FP_4   4                  // Finger 4
#define BACKUP_FP_5   5                  // Finger 5
#define BACKUP_FP_6   6                  // Finger 6
#define BACKUP_FP_7   7                  // Finger 7
#define BACKUP_FP_8   8                  // Finger 8
#define BACKUP_FP_9   9                  // Finger 9
#define BACKUP_PSW   10                  // Password
#define BACKUP_CARD   11                 // Card

////                Manipulation of SuperLogData                //
#define LOG_ENROLL_USER   3              // Enroll-User
#define LOG_ENROLL_MANAGER   4           // Enroll-Manager
#define LOG_ENROLL_DELFP   5             // FP Delete
#define LOG_ENROLL_DELPASS   6           // Pass Delete
#define LOG_ENROLL_DELCARD   7           // Card Delete
#define LOG_LOG_ALLDEL   8               // LogAll Delete
#define LOG_SETUP_SYS   9                // Setup Sys
#define LOG_SETUP_TIME   10              // Setup Time
#define LOG_SETUP_LOG   11               // Setup Log
#define LOG_SETUP_COMM   12              // Setup Comm
#define LOG_PASSTIME   13                // Pass Time Set
#define LOG_SETUP_DOOR   14              // Door Set Log

////                VerifyMode of GeneralLogData                //
#define LOG_FPVERIFY1   1                 // Fp Verify
#define LOG_PASSVERIFY1   2               // Pass Verify
#define LOG_CARDVERIFY1  3               // Card Verify
#define LOG_FPPASS_VERIFY1   4            // Pass+Fp Verify
#define LOG_FPCARD_VERIFY1   5            // Card+Fp Verify
#define LOG_PASSFP_VERIFY1   6            // Pass+Fp Verify
#define LOG_CARDFP_VERIFY1   7            // Card+Fp Verify
#define LOG_JOB_NO_VERIFY1   8            // Job number Verify
#define LOG_CARDPASS_VERIFY1   9          // Card+Pass Verify
#define LOG_CLOSE_DOOR1   10              // Door Close
#define LOG_OPEN_HAND1   11               // Hand Open
#define LOG_PROG_OPEN1   12               // Open by PC
#define LOG_PROG_CLOSE1   13              // Close by PC
#define LOG_OPEN_IREGAL1   14             // Iregal Open
#define LOG_CLOSE_IREGAL1   15            // Iregal Close
#define LOG_OPEN_COVER1   16              // Cover Open
#define LOG_CLOSE_COVER1   17             // Cover Close

#define LOG_OPEN_DOOR1   32               // Door Open
#define LOG_OPEN_THREAT1   48             // Door Open as threat
////                IOMode of GeneralLogData                //
#define LOG_IOMODE_IN1   0
#define LOG_IOMODE_OUT1   1
#define LOG_IOMODE_OVER_IN1   2    //   LOG_IOMODE_IO
#define LOG_IOMODE_OVER_OUT1   3

////                Machine Privilege                //
#define MP_NONE   0                      // General user
#define MP_ALL   1                       // Manager

//                Index of  GetDeviceStatus                //
#define GET_MANAGERS   1
#define GET_USERS   2
#define GET_FPS   3
#define GET_PSWS   4
#define GET_SLOGS   5
#define GET_GLOGS   6
#define GET_ASLOGS   7
#define GET_AGLOGS   8
#define GET_CARDS   9

//                Index of  GetDeviceInfo                //
#define DI_MANAGERS   1                  // Numbers of Manager
#define DI_MACHINENUM   2                // Device ID
#define DI_LANGAUGE   3                  // Language
#define DI_POWEROFF_TIME   4             // Auto-PowerOff Time
#define DI_LOCK_CTRL   5                 // Lock Control
#define DI_GLOG_WARNING   6              // General-Log Warning
#define DI_SLOG_WARNING   7              // Super-Log Warning
#define DI_VERIFY_INTERVALS   8          // Verify Interval Time
#define DI_RSCOM_BPS   9                 // Comm Buadrate
#define DI_DATE_SEPARATE   10            // Date Separate Symbol
#define DI_VERIFY_KIND   24              // Verify Kind Symbol

////                Baudrate   value of DI_RSCOM_BPS                //
#define BPS_9600   3
#define BPS_19200   4
#define BPS_38400   5
#define BPS_57600   6
#define BPS_115200   7

//                Product Data Index                //
#define PRODUCT_SERIALNUMBER   1    // Serial Number
#define PRODUCT_BACKUPNUMBER   2    // Backup Number
#define PRODUCT_CODE   3            // Product code
#define PRODUCT_NAME   4            // Product name
#define PRODUCT_WEB   5             // Product web
#define PRODUCT_DATE   6            // Product date
#define PRODUCT_SENDTO   7          // Product sendto

//                Door Status                //
#define DOOR_CONROLRESET   0
#define DOOR_OPEND   1
#define DOOR_CLOSED   2
#define DOOR_COMMNAD   3

//                Error code                //
#define RUN_SUCCESS   1
#define RUNERR_NOSUPPORT   0
#define RUNERR_UNKNOWNERROR   -1
#define RUNERR_NO_OPEN_COMM   -2
#define RUNERR_WRITE_FAIL   -3
#define RUNERR_READ_FAIL   -4
#define RUNERR_INVALID_PARAM   -5
#define RUNERR_NON_CARRYOUT   -6
#define RUNERR_DATAARRAY_END   -7
#define RUNERR_DATAARRAY_NONE   -8
#define RUNERR_MEMORY   -9
#define RUNERR_MIS_PASSWORD   -10
#define RUNERR_MEMORYOVER   -11
#define RUNERR_DATADOUBLE   -12
#define RUNERR_MANAGEROVER   -14
#define RUNERR_FPDATAVERSION   -15


#define FK625_FP1000	2001
#define FK625_FP2000	2002
#define FK625_FP3000	2003
#define FK625_FP5000	2004
#define FK625_FP10000	2005
#define FK625_FP30000	2006
#define FK625_ID30000	2007

#define FK635_FP700		3001
#define FK635_FP3000	3002
#define FK635_FP10000	3003
#define FK635_ID30000	3004

#define FK723_FP1000	4001

#define FK725_FP1000	5001
#define FK725_FP1500	5002
#define FK725_ID5000	5003
#define FK725_ID30000	5004

#define FK735_FP500		6001
#define FK735_FP3000	6002
#define FK735_ID30000	6003

#define FK925_FP3000	7001

#define FK935_FP3000	8001


#define gstrNoDevice	"No Device"

#define	MULTIPROC( )											\
{ MSG msg;														\
	while ( PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {				\
	TranslateMessage(&msg);										\
	DispatchMessage(&msg);										\
	}															\
}