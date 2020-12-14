/*
 * xpctarget.h - xPC Target Header File
 *
 * Copyright 2006-2010 The MathWorks, Inc.
 * This file bi at R2015a
*/


#ifndef __XPCTARGET_H__
#define __XPCTARGET_H__
#ifdef __cplusplus
extern "C" {
#endif

 
#ifndef XPCCALLCONV
  #ifdef _MSC_VER
    #define XPCCALLCONV 
    /*__cdecl */ 
  #else
    #define XPCCALLCONV    __syscall *
    /* * __syscall */
    
  #endif
#endif


#ifndef NOT_EXTERN_FN_POINTERS
  #  define EXTERN  extern
  
#else
  #  define EXTERN
#endif

#ifdef MODEL
  #include "c:\MatlabWork\rtwtypes.h"  /* Constructed for model builds */
#else
  #include "tmwtypes.h"  /* full version for precompiled libs */
#endif
#include "c:\MatlabWork\deviceinfo.h"


/**********************************\
|*** xPC Target Data Structures ***|
\**********************************/

/*
 * For use with the PCIDeviceInfo functions this data structure maps
 * to portions of the PCI Configuration Header.
*/
#define xpcPCIDevice PciDevice   /* typedef struct xpcPCIDeviceStruct {
                                     uint32_T BaseAddress[6];
                                     uint32_T VirtAddress[6];
                                     uint32_T Length[6];
                                     uint16_T AddressSpaceIndicator[6];
                                     uint16_T MemoryType[6];
                                     uint16_T Prefetchable[6];
                                     uint16_T InterruptLine;
                                     uint16_T VendorId;
                                     uint16_T DeviceId;
                                     uint16_T SubDeviceId;
                                     uint16_T SubVendorId;
                                  } xpcPCIDevice;
                                    */
/*
 * For use with the ElapsedTime & SubtractTime functions these data structures
 * contains time represented in nanoseconds to 64 bits.
*/
typedef struct xpcTime64Struct {
    uint32_T NanoSecondsLo;   /* Bottom 32 bits of 64 bit value */
    uint32_T NanoSecondsHi;   /* Top 32 bits of 64 bit value */
} xpcTime64;

typedef union xpcTimeStruct {
    xpcTime64  U64;
    /* uint64_T NanoSeconds; */
} xpcTime;


/****************************\
|*** xPC Target Functions ***|
\****************************/

/*
 * Port I/O functions. These functions read and write data to and from I/O
 * port space.  Functions are provided to read and write 8 bit(byte), 16
 * bit(word) and 32 bit(double word) values.
*/
EXTERN uint32_T (XPCCALLCONVxpcInpDW)(uint16_T port);
EXTERN uint16_T (XPCCALLCONVxpcInpW)(uint16_T port);
EXTERN uint8_T  (XPCCALLCONVxpcInpB)(uint16_T port);

EXTERN void (XPCCALLCONVxpcOutpDW)(uint16_T port, uint32_T value);
EXTERN void (XPCCALLCONVxpcOutpW)( uint16_T port,  uint16_T value);
EXTERN void (XPCCALLCONVxpcOutpB)( uint16_T port,  uint8_T  value);

/*
 * Functions to read and display the PCI Configuration Header.
 * Note that the Show function is for debug only.
*/
#define XPC_NO_VEND_DEV   ((uint16_T) 0xffff)   /* Use when no VendorId or DeviceID is required */
#define XPC_NO_SUB        XPC_NO_VEND_DEV       /* Use when no subVendorId or subDeviceID is required */

#define XPC_NO_BUS_SLOT   ((int32_T) -1)        /* Use when not specifying a bus or slot */

EXTERN int32_T (XPCCALLCONVxpcGetPCIDeviceInfo) ( uint16_T vendorId,uint16_T deviceId,uint16_T subVendorId,uint16_T subDeviceId,
                                                   int bus, int slot,xpcPCIDevice *pciInfo);

#define  xpcGetPCIDeviceInfo   (int32_T)GetPCIDeviceFull   /* This is original codes */

#define xpcShowPCIDeviceInfo    ShowPCIDevice      /* void    xpcShowPCIDeviceInfo(xpcPCIDevice *pciInfo); */

/*
 * PCI memory mapping functions.
*/
#define XPC_RT_PG_USERREADWRITE   ((uint32_T) 0x7)   /* Access type */

EXTERN void *(XPCCALLCONVxpcReserveMemoryRegion)(const void *physical,
                             uint32_T    numBytes,
                             uint32_T    access);

/*
 * Functions to allocate and free physical memory.
 * Physical memory allocation for DMA buffers.
*/
EXTERN void * (XPCCALLCONVxpcAllocPhysicalMemory)(unsigned int numBytes);
EXTERN void   (XPCCALLCONVxpcFreePhysicalMemory)(void *mem);

/*
 * Time functions. Both GetElapsedTime & SubtractTime functions return a double precision floating point
 * value of time in seconds. If more precision is required pass a pointer to an xpcTime struct as the first
 * argument and the functions will populate the struct with nanosecond precision.
*/
EXTERN void (XPCCALLCONVxpcBusyWait)(double period);
EXTERN real_T (XPCCALLCONVxpcGetElapsedTime)(xpcTime *upTime);

real_T xpcSubtractTime(xpcTime       *time,   /* Returns time2 - time1 in seconds. Will fill in *time if not NULL. */
                       const xpcTime *time2,
                       const xpcTime *time1);

/*
 * Miscellaneous functions.
*/
EXTERN boolean_T (XPCCALLCONVxpcIsModelInit)(void);

/* Asynchronous interrupt support (async_xpcimport) */


/**
 * Since this is a void-void function, the default calling convention
 * should ok. The default calling convention for Visual is cdecl and Watcom
 * uses syscall for the ISRHandler.
 */
typedef void (*ISRHandler)(void);
typedef int (__cdecl *HookFunc)(/*xpc*/PciDevice *pdev);

/* Make sure the following two defines and the function prototypes */
/* agree with xpcasynchandlers.h in kernel code. */

#define XPC_RUN_ISR  1
#define XPC_DROP_ISR 0

/* The Prehook function will return XPC_RUN_ISR if the kernel should call */
/* ISRHandler, else XPC_DROP_ISR to indicate that this board doesn't */
/* need service. */

EXTERN int (XPCCALLCONVxpceRegisterISR)  (unsigned char IRQ, ISRHandler handler, HookFunc PreISR, HookFunc postISR, int preemptable, void *Devinfo );
EXTERN int (XPCCALLCONVxpceDeRegisterISR) (void *Devinfo);

/* Typically, Devinfo is a pointer to an xpcPCIDevice structure. */

EXTERN uint32_T ( XPCCALLCONVxpceaddEvent)(uint32_T sectionID);
void xpceuploadevent(void* pData, uint32_T numMemUnits, uint32_T sectionId);
#ifdef __cplusplus
}
#endif

#endif   /* __XPCTARGET_H__ */
