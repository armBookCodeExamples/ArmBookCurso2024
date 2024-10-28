//=====[#include guards - begin]===============================================

#ifndef _SERIAL_COMMUNICATION_
#define _SERIAL_COMMUNICATION_

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void serialComInit();
void serialComKeypadUpdate();
void serialMenu();
void serialComWrite( char* message, int length);

//=====[#include guards - end]=================================================

#endif // _PC_SERIAL_COM_H_