/* For driver ICP_DIO 24  DI, 24 DO */
/*  30/05/2019  for Simulink Desktop Real Time External Mode */
/*Based on original examplels ICP_PCI_IO.h */
/* For CSI AC Drive with 8 bit Date Bus  */


#include  "csi_dio.h"

#define   MAX_BOARD_NUMBER      16

#define PCI_CONFIG_VENDEV      0
#define PCI_CONFIG_BAR0        4
#define PCI_CONFIG_BAR1        5
#define PCI_CONFIG_BAR2        6
#define PCI_CONFIG_BAR3        7
#define PCI_CONFIG_SUBVENDEV  11
#define PCI_CONFIG_INTERRUPT  15

#define   PCI826        0x10B53001
#define   PISO600       0xE1590002
#define   DIO24         0xE1590001
#define   TIGER100      0xE1590002
typedef struct
{
         DWORD VendorDevice;  // Vendor-Device ID
         DWORD SubVenDev;     // SubVendor, SubDevice ID
         BYTE  SubAux;        // SubAux
         BYTE  SVDMask;       // Mask for Sub-VendorDevice ID (First Byte)
         DWORD CompatibleID;  // Compatible ID
         char  name[20];      // Device Name
} CARD_ID;

/* Order by SVDMask: more mask off (0x1F), more below (last one)*/
const CARD_ID pcicard[]={
/*   VenDevID, Sub-VenDev, Aux, SVDMask,CompatibleID, Name */
        {PCI826,   0x21290826, 0xFF, 0xFF, 0x21290826, "PCI826U"   },
        {PISO600,  0x00810005, 0xFF, 0xFF, 0x00810005, "PISO600"   },
        {DIO24,  0x80800001, 0x40, 0xFF, 0x80800001, "DIO24"   },
    /* PISO-725, bit15 ~13 = Aux6,1,7 = Interrupt Status */
        {TIGER100,   0x80000C, 0xFF, 0xFF, 0x008008FF, "PISO-725"   },
    /*    {TIGER320, 0x03800000, 0xFF, 0x1F, 0x008008FF, "PISO-725"   }*/
                        };

WORD  TotalRecords=sizeof(pcicard)/sizeof(CARD_ID); 

WORD  wTotalBoards=0;
WORD  wGetAddress=0;

WORD  wConfigSpace[MAX_BOARD_NUMBER][12];
WORD  SearchCards(WORD wSubVendor, WORD wSubDevice, WORD wSubAux);

/* -------------------------------------------------------------------- */
/* PCI function                                                 Yes     */
/* bBus  : Bus number, 0~255                                            */
/* bDevice : Device number, 0~31                                        */
/* bFunction : Function number, 0~7                                     */
/* bCfgIndex : Configuation DWORD index, 0~15                           */
/* -------------------------------------------------------------------- */
void  Pci_WriteConfigAddress(BYTE bBus, BYTE bDevice, BYTE bFunction, BYTE bCfgIndex)
{
        DWORD   dOutData;
        WORD    HiWord,LoWord;
        BYTE   HiByte,LoByte;

        HiWord=0x8000|bBus;

        HiByte=(bDevice   <<3) | bFunction;
        LoByte=(bCfgIndex <<2) & 0xfc;
        LoWord=( (WORD)HiByte <<8 ) | LoByte;

        dOutData=( (DWORD)HiWord<<16 ) | LoWord;
        outpd(0xcf8,dOutData);
  
}
/* -------------------------------------------------------------------- */
/* PCI function                                                  Yes    */
/* -------------------------------------------------------------------- */

DWORD Pci_ReadConfigData(WORD *dwCFC, WORD *dwCFE)
{
        DWORD CfgData = inpd(0xcfc);
        if ( dwCFC ) /* if addres not ziro */  
                *dwCFC = (WORD)(CfgData & 0xFFFF);
        if ( dwCFE ) /* if addres not ziro */  
                *dwCFE = (WORD)(CfgData >> 16)& 0xFFFF;
        return CfgData;
}
 
/* -------------------------------------------------------------------- */
/* PCI function                                                     Yes */
/* -------------------------------------------------------------------- */

