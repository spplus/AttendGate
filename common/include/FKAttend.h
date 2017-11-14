/************************************************************************
 *                                                                      *
 *  Program : FKAttend.h                                                *
 *                                                                      *
 *  Purpose : Interface Function For PEFIS Fingerkeeper 6.0 family.     *
 *                                                                      *
 *  Compile : Microsoft Visual C++ .NET                                 *
 *                                                                      *
 *  Version : 2.40                                                      *
 *                                                                      *
 *  Create  : 2008/03/12 Wednesday        -Ver1.00-    By PEFIS         *
 *  Update  : 2008/04/22 Tuesday          -Ver1.01-    By PEFIS         *
 *  Update  : 2008/05/05 Monday           -Ver2.00-    By PEFIS         *
 *  Update  : 2008/05/19 Monday           -Ver2.10-    By PEFIS         *
 *  Update  : 2008/05/28 Monday           -Ver2.20-    By PEFIS         *
 *  Update  : 2008/07/22 Tuesday          -Ver2.40-    By PEFIS         *
 *  Update  : 2008/09/22 Monday           -Ver2.43-    By PEFIS         *
 *                                                                      *
 *  Copyright (C) 2008  PEFIS Electronic Technology Co.,Ltd.            *
 *     All Rights Reserved.                                             *
 *                                                                      *
 ************************************************************************/
//#include "FKDefine.h"

#ifndef _INC_FKATTENDINFO
#define _INC_FKATTENDINFO

/******************************************************************/
/*                            Function                            */
/******************************************************************/
#ifdef _FKATTEND
	#define FP_EXPORT __declspec(dllexport) APIENTRY
#else
	#define FP_EXPORT __declspec(dllimport) APIENTRY
#endif

