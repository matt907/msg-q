#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Melissa Collins 11/10/17
// Path for mult: /home3/mcollins/cs311

struct mess{
	long mtype;
	int nextInt;
};

void main(int argc, char *argv[]){
	if(argc == 3){ //check for the correct number of parameters
		struct mess message;    //declare message struct
		struct msqid_ds buffer; //declare buffer struct

		int value = atoi(argv[1]); //value being multiplied by
	
		int counter = atoi(argv[2]); //counter declared in parameters
		int i, status; //for loop counter & return value of msgsnd, msgrcv
	   
	   //create message queues
		int server = msgget(130, 0600 | IPC_CREAT);
		int mult = msgget(130 + value, 0600 | IPC_CREAT);
	   
	   //initialize message
		message.mtype = 2; 
		message.nextInt = 1;
	
	   //receive value of 1 from server
		status = msgrcv(mult, &message, sizeof(message)-sizeof(long),2 ,0);
		int result = message.nextInt * value;
		message.nextInt = result;
		//send value to server array
		status = msgsnd(server, &message, sizeof(message)-sizeof(long), 0);
	
		for(i = 0; i < counter; i++){
		  //recieve what is in the server array
			status = msgrcv(mult, &message, sizeof(message)-sizeof(long),2,0);
			result = message.nextInt * value;
			message.nextInt = result;
		  //send back multiplied value
			status = msgsnd(server, &message, sizeof(message)-sizeof(long), 0);
		}

		status = msgctl(mult, IPC_RMID, & buffer); //close queue

	}
	else
		printf("Invalid Parameters");

}//end main
	
