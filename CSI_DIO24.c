/*
 * CSI_DIO24.c: C Source stub file for CSI_DIO24 driver
 * Generated on: 11-Jul-2019 19:39:08
*/

#include "CSI_DIO24.h"
#include <csi_dio_io.h>
short int plata,memo;

void CSI_DIO24Start(short int *plata, short int *memo)
{
#ifndef MATLAB_MEX_FILE
// Place driver code to run on the target at model start/stop here (see xpcIsModelInit)
WORD wSubVenderID=0,wSubDeviceID=0;
BYTE wSubAuxID=0;
WORD wDriverVersion=0,wBoards=0;
*memo=malloc(200*sizeof(int));
DIO24_GetDriverVersion(&wDriverVersion,&wSubVenderID,&wSubDeviceID);
   wSubAuxID=0x40;
   *plata=-1;
/* Set initial dates in variables */   
*plata=DIO24_DriverInit(&wBoards,wSubVenderID,wSubDeviceID,wSubAuxID);
/* Test: the card is present? If no, then platen=1 -Error  */ 
if(*plata==0) { DIO24_Enable(0);  /* If plata present, then enable inputs/outputs */
               }              
#endif
}

void CSI_DIO24Output(short int *N1, short int *N2, short int *N3, short int *NM, short int I[3], short int U[3], long int *FI, short int *W, short int *M, short int *plata, short int *memo)
{
#ifndef MATLAB_MEX_FILE
// Place driver code to run on the target during model execution here
   WORD x,F1,F2; 
   long int y;
if (*plata==0){  /* x=DIO24_Out(0,1,*N1);  
                  x=DIO24_Out(0,2,*N2); 
                  x=DIO24_Out(0,3,*N3);
                  x=DIO24_Out(0,4,*NM);  
                  x=DIO24_Read(0,11,&F1); 
                  x=DIO24_Read(0,12,&F2); 
                  y=(long int)F1; */ /*<<16; */
                /*  *FI=(long int)F2; */    
		          x=DIO24_Read(0,5,I);  
                /*  x=DIO24_Read(0,6,I+1);
                  x=DIO24_Read(0,7,I+2); 
                  x=DIO24_Read(0,8,U);
                  x=DIO24_Read(0,9,U+1);
                  x=DIO24_Read(0,10,U+2);     
                  x=DIO24_Read(0,13,W);
                  x=DIO24_Read(0,14,M); */  
                   }
#endif
}

void CSI_DIO24Terminate(short int *plata, short int *memo)
{
#ifndef MATLAB_MEX_FILE
// Place driver code to run on the target at model start/stop here
   WORD x;
   short int *wCont=0;
   if(*plata==0){  /* Reset cantrols */
                  x=DIO24_Out(0,1,*wCont);
                  x=DIO24_Out(0,2,*wCont);
                  x=DIO24_Out(0,3,*wCont);
                  x=DIO24_Out(0,4,*wCont); 
                  x=DIO24_Out(0,15,*wCont);                
                  x=DIO24_ConfigPorts(0,0);/* Все порты - In */ 
                  free(memo); 
                  DIO24_Disable(0); /* Disable DIO24 */
                 }
   
   DIO24_DriverClose();    
#endif
}

