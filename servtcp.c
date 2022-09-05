#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int openAtcpNetworkConnection()
{
	fprintf(stderr, "Criando socket TCP\n");

	int connection = socket(AF_INET, SOCK_STREAM, 0);
	
	if( connection == -1 )
	{
		fprintf(stderr, "ERRO em socket()\n");
		exit(2);
	}

	fprintf(stderr, "Socket criado com sucesso\n");

	return connection;
}

void recvMsg (char * msgBuffer, int ns) 
{
	bzero(msgBuffer, 1024);

	if( recv(ns, msgBuffer, sizeof(msgBuffer), 0) == -1 )
	{ 
		fprintf(stderr, "Erro em recv()\n");
		exit(7);
	}

}

int main(void)
{
	// mudar s -> sockint
	int s; /* descritor do socket */
	int namelen; /* comprimento do nome do cliente */
	int ns; /* socket cliente */
	struct sockaddr_in client; /* informacao do end do cliente*/
	struct sockaddr_in server; /* informacao do end do servidor*/
	char buf[1024]; /* buffer de dados */

	s = openAtcpNetworkConnection();


	server.sin_family = AF_INET;
	server.sin_port = 0; /* usa a primeira porta disponivel */
	server.sin_addr.s_addr = INADDR_ANY;

	if(bind(s, (struct sockaddr *)&server, sizeof( server )) < 0 )
	{ 
		fprintf(stderr,"Erro em bind()\n");
		exit(3);
	}

	/* descobre a porta associada*/
	namelen = sizeof( server );
	if(getsockname( s, (struct sockaddr *) &server, &namelen) < 0 )
	{
		fprintf(stderr, "Erro em getsockname()\n");
		exit(4);
	}

	fprintf(stderr,"Numero de porta : %d\n", ntohs( server.sin_port));

	if(listen(s, 1) != 0)
	{ 
		fprintf(stderr, "Erro em listen()\n");
		exit(5);
	}

	while (1 && (buf[0] != 'E'))
	{
		namelen = sizeof(client); 
		ns = accept(s, (struct sockaddr *)&client, &namelen);
		if( ns == -1)
		{
			fprintf(stderr, "Erro em accept()\n");
			exit(6);
		}

		recvMsg(buf, ns);
		
		printf("\n%s", buf);

		if( send( ns, buf, sizeof(buf), 0) < 0)
		{
			fprintf(stderr, " Erro em send()\n");
			exit(8);
		}
	}
       

	close(ns); /* close socket cliente */
	close(s); /* close  socket servidor*/

	printf("Servidor finalizado\n");
	exit(0);
	return (0);
}

