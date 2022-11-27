/*----------------------------------------------------------------------------
 * Name: _mutexAPI.h
 * Purpose: Stores any functions a part of the Mutex API
 *----------------------------------------------------------------------------
*/

//Include guards for _mutexAPI
#ifndef _mutexAPI
#define _mutexAPI

#include "osDefs.h"

//Create a mutex
int osCreateMutex(void);

//Acquire the mutex
bool osAcquireMutex(int thread_index, int mutex_index);

//Release the mutex
void osReleaseMutex(int thread_index, int mutex_index);

#endif
