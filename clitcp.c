#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void isValidArguments (int argc, char ** argv);
int openAtcpNetworkConnection ();
void initiateAconnectionOnAsocket (char ** argv, int s);
void readMsgAndCopyToBuffer (char * buffer);
void sendMsg (char * msgBuffer, int s);
void recvMsg (char * msgBuffer, int s);
void closeClient (int s);

int main( argc, argv)
int argc;
char **argv;
{
	int s; 
	char buf[1024]; 

	isValidArguments(argc, argv);

	s = openAtcpNetworkConnection();

	initiateAconnectionOnAsocket(argv, s);

	readMsgAndCopyToBuffer(buf);

	sendMsg(buf, s);

	recvMsg(buf, s);

	closeClient(s);
	return (0);
}

void isValidArguments (int argc, char ** argv)
{
	/* argv[0] file name
	* argv[1] IP host
	* argv[2] PORT host
	*/
	if( argc != 3 )
	{ 
		fprintf( stderr, "Uso: %s hostname porta\n", argv[0] );
		exit(1);
	}
}

int openAtcpNetworkConnection ()
{
	fprintf(stderr, "Criando socket TCP\n");

	int connenction = socket(AF_INET, SOCK_STREAM, 0);

	if( connenction == -1 ) // { 0 succeeds, -1 an Error }
	{
		fprintf(stderr, "Erro Socket ()\n");
		exit(2);
	}

	fprintf(stderr, "Socket criado com successo\n");
	
	return connenction; // returns a channel identifier
}

void initiateAconnectionOnAsocket (char ** argv, int s)
{
	// inicia uma conexao com o socket do servidor
	// argv[1] IP host
	// argv[2] PORT host

	unsigned short port ; /* numero porta servidor */
	struct sockaddr_in server; /* end servidor */

	// atoi() converte string pra int
	port = (unsigned short) atoi(argv[2]); /* porta */

	server.sin_family = AF_INET; 
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr( argv[1] );

	if(connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		fprintf(stderr, "Erro connect ()\n");
		exit(3);
	}
}

void readMsgAndCopyToBuffer (char * buffer)
{
	char readMsg[1024];

	printf("Mensagem para o servidor: ");	
	scanf("%s", readMsg);

	strcpy(buffer, readMsg); /* messagem */
}

void sendMsg (char * msgBuffer, int s)
{
	if( send(s, msgBuffer, sizeof(msgBuffer), 0) < 0 )
	{
		fprintf(stderr, "Erro send ()\n");
		exit(4);
	}

	printf("Menssagem enviada: %s\n", msgBuffer);
}

void recvMsg (char * msgBuffer, int s)
{
	/* recebe msg d echo do servidor */

	if(recv(s, msgBuffer, sizeof(msgBuffer), 0) < 0 )
	{
		fprintf(stderr, "Erro receive ()\n");
		exit(5);
	}

	printf("Menssagem recebida: %s\n", msgBuffer);
}

void closeClient (int s)
{
	close(s);
	printf("close () cliente ok\n"); 
	exit(0);
}