long   FP_EXPORT FK_ConnectComm(long nMachineNo, long nComPort, long nBaudRate,	char *pstrTelNumber, long nWaitDialTime, long nLicense, long nComTimeOut);
long   FP_EXPORT FK_ConnectNet(long nMachineNo, char * pstrIpAddress, long nNetPort, long nTimeOut, long nProtocolType, long nNetPassword, long nLicense);
long   FP_EXPORT FK_ConnectUSB(long nMachineNo, long nLicense);
void   FP_EXPORT FK_DisConnect(long nHandleIndex);
long   FP_EXPORT FK_GetLastError(long nHandleIndex);
long   FP_EXPORT FK_EnableDevice(long nHandleIndex, unsigned char nEnableFlag);
void   FP_EXPORT FK_PowerOnAllDevice(long nHandleIndex);
long   FP_EXPORT FK_PowerOffDevice(long nHandleIndex);
long   FP_EXPORT FK_GetDeviceStatus(long nHandleIndex, long nStatusIndex, long *pnValue);
long   FP_EXPORT FK_GetDeviceTime(long nHandleIndex, DATE * pnDateTime);
long   FP_EXPORT FK_SetDeviceTime(long nHandleIndex, DATE nDateTime);
long   FP_EXPORT FK_GetDeviceInfo(long nHandleIndex, long nInfoIndex, long *pnValue);
long   FP_EXPORT FK_SetDeviceInfo(long nHandleIndex, long nInfoIndex, long nValue);
long   FP_EXPORT FK_GetProductData(long nHandleIndex, long nDataIndex, char *pstrValue);
long   FP_EXPORT FK_LoadSuperLogData(long nHandleIndex, long nReadMark);
long   FP_EXPORT FK_USBLoadSuperLogDataFromFile(long nHandleIndex, char *astrFilePath);
long   FP_EXPORT FK_GetSuperLogData(long nHandleIndex, long *pnSEnrollNumber, long *pnGEnrollNumber, long *pnManipulation, long *pnBackupNumber, DATE *pnDateTime);
long   FP_EXPORT FK_EmptySuperLogData(long nHandleIndex);
long   FP_EXPORT FK_LoadGeneralLogData(long nHandleIndex, long nReadMark);
long   FP_EXPORT FK_USBLoadGeneralLogDataFromFile(long nHandleIndex, char * pstrFilePath);
long   FP_EXPORT FK_GetGeneralLogData(long nHandleIndex, long * pnEnrollNumber, long *pnVerifyMode, long *pnInOutMode, DATE *pnDateTime);
long   FP_EXPORT FK_EmptyGeneralLogData(long nHandleIndex);
long   FP_EXPORT FK_GetBellTime(long nHandleIndex, long * pnBellCount, long * ptBellInfo);
long   FP_EXPORT FK_SetBellTime(long nHandleIndex, long nBellCount, long * ptBellInfo);
long   FP_EXPORT FK_GetEnrollData(long nHandleIndex, long nEnrollNumber, long nBackupNumber, long * pnMachinePrivilege, void * pnEnrollData, long * pnPassWord);
long   FP_EXPORT FK_PutEnrollData(long nHandleIndex, long nEnrollNumber, long nBackupNumber, long nMachinePrivilege, void * pnEnrollData, long nPassWord);
long   FP_EXPORT FK_SaveEnrollData(long nHandleIndex);
long   FP_EXPORT FK_DeleteEnrollData(long nHandleIndex, long nEnrollNumber, long nBackupNumber);
long   FP_EXPORT FK_USBReadAllEnrollDataFromFile(long nHandleIndex, char * pstrFilePath);
long   FP_EXPORT FK_USBReadAllEnrollDataCount(long nHandleIndex, long * pnValue);
long   FP_EXPORT FK_USBGetOneEnrollData(long nHandleIndex, long * pnEnrollNumber, long * pnBackupNumber, long * pnMachinePrivilege, void * pnEnrollData, long * pnPassWord, long * pnEnableFlag, BSTR * dwEnrollName);
long   FP_EXPORT FK_USBSetOneEnrollData(long nHandleIndex, long nEnrollNumber, long nBackupNumber,long nMachinePrivilege, void * pnEnrollData, long nPassWord, long nEnableFlag, char *dwEnrollName);
long   FP_EXPORT FK_USBWriteAllEnrollDataToFile(long nHandleIndex, char * pstrFilePath);
long   FP_EXPORT FK_EnableUser(long nHandleIndex, long nEnrollNumber, long nBackupNumber, long nEnableFlag);
long   FP_EXPORT FK_ModifyPrivilege(long nHandleIndex, long nEnrollNumber, long nBackupNumber, long nMachinePrivilege);
long   FP_EXPORT FK_ReadAllUserID(long nHandleIndex);
long   FP_EXPORT FK_GetAllUserID(long nHandleIndex, long * pnEnrollNumber, long * pnBackupNumber, long * pnMachinePrivilege, long * pnEnableFlag);
long   FP_EXPORT FK_EmptyEnrollData(long nHandleIndex);
long   FP_EXPORT FK_ClearKeeperData(long nHandleIndex);
long   FP_EXPORT FK_GetUserName(long nHandleIndex, long nEnrollNumber, BSTR* pstrUserName);
long   FP_EXPORT FK_SetUserName(long nHandleIndex, long nEnrollNumber, char *pstrUserName);
long   FP_EXPORT FK_GetNewsMessage(long nHandleIndex, long nNewsId, BSTR *pstrNews);
long   FP_EXPORT FK_SetNewsMessage(long nHandleIndex, long nNewsId, char * pstrNews);
long   FP_EXPORT FK_GetUserNewsID(long nHandleIndex, long nEnrollNumber, long * pnNewsId); 
long   FP_EXPORT FK_SetUserNewsID(long nHandleIndex, long nEnrollNumber, long nNewsId);
long   FP_EXPORT FK_GetDeviceVersion(long nHandleIndex, long *pnVersion);
long   FP_EXPORT FK_GetEnrollDataWithString(long nHandleIndex, long nEnrollNumber, long nBackupNumber, long * pnMachinePrivilege, BSTR *apstrEnrollData);
long   FP_EXPORT FK_PutEnrollDataWithString(long nHandleIndex, long nEnrollNumber, long nBackupNumber, long nMachinePrivilege, char *apstrEnrollData);
long   FP_EXPORT FK_USBGetOneEnrollDataWithString(long nHandleIndex, long * pnEnrollNumber, long * pnBackupNumber, long * pnMachinePrivilege, BSTR *apstrEnrollData, long * pnEnableFlag, BSTR *dwEnrollName);
long   FP_EXPORT FK_USBSetOneEnrollDataWithString(long nHandleIndex, long nEnrollNumber, long nBackupNumber,long nMachinePrivilege, char *apstrEnrollData, long nEnableFlag, char *dwEnrollName);
long   FP_EXPORT FK_GetBellTimeWithString(long nHandleIndex, long *pnBellCount, BSTR *apstrBellInfo);
long   FP_EXPORT FK_SetBellTimeWithString(long nHandleIndex, long nBellCount, char *apstrBellInfo);
	//---- Insert   2008/04/22 12:00:00 PM -line 10-  By PEFIS ---- 