WORD   Pci_SearchDIO24(WORD SubVendorID, WORD SubDeviceID, WORD SubAuxID)
{
  BYTE    Bus, Device, Function;
  WORD    GotVendorID,    GotDeviceID,y=15;
  WORD    GotSubVendorID, GotSubDeviceID;
  BYTE    GotSubAuxID=0;
  DWORD   GotVenDev,    GotSubVenDev;
  DWORD   dwFindingID;/*    dwCompatibleID;*/
  WORD    wBaseAddr,wIrqNo;

  wTotalBoards =0;   /* initial number of board is 0 */
  dwFindingID  = ((DWORD)SubVendorID << 16) | SubDeviceID;

  for(Bus=0; Bus<32;  Bus++)  /* for 32 bus */
        {
                Function=0;
                for(Device=0;  Device<32;  Device++)  /* for 32 device */
                {
                         /* ****************************** */
                         /* Get the Vendor and Device ID */
                        Pci_WriteConfigAddress(Bus, Device, Function,PCI_CONFIG_VENDEV);
                        Pci_ReadConfigData(&GotVendorID, &GotDeviceID);
                        GotVenDev   = ((DWORD)GotVendorID << 16) | GotDeviceID; 
                         /* ****************************** */
                         /* Get the Sub-Vendor and Sub-Device ID */
                        Pci_WriteConfigAddress(Bus, Device, Function, PCI_CONFIG_SUBVENDEV);
                        Pci_ReadConfigData(&GotSubVendorID, &GotSubDeviceID);
                        GotSubVendorID &= 0xFFFF;
                        GotSubVenDev   = ((DWORD)GotSubVendorID << 16) | GotSubDeviceID;
                        if(GotSubVenDev!=dwFindingID) continue;
                         /*  ******************************  */
                         /*  Get Base Address Bar0           */
                        Pci_WriteConfigAddress(Bus, Device, Function, PCI_CONFIG_BAR0);
                        Pci_ReadConfigData(&wBaseAddr, NULL);
                        wBaseAddr &= 0xfffe;     /* addres of Digital I/O port */    
                         /* ****************************** */
                         /*  Get the interrupt channel  */
                        Pci_WriteConfigAddress(Bus, Device, Function, PCI_CONFIG_INTERRUPT);
                        Pci_ReadConfigData(&wIrqNo, NULL);
                        wIrqNo &= 0xff;                   
                         /* We are finding cards */                
                        wConfigSpace[wTotalBoards][0]=0xE159;         /* Vendor_ID     */
                        wConfigSpace[wTotalBoards][1]=0x0001;         /* Device_ID     */
                        wConfigSpace[wTotalBoards][2]=GotSubVendorID; /* sub_VENDOR_ID */
                        wConfigSpace[wTotalBoards][3]=GotSubDeviceID; /* sub_DEVICE_ID */
                        wConfigSpace[wTotalBoards][4]=GotSubAuxID;    /* sub_AUX_ID    */
                        wConfigSpace[wTotalBoards][5]=Bus;            /* SlotBusID     */
                        wConfigSpace[wTotalBoards][6]=Device;         /* SlotDeviceID  */
                        wConfigSpace[wTotalBoards][7]=wBaseAddr;      /* wBaseAddress  */
                        wConfigSpace[wTotalBoards][8]=wIrqNo;        /* wIrq          */

                        wTotalBoards++;  // increment board number

                } /* end of each device */

        }    /* end of each bus */

             if( wTotalBoards==0 )
                return( FindBoardError );
 
             
             return 0; /* No errors */           
   
  }
  
/* -------------------------------------------------------------------- */
/* DIO24 function                                              Yes     */
/* -------------------------------------------------------------------- */
WORD  DIO24_GetDriverVersion(WORD *wDriverVersion, WORD *wSubVendor, WORD *wSubDevice)
{
        *wDriverVersion=0x24;
        /* [2][3] --> subID to identify PCI cards */
        *wSubVendor = (pcicard[2].SubVenDev>>16)&0xFFFF;
        *wSubDevice =  pcicard[2].SubVenDev&0xFFFF;
        return( NoError );
}

/* -------------------------------------------------------------------- */
/* DIO24 function                                               Yes    */
/* -------------------------------------------------------------------- */
WORD  DIO24_DriverInit(WORD *wBoards,DWORD wSubVendorID, WORD wSubDeviceID, BYTE wSubAuxID)
{
        WORD  wReturnCode;
        wReturnCode=Pci_SearchDIO24((WORD)wSubVendorID, wSubDeviceID, wSubAuxID);
        *wBoards=wTotalBoards;
        return( wReturnCode );
}

/* -------------------------------------------------------------------- */
/* DIO24 function                                               Yes    */
/* -------------------------------------------------------------------- */
WORD  DIO24_DriverClose(void)
{
        return( NoError );
}

