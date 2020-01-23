/*
 * ex6a1.c
 *
 * socket
 * ====================================================
 *
 * Writen by: adi ben ezra, id = 206124000, login = adibene
 *            yael yishay , id = 305345811, login = yaelor
 *
 * algoritem:
 * built out of two servers and a client
 * the program sends msg between the client
 * and servers and between the two servers by socket
 */



//Includes--------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFLEN 20
#define PROCESS_MAX 5 // size process array

//Functions---------------------------------
void checkInput(int argc);
void memsetFunc(struct addrinfo *con_kind);
void socketFunc(int *main_socket,struct addrinfo *addr_info_res);
void bindFunc(int main_socket,struct addrinfo *addr_info_res);
void listenFunc(int main_socket);
void acceptFunc(int* serving_socket,int main_socket);
void getClients(FILE *fdr,FILE *fdw);
int addprocess(pid_t *processArray,int my_msg, int* counter);
int processCheck(pid_t *processArray,int my_msg,int* counter);
void deleteProcess(pid_t processArray[],int my_msg,int* counter);
//Main-------------------------------------
int main(int argc,char* argv[]){

	int main_socket, //file descriptor for new client
	serving_socket;
	char rbuf[BUFLEN];

	struct addrinfo con_kind,
	*addr_info_res;

	checkInput(argc);

	memsetFunc(&con_kind);

	int rc;   //return code of s.c

	if((rc = getaddrinfo(argv[1], // Ip server
			argv[2], //port server
			&con_kind,
			&addr_info_res))!= 0){
		fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc)) ;
		exit(EXIT_FAILURE) ;
	}

	socketFunc(&main_socket,addr_info_res);

	bindFunc(main_socket,addr_info_res);

	listenFunc(main_socket);

	while(1){

		acceptFunc(&serving_socket,main_socket);

		while(read(serving_socket,rbuf,BUFLEN)>0);

		printf("%s/n",rbuf);

		close(serving_socket);
	}

	return (EXIT_SUCCESS);

}
//--------------------------------------------------
void checkInput(int argc)
{
	if(argc != 2){
		perror("Missing server name or IP server\n");
		exit(EXIT_FAILURE);
	}
}
//-------------------------------------------------------------
void memsetFunc(struct addrinfo *con_kind)
{
	memset(con_kind,0,sizeof(*con_kind)) ;
	con_kind->ai_family = AF_UNSPEC;  // AF_INET, AF_INET6
	con_kind->ai_socktype = SOCK_STREAM;
	con_kind->ai_flags = AI_PASSIVE;
}
//-------------------------------------------------------
void socketFunc(int *main_socket,struct addrinfo *addr_info_res){

	if((*main_socket = socket(addr_info_res->ai_family, // PF_INET
			addr_info_res->ai_socktype,               // SOCK_STREAM
			addr_info_res->ai_protocol)) < 0){

		perror("socket: allocation failed");
		exit(EXIT_FAILURE) ;
	}
}
//---------------------------------------------------
void bindFunc(int main_socket,struct addrinfo *addr_info_res){

	if(bind(main_socket,
			addr_info_res->ai_addr,
			addr_info_res->ai_addrlen)){

		perror("bind failed");
		exit(EXIT_FAILURE) ;
	}
}
//-----------------------------------------------------------------
void listenFunc(int main_socket){

	if(listen(main_socket,PROCESS_MAX)){

		perror("listen failed");
		exit(EXIT_FAILURE) ;
	}
}
//-------------------------------------------------------
void acceptFunc(int* serving_socket,int main_socket){


	*serving_socket = accept(main_socket,NULL,NULL);

	if(*serving_socket < 0){
		perror("accept failed");
		exit(EXIT_FAILURE) ;
	}
}
//--------------------------------------------------------
//gets process number from client
void getClients(FILE *fdr,FILE *fdw)
{

	pid_t processArray[PROCESS_MAX];//array of client

	int num,   //number of request
	my_msg,    //process id client
	counter = 0, // the last location in process array
	result;    //response server

	while(1)
	{
		//read number and id
		fscanf(fdr,"%d %d",&num,&my_msg);

		switch(num){

		case 1:
			result = addprocess(processArray,my_msg,&counter);
			break;
		case 2:
			result = addprocess(processArray,my_msg,&counter);
			break;
		case 3:
			deleteProcess(processArray,my_msg,&counter);
		}
		break;
	}
}
//-----------------------------------------------------------
//add process to id array
int addprocess(pid_t *processArray,int my_msg, int* counter){

	pid_t processNumber = my_msg;

	//if process is in array
	if(processCheck(processArray,my_msg,counter) == 1)
		return 1;

	else if(*counter < PROCESS_MAX){
		processArray[*counter] = processNumber;
		(*counter)++;
		return 0;
	}
	return 2;
}
//------------------------------------------------------
//check if process is in array
int processCheck(pid_t *processArray,int my_msg,int* counter)
{
	int i;
	pid_t processNumber = my_msg;

	for(i = 0; i<=*counter; i++)
		if(processArray[i] == processNumber)
			return 1;

	return 0;
}
//-------------------------------------------------------
//delete process from array
void deleteProcess(pid_t processArray[],int my_msg,int* counter)
{
	int i,j;
	pid_t processNumber= my_msg;

	for(i = 0; i< *counter; i++){
		if(processArray[i] == processNumber)
			break;
	}

	for(j = i; j<*counter; j++)
		processArray[j] = processArray[j+1];
}
