/*
 * ex6a3.c
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
#include <unistd.h> // for read/write/close
#include <sys/types.h> // Stander system types
#include <netinet/in.h> // Internet address structures
#include <sys/socket.h> // socket interface functions
#include <netdb.h> //host to ip resolution

#define SERVER_PORT "3876"
#define BUFLEN 20 // maximum response size
//Functions---------------------------------
void memsetFunc(struct addrinfo *con_kind);
void checkInput(int argc);
void socketFunc(int *my_socket,
		struct addrinfo *addr_info_res);
void connectFunc(int my_socket,
		struct addrinfo *addr_info_res);
void registeredToServer(int my_socket);
void readUserInput(char c[BUFLEN],
		char string[BUFLEN]);
void closeFunc(int my_socket,
		struct addrinfo *addr_info_res);
//Main-------------------------------------
int main(int argc,char *argv[]){

	int rc;   //return code of s.c
	int my_socket; //file descriptor
	char c[BUFLEN], // char user input
	string[BUFLEN]; // string user input

	struct addrinfo con_kind,
	*addr_info_res;

	checkInput(argc);

	memsetFunc(&con_kind);

	if((rc = getaddrinfo(argv[1], // IP server
			SERVER_PORT, //port server
			&con_kind,
			&addr_info_res)) != 0){
		fprintf(stderr, "getaddrinfo() failed %s\n", gai_strerror(rc)) ;
		exit(EXIT_FAILURE) ;
	}

	socketFunc(&my_socket,addr_info_res);

	connectFunc(my_socket,addr_info_res);

	registeredToServer(my_socket);

	while(1){

		readUserInput(c,string);
	}

	closeFunc(my_socket,addr_info_res);

	return (EXIT_SUCCESS);
}
//--------------------------------------------------
void checkInput(int argc)
{
	if(argc != 5){  //check before running maybe is 4
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
}

//-------------------------------------------------------
void socketFunc(int *my_socket,struct addrinfo *addr_info_res){

	if((*my_socket = socket(addr_info_res->ai_family, // PF_INET
			addr_info_res->ai_socktype,               // SOCK_STREAM
			addr_info_res->ai_protocol)) < 0){

		perror("socket: allocation failed");
		exit(EXIT_FAILURE) ;
	}
}
//---------------------------------------------------------------------
void connectFunc(int my_socket,struct addrinfo *addr_info_res){

	int rc;   //return code of s.c

	rc = connect(my_socket,
			addr_info_res->ai_addr,    // addr of server: IP+PORT
			addr_info_res->ai_addrlen);

	if(rc){
		perror("connect failed:");
		exit(EXIT_FAILURE) ;
	}
}
//----------------------------------------------------------
//send request to register server and get result
//the client registering to the server
void registeredToServer(int my_socket)
{
	char rbuf[BUFLEN];
	int number;

	sprintf(rbuf,"%d %d",1,getpid());
	printf("%s\n",rbuf);

	write(my_socket, rbuf, strlen(rbuf)+1);

	while (read(my_socket, rbuf, BUFLEN) > 0);
	printf("%s\n",rbuf);

	//change text to number
	number = atoi(rbuf);
	printf("%d",number);

	if(number != 0){
		perror("The client can not be registered\n");
		exit (EXIT_FAILURE);
	}
}
//-------------------------------------------------------
void readUserInput(char c[BUFLEN],char string[BUFLEN])
{
	//get input from user
	printf("Enter char:");

	scanf("%s",c);

	//get input to string number/string
	scanf("%s",string);
}
//------------------------------------------------------------
void closeFunc(int my_socket,struct addrinfo *addr_info_res)
{
	close(my_socket);
	freeaddrinfo(addr_info_res);
}