/* -------------------------------------------------------------------- */
/* DIO24 function                                                       */
/* -------------------------------------------------------------------- */
WORD   DIO24_Enable(WORD wBoard)
{WORD    wBaseAddr, wControl;
  wBaseAddr= wConfigSpace[(int)wBoard][7];
  wControl=1;/* Разрешение работы платы */   
  outpd(wBaseAddr,wControl);
  wControl=0;/*Конфигурация  PA,PB,PC - In */
  outpd(wBaseAddr+0xCC,wControl);
  return 1;
}

/* -------------------------------------------------------------------- */
/* DIO24 function                                                         */
/* -------------------------------------------------------------------- */
WORD   DIO24_Disable(WORD wBoard)
{WORD    wBaseAddr, wControl;
  wBaseAddr= wConfigSpace[(int)wBoard][7];
  wControl=0;/*Конфигурация РА,PB,PC - In */
  outpd(wBaseAddr+0xCC,wControl);
  wControl=0;  
  outpd(wBaseAddr,wControl);
  return 1;
}

/* -------------------------------------------------------------------- */
/* DIO24 function                                                       */
/* -------------------------------------------------------------------- */
WORD   DIO24_ConfigPorts(WORD wBoard, WORD wConf)
{WORD    wBaseAddr;
  wBaseAddr= wConfigSpace[(int)wBoard][7];
  outpd(wBaseAddr+0xCC,wConf);/*Конфигурация РА,PB,PC*/
  return 1;
 }
/* -------------------------------------------------------------------- */
/* DIO24 function                                                       */
/* -------------------------------------------------------------------- */
WORD   DIO24_Out(WORD wBoard,WORD Addr,WORD Out)
{ WORD    wBaseAddr,wDate,i=0;
  wBaseAddr= wConfigSpace[(int)wBoard][7];
  i=Addr&0x0FF; /* Wr into register */
  outpd(wBaseAddr+0xC8,0x00);
  outpd(wBaseAddr+0xC0,0); /* Port A - Hight byte */
  outpd(wBaseAddr+0xC4,i); /* Port B - Low byte */
  outpd(wBaseAddr+0xCC,7);/* All Ports in Out State */
  outpd(wBaseAddr+0xC8,0x01); /* Port C */
  outpd(wBaseAddr+0xC8,0x01); /* Пауза */
/*  outpd(wBaseAddr+0xC8,0x01); */ /* Пауза */
/*  outpd(wBaseAddr+0xC8,0x01); */ /* Пауза */
  wDate=(Out&0x0FF00)>>8; /* High byte */
  outpd(wBaseAddr+0xC0,wDate); /* Port A */
  wDate=(Out&0x00FF);
  outpd(wBaseAddr+0xC4,wDate); /* Port B */
  outpd(wBaseAddr+0xC4,wDate); /* Port B */
  outpd(wBaseAddr+0xC8,0); /* Port C */
  return 1;
    } 

/* -------------------------------------------------------------------- */
/* DIO24 function                                                       */
/* -------------------------------------------------------------------- */
WORD   DIO24_Read(WORD wBoard,WORD Addr,WORD *In)
{ WORD    wBaseAddr,wDate=0,y,x;
  wBaseAddr= wConfigSpace[(int)wBoard][7];
  wDate=Addr&0x0FF;
  outpd(wBaseAddr+0xCC,0x07);
  outpd(wBaseAddr+0xC8,0x00); /* Задание адреса */
  outpd(wBaseAddr+0xC0,0x00); /* Port A */
  outpd(wBaseAddr+0xC4,wDate); /* Port B */
  outpd(wBaseAddr+0xC8,0x01); /* Пауза */
/*  outpd(wBaseAddr+0xC8,0x01); */ /* Передача адреса */
  outpd(wBaseAddr+0xCC,0x04); /*Pfase 2 Ports A,B-In, C - Out; Funk - Read */
  outpd(wBaseAddr+0xC8,0x00);/* Переключение на ввод данных   */
  outpd(wBaseAddr+0xC8,0x00);/* Пауза */
/*  outpd(wBaseAddr+0xC8,0x00);*/ /* Пауза */
/*  outpd(wBaseAddr+0xC8,0x00);*/ /* Пауза */
  x=inpd(wBaseAddr+0xC4)&0x0FF;/* Read port PB */
  y=inpd(wBaseAddr+0xC0)&0x0FF;/* Read port PA, high byte */
  *In=(y<<8)+x;  
  outpd(wBaseAddr+0xC8,0x01);/* Обращение к нулевой ячейке  */
  outpd(wBaseAddr+0xC8,0x01);/* Пауза */
  outpd(wBaseAddr+0xC8,0); /* Завершение цикла чтения   */
  outpd(wBaseAddr+0xC4,0); /* Завершение цикла чтения   */
  return 1;
  }
