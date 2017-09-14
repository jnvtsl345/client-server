// A synchronized client/server message exchange(1 to 1).

// Example 2: Server

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

void child(int sd,int fd);

int main(int argc, char *argv[])
{
	int sd, client1,client2, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 2)
	{
		printf("Call model: %s <Port #>\n", argv[0]);
		exit(0);
	}

	if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);
	bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
	listen(sd, 5);

	while(1)
	{
		client1=accept(sd,(struct sockaddr*)NULL,NULL);
		client2=accept(sd,(struct sockaddr*)NULL,NULL);
		printf("Got a clients\n");
		if(!fork())
			child(client1,client2);
		close(client1);
		close(client2);
	}
}

void child(int sd,int fd)
{
	int x;
	int y;
	int j,k;
	char player1[255];
	char player2[255];
		
	recv(sd,&player1,sizeof(player1),0);
		recv(fd,&player2,sizeof(player2),0);
		fprintf(stderr,"Name of Players:%s,%s \n",player1,player2);
		send(fd,&player1,sizeof(player1),0);
		send(sd,&player2,sizeof(player2),0);
	while(1)
	{
		
		recv(sd,&x,sizeof(x),0);
		recv(sd,&y,sizeof(y),0);
		recv(fd,&j,sizeof(j),0);
		recv(fd,&k,sizeof(k),0);
		send(sd,&j,sizeof(j),0);
		send(sd,&k,sizeof(k),0);
		send(fd,&x,sizeof(x),0);
		send(fd,&y,sizeof(y),0);
	}
}

