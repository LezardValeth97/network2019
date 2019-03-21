#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>


int main(int argc, char **argv)
{
	struct sockaddr_in saddr;
	struct hostent *h;
	int sockfd;
	unsigned short port = 8784;

	char hostname[256];
	char s[100];
	char t[100];	
	if(argc == 1){
		printf("Enter a hostname: ");
		scanf("%s", hostname);		

	}
	else if(argc > 2){
		printf("Too many arguments.\n");
		return -1;
	}
	else if(argc < 2){
		printf("Provide a hostname to resolve \n");
		return 1;
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket \n");
		close(sockfd);
		return -1;
	}

	if((h = gethostbyname(hostname)) == NULL) {
		printf("Unknown host \n");
		close(sockfd);
		return -1;
	}

	memset(&saddr, 0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	memcpy((char *) &saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);

	struct in_addr **IP;
	IP = (struct in_addr **)h -> h_addr_list; // get all the address
	struct in_addr ip = *IP[0];
	printf("%s\n", inet_ntoa(ip));

/*
	for(int i=0; IP[i] != NULL; i++){
		printf("%s\n", inet_ntoa(*IP[i])); // to convert the hostname into IP address and show the resolved IP address
	}
*/
	saddr.sin_port = htons(port);

	if(connect(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
		printf("Cannot connect \n");
		close(sockfd);
		return -1;
	}
	else{
		printf("Connected successfully \n");
	}

	while (1) {
		memset(s, 0, sizeof(s));

		while(1){
			recv(sockfd, t, sizeof(t), 0);
			strcat(s, t);
			memset(t, 0, sizeof(t));
			if(s[strlen(s)] == '\0') 
				break;
		}

		printf("Server says: %s\n", s);	
		//read(sockfd, s, sizeof(s));

		memset(s, 0, sizeof(s));
		printf("Client>: ");

		if(fgets(s,sizeof(s), stdin) == NULL){
			return -1;
		}

		if(strcmp(s, "/quit\n") == 0){
			shutdown(sockfd, SHUT_RDWR);
			close(sockfd);
			printf("Exit! \n");
			break;
		}		
		
		send(sockfd, s, strlen(s), 0);

	}
	return 0;
}

