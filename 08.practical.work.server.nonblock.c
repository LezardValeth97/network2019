#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int sockfd, clen, clientfd;

	struct sockaddr_in saddr, caddr;

	unsigned short port = 8784;

	char s[256];

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
		printf("Error creating socket\n");
		close(sockfd);
		return -1;
	}

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	int fl = fcntl(sockfd, F_GETFL, 0);
	fl |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, fl);

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if((bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr))) < 0){
		printf("Error in binding\n");
		close(sockfd);
		return -1;
	}

	if(listen(sockfd, 5) < 0){
		printf("Error Listening\n");
		close(sockfd);
		return -1;
	}

	clen = sizeof(caddr);

	while(1){
		if ((clientfd = accept(sockfd, (struct sockaddr *) &caddr, &clen)) > 0){
			int fl = fcntl(clientfd, F_GETFL, 0);
			fl |= O_NONBLOCK;
			fcntl(clientfd, F_SETFL, fl);
			
			printf("There is a client want to connect \n");

			while(1){
				memset(s, 0, sizeof(s));
				if(recv(clientfd, s, sizeof(s), 0) > 0){
					printf("Client says:%s", s);
				}
			
				if(strcmp(s, "/dc\n") == 0){
					shutdown(clientfd, SHUT_RDWR);
					close(clientfd);
					break;
				}
			}
		}
	}

	return 0;
}




