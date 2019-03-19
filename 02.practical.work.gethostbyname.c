#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h> 
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
	char hostname[256];
	if(argc == 1){
		printf("Enter a hostname: ");
		scanf("%s", hostname);		

	}
	else if(argc > 2){
		printf("Too many arguments.\n");
		exit(-1);
	}
	/*
	else if(argc == 2){
		char *hostname = argv[1];
	}
	
	else if(argc < 2){
		printf("Provide a hostname to resolve \n");
		exit(1);
	}
	*/

	struct hostent *gethost = gethostbyname(hostname); // get Internet host name
	struct in_addr **IP;
	IP = (struct in_addr **)gethost -> h_addr_list; // get all the address

	for(int i=0; IP[i] != NULL; i++){
		printf("%s\n", inet_ntoa(*IP[i])); // to show the resolved IP address
	}

	return 0;
}