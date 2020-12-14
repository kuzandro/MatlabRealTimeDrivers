/* For driver ICP_DIO  24  DI, 24 DO */
/*  30/05/2019  for Simulink Desktop Real Time External Mode */
#include  <stdio.h>
#include  <stdlib.h>
#include  <dos.h>
#include  <conio.h>
#include  <math.h>

#define   BYTE      unsigned char
#define   UCHAR     unsigned char
#define   WORD      unsigned short
#define   DWORD     unsigned long

#define   NoError                0
#define   FindBoardError         1
#define   DriverHandleError      10
#define   DriverCallError        20
#define   TimeOut                40
#define   ExceedBoardNumber      50
#define   NotFoundBoard          60


/*------------------------------------------
 
PISO-813           TIGER100         80   0A      00     
PISO-813           TIGER320       4280   02      00     
PISO-725           TIGER100         80   0C      00     
PISO-725           TIGER320       0380   00      FF     
PISO-PS400     TIGER320   0B89   03  FF
PISO-PS200     TIGER320   0B89   02  FF
-----------------------------------------------*/

#ifdef __cplusplus 
extern "C" {
#endif

WORD   DIO24_GetDriverVersion(WORD *wDriverVersion, WORD *wSubVenderID, WORD *wSubDevice);
WORD   DIO24_DriverInit(WORD *wBoards, DWORD wSubVenderID, WORD wSubDeviceID,BYTE wSubAuxID);
WORD   DIO24_DriverClose(void);
WORD   DIO24_Enable(WORD wBoard);
WORD   DIO24_Diable(WORD wBoard);
WORD   DIO24_ConfigPorts(WORD wBoard, WORD wConf);
WORD   DIO24_Out(WORD wBoard,WORD Addr,WORD Out);
WORD   DIO24_Read(WORD wBoard,WORD Addr,WORD *In); /* In - int */
WORD   Pci_SearchDIO24(WORD SubVendorID, WORD SubDeviceID, WORD SubAuxID);
void   Pci_WriteConfigAddress(BYTE bBus, BYTE bDevice, BYTE bFunction, BYTE bCfgIndex);
DWORD  Pci_ReadConfigData(WORD *dwCFC, WORD *dwCFE);




#ifdef __cplusplus 
}
#endif


/*************** for Microsoft C Compiler ****************/
#ifdef _MSC_VER

/* MSC inp returns byte, inpw returns word */
/* TC  inportb returns byte, inport returns word */
/* Now, it is unified to use inpb, inpw, inpd, outpb, outpw, outpd */

#define enable           _enable
#define disable          _disable
#define setvect          _dos_setvect
#define sleep            delay1
#define delay            delay1

#ifdef __cplusplus 
extern "C" {
#endif

#ifdef __cplusplus 
}
#endif

#else     // for BC and TC

/* Now, it is unified to use inpb, inpw, inpd, outpb, outpw, outpd */
/*#define inp          inportb
#define inpw         inport */
#define inpb         _inpw
#define inpd         _inpd
/*#define outp         outportb
#define outpw        outport */
#define outpb        _outpw
#define outpd        _outpd

#endif
/*************** for Microsoft C Compiler ****************/