long   FP_EXPORT FK_GetDoorStatus(long nHandleIndex, long *apnStatusVal);
long   FP_EXPORT FK_SetDoorStatus(long nHandleIndex, long anStatusVal);
long   FP_EXPORT FK_GetPassTime(long nHandleIndex, long anPassTimeID, long *apnPassTime, long anPassTimeSize);
long   FP_EXPORT FK_SetPassTime(long nHandleIndex, long anPassTimeID, long *apnPassTime, long anPassTimeSize);
long   FP_EXPORT FK_GetUserPassTime(long nHandleIndex, long anEnrollNumber, long *apnGroupID, long *apnPassTimeID, long anPassTimeIDSize);
long   FP_EXPORT FK_SetUserPassTime(long nHandleIndex, long anEnrollNumber, long anGroupID, long *apnPassTimeID, long anPassTimeIDSize);
long   FP_EXPORT FK_GetGroupPassTime(long nHandleIndex, long anGroupID, long *apnPassTimeID, long anPassTimeIDSize);
long   FP_EXPORT FK_SetGroupPassTime(long nHandleIndex, long anGroupID, long *apnPassTimeID, long anPassTimeIDSize);
long   FP_EXPORT FK_GetGroupMatch(long nHandleIndex, long *apnGroupMatch, long anGroupMatchSize);
long   FP_EXPORT FK_SetGroupMatch(long nHandleIndex, long *apnGroupMatch, long anGroupMatchSize);
	//---- Insert   2008/05/05 12:00:00 PM -line 8-  By PEFIS ---- 
long   FP_EXPORT FK_GetPassTimeWithString(long nHandleIndex, long anPassTimeID, BSTR *apstrPassTime);
long   FP_EXPORT FK_SetPassTimeWithString(long nHandleIndex, long anPassTimeID, char *apstrPassTime);
long   FP_EXPORT FK_GetUserPassTimeWithString(long nHandleIndex, long anEnrollNumber, long *apnGroupID, BSTR *apstrPassTimeID);
long   FP_EXPORT FK_SetUserPassTimeWithString(long nHandleIndex, long anEnrollNumber, long anGroupID, char *apstrPassTimeID);
long   FP_EXPORT FK_GetGroupPassTimeWithString(long nHandleIndex, long anGroupID, BSTR *apstrPassTimeID);
long   FP_EXPORT FK_SetGroupPassTimeWithString(long nHandleIndex, long anGroupID, char *apstrPassTimeID);
long   FP_EXPORT FK_GetGroupMatchWithString(long nHandleIndex, BSTR *apstrGroupMatch);
long   FP_EXPORT FK_SetGroupMatchWithString(long nHandleIndex, char *apstrGroupMatch);
	//---- Insert   2008/09/22 12:00:00 PM -line 1-  By PEFIS ---- 
long   FP_EXPORT FK_BenumbAllManager(long nHandleIndex);
//---- Insert   2009/02/18 12:00:00 PM -line 1-  By PEFIS_CCH ---- 
long   FP_EXPORT FK_ConnectGetIP(BSTR* strComName);
//---- Insert   2009/03/02 12:00:00 PM -line 1-  By PEFIS_CCH ---- 
long   FP_EXPORT FK_GetGeneralLogData_1(long nHandleIndex,long *apnEnrollNumber, long *apnVerifyMode, long *apnInOutMode,
									   long *apnYear,long *apnMonth,long *apnDay,
									   long *apnHour,long *apnMinute,long *apnSec);
long   FP_EXPORT FK_GetSuperLogData_1(long nHandleIndex,long *apnSEnrollNumber, long *apnGEnrollNumber, long *apnManipulation, long *apnBackupNumber,
		                            long *apnYear,long *apnMonth,long *apnDay,
									long *apnHour,long *apnMinute,long *apnSec);
long   FP_EXPORT FK_GetDeviceTime_1(long nHandleIndex,long *apnYear,long *apnMonth,long *apnDay,long *apnHour,long *apnMinute,long *apnSec,long *apnDayOfWeek);
long   FP_EXPORT FK_SetDeviceTime_1(long nHandleIndex,long anYear,long anMonth,long anDay,long anHour,long anMinute,long anSec,long anDayOfWeek);
//---- Insert   2009/05/04 17:21:00 PM -line 1-  By PEFIS_CCH ---- 
long   FP_EXPORT FK_GetAdjustInfo(long nHandleIndex,
								  long* dwAdjustedState,long* dwAdjustedMonth,long* dwAdjustedDay,long* dwAdjustedHour,long* dwAdjustedMinute,
								  long* dwRestoredState,long* dwRestoredMonth,long* dwRestoredDay,long* dwRestoredHour,long* dwRestoredMinte );
long   FP_EXPORT FK_SetAdjustInfo(long nHandleIndex,
								  long dwAdjustedState,long dwAdjustedMonth,long dwAdjustedDay,long dwAdjustedHour,long dwAdjustedMinute,
								  long dwRestoredState,long dwRestoredMonth,long dwRestoredDay,long dwRestoredHour,long dwRestoredMinte );
