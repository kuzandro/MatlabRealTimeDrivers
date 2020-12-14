/*
 * CSI_DIO24.h: Header file for CSI_DIO24 driver
 * Generated on: 30-Oct-2019 13:08:15
*/

#ifndef __CSI_DIO24_H__
#define __CSI_DIO24_H__

#ifndef MATLAB_MEX_FILE
#include "xpctarget.h"
#endif

void CSI_DIO24Start(short int *plata, short int *memo);
void CSI_DIO24Output(short int *N1, short int *N2, short int *N3, short int *NM, short int I[3], short int U[3], long int *FI, short int *W, short int *M, short int *plata, short int *memo);
void CSI_DIO24Terminate(short int *plata, short int *memo);

#endif

/*
 * Mapping of mask variables to parameter arguments:
*/
