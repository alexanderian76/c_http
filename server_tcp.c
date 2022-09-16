/*
    Bind socket to port 8888 on localhost
*/

#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
 
#pragma comment(lib,"ws2_32.lib") //Winsock Library
 
int main(int argc , char *argv[])
{
    //WSADATA wsa;
    int s , new_socket;
    struct sockaddr_in server , client;
    int c;
    char *message, server_reply[6000];;
 
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
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Bind failed with error code" );
    }
     
    puts("Bind done");
 
    //Listen to incoming connections
    listen(s , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
     
    c = sizeof(struct sockaddr_in);
    new_socket = accept(s , (struct sockaddr *)&client, &c);
    if (new_socket < 0)
    {
        printf("accept failed with error code ");
    }
     
    puts("Connection accepted");

    if( recv(new_socket, server_reply , 6000 , 0) < 0)
	{
		puts("recv failed");
	}

	puts("Reply received\n");
	puts(server_reply);
    close(s);

    //Reply to client
    message = "Hello Client , I have received your connection. But I have to go now, bye\n";
    send(new_socket , message , strlen(message) , 0);

   // getchar();
 
    close(s);

   // closesocket(s);
   // WSACleanup();
     
    return 0;
}