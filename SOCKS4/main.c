#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <memory.h>
#include <assert.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#define BACKLOG 25 // the maximum length to which the queue of pend‐
                   // ing  connections for sockfd may grow
#define BUF_SIZE 1024*8

void* prox_routine(void* );
void* exchange(void* );

typedef struct{
    int socket_fd;
    }THREAD_ARG;

/*typedef struct{
	int from_fd;
	int to_fd;
	char* mes;
}DATA_EXCH_ARG;*/

int main()
{
    int prox_fd = socket(AF_INET,SOCK_STREAM,0);
    assert( prox_fd != -1);

    struct sockaddr_in prox_addr;
    bzero( &prox_addr, sizeof(prox_addr));//clears sruct with 0
    prox_addr.sin_family        = AF_INET;
    prox_addr.sin_port          = htons(8080);
    prox_addr.sin_addr.s_addr   = INADDR_ANY;

    assert( bind(prox_fd, (struct sockaddr *) &prox_addr, sizeof(prox_addr)) != -1);
    assert( listen(prox_fd,BACKLOG) == 0);

    while(1)
    {
		printf("Before accept\n");
        int connect_fd = accept( prox_fd, NULL, 0);
		printf("After accept\n");
        pthread_t thread_ID;
        THREAD_ARG* arg = (THREAD_ARG*) calloc(1,sizeof(THREAD_ARG));
        arg->socket_fd  = connect_fd;
        int ret_val     = pthread_create(&thread_ID, NULL, prox_routine, arg);
        assert(ret_val == 0);
        //!exit?
    }

    return 0;
}


void * prox_routine(void *a)
{
    //client connection
    unsigned char buffer[BUF_SIZE];

    int client_fd = ((THREAD_ARG*) a)-> socket_fd;
    free(a);

    int n = recv(client_fd, buffer, BUF_SIZE - 1 , 0);

    if( n<0 )
    {
        perror("ERROR: reading from socket");
        assert(1);
    }

    if( buffer[0] != 0x04 )
    {
        perror("ERROR: wrong socks version, must be 4");
        assert(1);
    }

    uint16_t port_num = buffer[2];
    port_num = port_num*256;
    port_num = port_num | buffer[3];

    int i;
    uint32_t IP_adr = buffer[4];
    for(i=5;i<8;i++){
    IP_adr = IP_adr*256;
    IP_adr = IP_adr | buffer[i];
    }

    printf("\nPORT: %d\n", port_num);
    char* ip = (char*)&IP_adr;
    unsigned char a1 = ip[0], a2 = ip[1], a3 = ip[2], a4 = ip[3];
	printf("IP: %u.%u.%u.%u\n", a1, a2, a3, a4);

    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    assert( server_fd != -1);

    struct sockaddr_in server_addr;
    bzero( &server_addr, sizeof(server_addr));
    server_addr.sin_family         = AF_INET;
    server_addr.sin_port           = htons(port_num);
    server_addr.sin_addr.s_addr    = htonl(IP_adr);


    int res = connect(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

	printf("connection result %d\n", res);

    bzero(buffer,BUF_SIZE);
    if( res == -1 )
        buffer[1] = 0x5b;
    if( res == 0 )
        buffer[1] = 0x5a;

    send(client_fd,buffer, 8, 0);

    //!data exchange
    int if_readable;
    int stream_check;
    while(1)
    {
        //!recieve from client
        if_readable = 0;

        res = ioctl(client_fd, FIONREAD, &if_readable);
        if( res == -1 ){
            perror("Error in ioctl function");
            assert(1);
            }

        if(if_readable)
        {
            stream_check = recv(client_fd, buffer, BUF_SIZE, 0);
            printf("recieved from client %d", stream_check);
            if( stream_check <= 0){
                    perror("Error: recieving from client or socket closed");
					break;
                }
            stream_check = send(server_fd, buffer, stream_check, 0);
            if( stream_check <= 0){
                    perror("Error: sending to server or socket closed");
					break;
                }
			printf("sent to server %d\n", stream_check);
        }

        //!recieve from server
        if_readable = 0;

        res = ioctl(server_fd, FIONREAD, &if_readable);
        if( res == -1 ){
            perror("Error in ioctl function");
            assert(1);
            }

        if(if_readable)
        {
            stream_check = recv(server_fd, buffer, BUF_SIZE, 0);
            printf("recieved from server %d", stream_check);
            if( stream_check <= 0){
                    perror("Error: recieving from client or socket closed");
					break;
                }
            stream_check = send(client_fd, buffer, stream_check, 0);
            if( stream_check <= 0){
                    perror("Error: sending to client or socket closed");
					break;
                }
			printf("sent to client %d\n", stream_check);
        }
    }

    close(server_fd);
    close(client_fd);
    return NULL;
}


