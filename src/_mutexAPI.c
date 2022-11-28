/*----------------------------------------------------------------------------
 * Name: _mutexAPI.c
 * Purpose: Stores any functions a part of the Mutex API
 *----------------------------------------------------------------------------
*/

//Include header file for _kernelCore, _threadsCore, and _mutexAPI 
#include "_kernelCore.h"
#include "_threadsCore.h"
#include "_mutexAPI.h"

osMutex osMutexes[MAX_MUTEXES]; //Static mutex struct array
int num_mutexes = 0; //Number of created mutexes

extern rtosThread osThreads[MAX_THREADS]; //Static thread struct array

//Create a mutex
int osCreateMutex(void)
{
	//Create the mutex if the number of mutexes is less than the maximum
	if (num_mutexes < MAX_MUTEXES)
	{
		osMutexes[num_mutexes].available = true; //Set the mutex as available
		osMutexes[num_mutexes].ID = num_mutexes; //Set the ID of mutex to the current index
		osMutexes[num_mutexes].threadOwns = EMPTY_INDEX; //No thread owns the mutex yet, so a value of -1 is used
		
		//Initalize the waiting queue for this mutex to have no threads stored (all indexes equal to -1)
		for(int i = 0; i < MAX_THREADS; i++)
		{
			osMutexes[num_mutexes].waitingQueue[i] = EMPTY_INDEX; //Set the index as -1 (no thread)
		}
		
		num_mutexes++; //Increment the number of mutexes
		return num_mutexes - 1; //Return the mutex index (position of the mutex in the array)
	}
	return -1; //Return -1 if the mutex cannot be created
}

//Acquire the mutex
bool osAcquireMutex(int thread_index, int mutex_index)
{
	bool acquiredMutex = false; //Boolean for whether the mutex is acquired or not
	
	//Only acquire the mutex if it is available or the thread already owns the mutex
	if(osMutexes[mutex_index].available || osMutexes[mutex_index].threadOwns == thread_index)
	{
		osMutexes[mutex_index].threadOwns = thread_index; //Set the thread index that owns the mutex
		osMutexes[mutex_index].available = false; //Set the availbility of the mutex to false
		acquiredMutex = true; //Acquiring the mutex was successful
	}
	else 
	{
		//If the mutex cannot be acquired, store the thread index in the waiting queue
		for(int i = 0; i < MAX_THREADS; i++)
		{
			//Check to see if the thread is already stored in the waiting queue
			if (osMutexes[mutex_index].waitingQueue[i] != thread_index)
			{
				//Find the first free index in the queue and store the thread index there
				if(osMutexes[mutex_index].waitingQueue[i] == EMPTY_INDEX)
				{
					osMutexes[mutex_index].waitingQueue[i] = thread_index; //Store the thread index
					osThreads[thread_index].status = BLOCKED; //Block the thread while in the waiting queue
					i = MAX_THREADS; //Break out of the loop
				}
			}
			else 
			{
				i = MAX_THREADS; //Break out of the loop if it is already stored in the waiting queue
			}
		}
	}
	return acquiredMutex; //Return whether the mutex was acquired or not (success or failed)
}

//Release the mutex
void osReleaseMutex(int thread_index, int mutex_index)
{
	//Only release the mutex if the thread already owns it
	if(osMutexes[mutex_index].threadOwns == thread_index)
	{
		osMutexes[mutex_index].available = true; //Set the availbility of the mutex to true
		
		//Give the mutex to the next thread in the waiting queue
		if(osMutexes[mutex_index].waitingQueue[0] != EMPTY_INDEX)
		{
			//Set the next thread in the waiting queue to acquire the mutex
			osAcquireMutex(osMutexes[mutex_index].waitingQueue[0], mutex_index);
			
			//Move the thread back into the OS's thread waiting pool
			osThreads[osMutexes[mutex_index].waitingQueue[0]].status = WAITING; 
			
			//Shift all the threads waiting in the waiting queue
			//This means the next waiting thread is in the earliest index (0)
			for(int i = 0; i < MAX_THREADS - 1; i++)
			{
				//Move the thread from the i+1 to the index at i
				osMutexes[mutex_index].waitingQueue[i] = osMutexes[mutex_index].waitingQueue[i+1];
			}
			
			//Make the last position empty in the waiting queue
			osMutexes[mutex_index].waitingQueue[MAX_THREADS - 1] = EMPTY_INDEX;
		}
	}
}