//---- Insert   2009/06/23 17:21:00 PM -line 1-  By PEFIS_CCH ---- 
long	FP_EXPORT FK_GetVerifyMode(long nHandleIndex, long anEnrollNumber, long *apnVerifyMode);
long	FP_EXPORT FK_SetVerifyMode(long nHandleIndex, long anEnrollNumber, long anVerifyMode);
//---- Insert   2009/06/24 08:00:00 PM -line 1-  By PEFIS_CCH ---- 
long	FP_EXPORT FK_USBGetOneEnrollData_1(long *apnEnrollNumber, long *apnBackupNumber,long *apnVerifyMode, long *apnMachinePrivilege, long *apnEnrollData, long *apnPassWord, long *apnEnableFlag, BSTR *dwEnrollName);
long	FP_EXPORT FK_USBSetOneEnrollData_1(long anEnrollNumber, long anBackupNumber, long anVerifyMode, long anMachinePrivilege, long *apnEnrollData, long anPassWord, long anEnableFlag, LPCTSTR dwEnrollName);
long	FP_EXPORT FK_USBGetOneEnrollDataWithString_1(long* apnEnrollNumber, long *apnBackupNumber, long *apnVerifyMode, long *apnMachinePrivilege, BSTR *apstrEnrollData, long *apnEnableFlag,BSTR * dwEnrollName);
long	FP_EXPORT FK_USBSetOneEnrollDataWithString_1(long anEnrollNumber, long anBackupNumber, long anVerifyMode, long anMachinePrivilege, LPCTSTR apstrEnrollData, long anEnableFlag,LPCTSTR dwEnrollName);


//---- Insert   2009/12/22 10:21:00 PM -line 1-  By PEFIS_CCH ---- 
long   FP_EXPORT FK_USBReadAllEnrollDataFromFile_Color(long nHandleIndex, LPCTSTR apstrFilePath);
long   FP_EXPORT FK_USBWriteAllEnrollDataToFile_Color(long nHandleIndex, LPCTSTR apstrFilePath, long anNewsKind);
long   FP_EXPORT FK_USBGetOneEnrollData_Color(long nHandleIndex, long *apnEnrollNumber, long *apnBackupNumber,long *apnMachinePrivilege, long *apnEnrollData, long *apnPassWord, long *apnEnableFlag, BSTR *dwEnrollName, long anNewsKind);
long   FP_EXPORT FK_USBSetOneEnrollData_Color(long nHandleIndex, long anEnrollNumber, long anBackupNumber, long anMachinePrivilege, long *apnEnrollData, long anPassWord, long anEnableFlag, LPCTSTR dwEnrollName, long anNewsKind);
long   FP_EXPORT FK_USBGetOneEnrollDataWithString_Color(long nHandleIndex, long* apnEnrollNumber, long *apnBackupNumber, long *apnMachinePrivilege, BSTR *apstrEnrollData, long *apnEnableFlag,BSTR * dwEnrollName, long anNewsKind);
long   FP_EXPORT FK_USBSetOneEnrollDataWithString_Color(long nHandleIndex, long anEnrollNumber, long anBackupNumber,long anMachinePrivilege, LPCTSTR apstrEnrollData, long anEnableFlag,LPCTSTR dwEnrollName, long anNewsKind);

//---- Insert   2010/01/07 17:21:00 PM -line 1-  By PEFIS_CCH ---- 
long	FP_EXPORT FK_GetAccessTime(long nHandleIndex, long anEnrollNumber, long *apnAccessTime);
long	FP_EXPORT FK_SetAccessTime(long nHandleIndex, long anEnrollNumber, long anAccessTime);

//---- Insert   2010/01/27 17:00:00 By _CCH ---- 
long	FP_EXPORT FK_SetFontName(long nHandleIndex, LPCTSTR aStrFontName, long anFontType);

//---- Insert   2010/02/01 17:21:00 By CCH ---- 
long	FP_EXPORT FK_GetRealTimeInfo(long nHandleIndex, long* apGetRealTime);
long	FP_EXPORT FK_SetRealTimeInfo(long nHandleIndex, long* apSetRealTime);

//---- Insert   2010/03/08 17:21:00 By CCH ---- 
long	FP_EXPORT FK_GetServerNetInfo(long nHandleIndex, BSTR* astrServerIPAddress, long* apServerPort, long* apServerRequest);
long	FP_EXPORT FK_SetServerNetInfo(long nHandleIndex, char*  astrServerIPAddress, long  anServerPort, long  anServerRequest);

//---- Insert   2010/06/08 17:21:00 By CCH ----
long	FP_EXPORT FK_SetUSBModel(long nHandleIndex, long anModel);


#endif //!_INC_FKATTENDINFO
/************************************************************************
 *                                                                      *
 *                        End of file : FKAttend.h                      *
 *                                                                      *
 ************************************************************************/