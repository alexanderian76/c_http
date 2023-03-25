/*
    Bind socket to port 8888 on localhost
*/

#include<stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
 
//#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    //WSADATA wsa;
    int s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message, server_reply[6000];
    fd_set readfds;
 
   // printf("\nInitialising Winsock...");
  /*  if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }*/
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) < 0)
    {
        printf("Could not create socket");
    }
 
    printf("Socket created.\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    //inet_addr("10.0.1.70");
    //INADDR_ANY;
    server.sin_port = htons( 8888 );
/*
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("10.0.1.71");
    //inet_addr("10.0.1.70");
    //INADDR_ANY;
    client.sin_port = htons( 5000 );
     */
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Bind failed with error code" );
    }
     
    puts("Bind done");
 
    //Listen to incoming connections
    listen(s , 3);
     
    while(1) {
        FD_ZERO(&readfds);
        FD_SET(s, &readfds);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    for(unsigned long i = 0; i < 6000; i++) {
        server_reply[i] = '\0';
    }
    
    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket < 0)
    {
        printf("accept failed with error code ");
    }
     
    puts("Connection accepted");
    puts(inet_ntoa(client.sin_addr));
    printf("%d", strcmp(inet_ntoa(client.sin_addr), "10.0.1.70"));
if( strcmp(inet_ntoa(client.sin_addr), "127.0.0.1") == 0) {
    if( recv(new_socket, server_reply , 6000 , 0) < 0)
	{
		puts("recv failed");
	}
    
    printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	puts("Reply received\n");
	puts(server_reply);
    //close(s);
    
    //Reply to client
    message = "HTTP1.1/ Header Hello Client , I have received your connection. But I have to go now, bye \n";
    send(new_socket , message , strlen(message) , 0);

    }
    else {
        printf("Packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        puts("Cant recieve from this address\n");
        puts("Reply received (denied)\n");
	    puts(server_reply);
    }
    }
   // getchar();
 
   // close(s);

   // closesocket(s);
   // WSACleanup();
     
    return 0;
}