#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
char matrix[3][3];  /* the tic tac toe matrix */

char check(void);
void init_matrix(void);
void get_player_move(void);
void get_computer_move(void);
void disp_matrix(void);
int main(int argc, char *argv[])
{
	char message[255];
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;

	if(argc != 3)
	{
		printf("Call model:%s <IP> <Port#>\n",argv[0]);
		exit(0);
	}

	if((server = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		fprintf(stderr, "Cannot create socket\n");
		exit(1);
	}

	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET,argv[1],&servAdd.sin_addr)<0)
	{
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}

	if(connect(server,(struct sockaddr *)&servAdd,sizeof(servAdd))<0)
	{
		fprintf(stderr, "connect() has failed, exiting\n");
		exit(3);
	}

	char done;
	while(1)
	{
		
		char name[255];
		char oppo[255];

  fprintf(stderr,"This is the game of Tic Tac Toe.\n");
  fprintf(stderr,"Enter your name:.\n");
	fgets(name, 254, stdin);
	send(server,&name,sizeof(name),0);
	recv(server,&oppo,sizeof(oppo),0);
  fprintf(stderr,"Your Opponent is:%s\n",oppo);

  done =  ' ';
  init_matrix();

  do {
     disp_matrix();
    int x, y;
  fprintf(stderr,"Enter X,Y coordinates for your move: ");
  scanf("%d%*c%d", &x, &y);
  send(server,&x,sizeof(x),0);
  send(server,&y,sizeof(y),0);
  x--; y--;

  matrix[x][y] = 'X';/* Get a player's move. */
    done = check(); /* see if winner */
    if(done!= ' ')
	{	
	break;
	} /* winner!*/
    
    int i, j;
    recv(server,&i,sizeof(i),0);
     recv(server,&j,sizeof(j),0);	

	fprintf(stderr,"server sent: %d,%d \n",i,j);
	i--;j--;
    matrix[i][j] = 'O';/* Get a move from the computer. */
    done = check();
      /* see if winner */
  } while(done== ' ');


	disp_matrix();	
  if(done=='X') fprintf(stderr,"vatsal has won!\n");
	
  else fprintf(stderr,"You Lost!!!!\n");
	
  
	}	
	
}
/* Initialize the matrix. */
void init_matrix(void)
{
  int i, j;

  for(i=0; i<3; i++)
    for(j=0; j<3; j++) matrix[i][j] =  ' ';
}

/* Display the matrix on the screen. */
void disp_matrix(void)
{
  int t;

  for(t=0; t<3; t++) {
    printf(" %c | %c | %c ",matrix[t][0],
            matrix[t][1], matrix [t][2]);
    if(t!=2) printf("\n---|---|---\n");
  }
  printf("\n");
}

/* See if there is a winner. */
char check(void)
{
  int i;

  for(i=0; i<3; i++)  /* check rows */
    if(matrix[i][0]==matrix[i][1] &&
       matrix[i][0]==matrix[i][2]) return matrix[i][0];

  for(i=0; i<3; i++)  /* check columns */
    if(matrix[0][i]==matrix[1][i] &&
       matrix[0][i]==matrix[2][i]) return matrix[0][i];

  /* test diagonals */
  if(matrix[0][0]==matrix[1][1] &&
     matrix[1][1]==matrix[2][2])
       return matrix[0][0];

  if(matrix[0][2]==matrix[1][1] &&
     matrix[1][1]==matrix[2][0])
       return matrix[0][2];

  
}
