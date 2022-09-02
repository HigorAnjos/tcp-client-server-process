#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main( argc, argv)
int argc;
char **argv;
{
	int sockint, s; 
	unsigned short port ; /* numero porta servidor */
	struct sockaddr_in server; /* end servidor */
	char buf[1024]; 
	
	if( argc != 3 ) { 
		fprintf( stderr, "Uso: %s hostname porta\n", argv[0] );
		exit(1);
	}

	port = (unsigned short) atoi(argv[2]); /* porta */

	strcpy(buf, "String enviada para o servidor"); /* messagem */

	fprintf(stderr, "Criando socket TCP\n");
	s = socket(AF_INET, SOCK_STREAM, 0);
	if( s == -1 ) {
		fprintf(stderr, "Erro Socket ()\n");
		exit(2);
	}
	else
		fprintf(stderr, "Socket criado com successo\n");

	server.sin_family = AF_INET; 
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr( argv[1] );

	
	if( connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
		fprintf(stderr, "Erro connect ()\n");
		exit(3);
	}

	printf("Menssagem enviada: %s\n", buf);

	if( send(s, buf, sizeof(buf), 0) < 0 ) {
		fprintf(stderr, "Erro send ()\n");
		exit(4);
	}

	/* recebe msg d echo do servidor */
	if( recv(s, buf, sizeof(buf), 0) < 0 ) {
		fprintf(stderr, "Erro receive ()\n");
		exit(5);
	}
	printf("Menssagem recebida: %s\n", buf);

	close(s);
	printf("close () cliente ok\n"); 
	exit(0);

return (0);
}