#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(){
	int socketfd, clen, clientfd;

	struct sockaddr_in saddr, caddr;

	unsigned short port = 8784;
	char s[100];
	char t[100];	

	if((socketfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
		printf("Error creating socket\n");
		return -1;
	}

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if((bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr))) < 0){
		printf("Error in binding\n");
		return -1;
	}

	if(listen(socketfd, 5) < 0){
		printf("Error Listening\n");
		return -1;
	}

	clen = sizeof(caddr);

	if ((clientfd = accept(socketfd, (struct sockaddr *) &caddr, &clen)) < 0){
		printf("Error accpeting connection\n");
		return -1;
	}
	
	printf("There's a client want to connect!\n");


	while(1){
		memset(s, 0, sizeof(s));
		printf("Server> \n");
		fgets(s, sizeof(s), stdin);

		if(strcmp(s, "/dc\n") == 0){
			shutdown(clientfd, SHUT_RDWR);
			close(clientfd);
			break;
		}	

		send(clientfd, s, strlen(s), 0);
		memset(s, 0, sizeof(s));

		while(1){
			recv(clientfd, t, sizeof(t), 0);
			strcat(s, t);
			memset(t, 0, sizeof(t));
			if(s[strlen(s)] == '\0')
				break;
		}

		printf("Client says:%s\n", s);
	}
	return 0;
}