#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define ARRAY_SIZE 150
#define REPEATS 100000

struct mess{
	long mtype;
	int nextInt;
};

//Swap items for bubble sort
void swap(int *first, int *second){
	int temp = *first;
	*first = *second;
	*second = temp;
}

//Bubble Sort
void sort(int array[], int arraySize){
	int i,j;
	for(i=0; i<arraySize; i++){
		for(j=0; j<arraySize; j++){
			if(array[j] > array[j+1])
				swap(&array[j], &array[j+1]);
		}
	}
}//end sort	

void main(int argc, char *argv[]){
	if(argc == 2){ //check for the correct number of parameters
		struct mess message;
		struct msqid_ds buffer;

		int counter = atoi(argv[1]); //counter declared in parameters
		int result[ARRAY_SIZE]; //holds the results from mult
	   
		int i, status; //for loop counter & return value of msgsnd, msgrcv

		//initialize values in result array
		for(i=0;i<ARRAY_SIZE;i++){
			result[i] = REPEATS;
		}

		//create array to handle duplicate values
		//while value = result[i], repeats[value] will be set to one
		//indicating that that certain value already exists in result[]
		int repeats[REPEATS];
		int resultCounter = 0;

	   //create message queues
		int server = msgget(130, 0600 | IPC_CREAT);
		int two = msgget(132, 0600 | IPC_CREAT);
		int three = msgget(133, 0600 | IPC_CREAT);
		int five = msgget(135, 0600 | IPC_CREAT);
	
	   //initialize message
		message.mtype = 2;
		message.nextInt = 1;

		//send message to mult.c and get result back
		status = msgsnd(two, &message, sizeof(message)-sizeof(long),0);
		status = msgrcv(server, &message, sizeof(message)-sizeof(long),2,0);

		//put those results into the result array and set repeats
		//at that position to 1
		result[resultCounter] = message.nextInt;
		repeats[message.nextInt] = 1;
		resultCounter++;

		//reset nextInt to 1
		message.nextInt = 1;

		//send message to mult.c and get result back
		status = msgsnd(three, &message, sizeof(message)-sizeof(long),0);
		status = msgrcv(server, &message, sizeof(message)-sizeof(long),2,0);

		//put those results into the result array and set repeats
		//at that position to 1
		result[resultCounter] = message.nextInt;
		repeats[message.nextInt] = 1;
		resultCounter++;

		//reset nextInt to 1
		message.nextInt = 1;

		//send message to mult.c and get result back
		status = msgsnd(five, &message, sizeof(message)-sizeof(long),0);
		status = msgrcv(server, &message, sizeof(message)-sizeof(long),2,0);

		//put those results into the result array and set repeats
		//at that position to 1
		result[resultCounter] = message.nextInt;
		repeats[message.nextInt] = 1;
		resultCounter++;	

		for(i=0; i<counter;	i++){ //loop through specified counter
			message.nextInt = result[i]; //put int from result into a message

			//send that message to mult.c and get result		
			status = msgsnd(two, &message, sizeof(message)-sizeof(long),0);
			status = msgrcv(server, &message, sizeof(message)-sizeof(long),2,0);
		
			//check for duplicate values
			if(repeats[message.nextInt] == 0){
				result[resultCounter] = message.nextInt;
				repeats[message.nextInt] = 1;
				resultCounter++;
			}

			message.nextInt = result[i]; //put int from result into a message

			//send that message to mult.c and get result
			status = msgsnd(three, &message, sizeof(message)-sizeof(long),0);
			status = msgrcv(server, &message, sizeof(message)-sizeof(long),2,0);
			
			//check for duplicate values
			if(repeats[message.nextInt] == 0){
				result[resultCounter] = message.nextInt;
				repeats[message.nextInt] = 1;
				resultCounter++;
			}

			message.nextInt = result[i]; //put int from result into a message

			//send that message to mult.c and get result
			status = msgsnd(five, &message, sizeof(message)-sizeof(long),0);
			status = msgrcv(server, &message, sizeof(message)-sizeof(long),2,0);

			//check for duplicate values			
			if(repeats[message.nextInt] == 0){
				result[resultCounter] = message.nextInt;
				repeats[message.nextInt] = 1;
				resultCounter++;
			}
		
			//use bubble sort to put results in order
			sort(result, resultCounter);

		}

		//print out results
		for(i = 0; i < counter; i++){
			printf("[%d] = %d\n", i, result[i]);
		}

		status = msgctl(server, IPC_RMID, & buffer); //close queue
	}
	else
		printf("Invalid Parameters");

}//end main
	
