/* Declaration of PciDevice, a structure used to pass information about */
/* installed CEI boards from the xPC environment to the CEI-x20 API. */

/*  Copyright 2004-2014 The MathWorks, Inc. */

#ifndef __DEVICEINFO_H__
  #define __DEVICEINFO_H__
  #include <stdio.h>
  #define MAX_CEI_BOARD_ID 16
 
  typedef struct PciDeviceStruct {
    unsigned long BaseAddress[6];
    unsigned long VirtAddress[6];
    unsigned long Length[6];
    unsigned short AddressSpaceIndicator[6];
    unsigned short MemoryType[6];
    unsigned short Prefetchable[6];
    unsigned short InterruptLine;
    unsigned short VendorId;
    unsigned short DeviceId;
    unsigned short SubDeviceId;
    unsigned short SubVendorId;
    unsigned short ClassCode;
    int            bus;
    int            slot;
                                 } PciDevice;

 void ShowPCIDevice( PciDevice *pdev );

 int GetPCIDeviceFull( unsigned short vendor_id,
                      unsigned short device_id,
                      unsigned short sub_vendor_id,
                      unsigned short sub_device_id,
                      int bus,
                      int slot,
                      PciDevice *pdev );

/* Get the config information for whatever board is at bus/slot. */

  int xpcGetPciConfigAtSlot( int bus,
                           int slot,
                           PciDevice  *pdev);

  void ShowPCIDevice( PciDevice *pdev );

#endif /* __DEVICEINFO_H__ */
