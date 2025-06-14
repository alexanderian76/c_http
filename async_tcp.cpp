/*
    Bind socket to port 8888 on localhost
*/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <vector> // подключили библиотеку
#include <thread>
#include <signal.h>

// #pragma comment(lib,"ws2_32.lib") //Winsock Library

class arg_struct
{

public:
    arg_struct(int sock, struct sockaddr_in cl)
    {
        new_socket = sock;
        client = cl;
    }
    int new_socket;
    struct sockaddr_in client;
};

ssize_t write_nosigpipe(int fd, void *buf, size_t len)
{
    sigset_t oldset, newset;
    ssize_t result;
    siginfo_t si;
    struct timespec ts = {0};

    sigemptyset(&newset);
    sigaddset(&newset, SIGPIPE);
    sigaddset(&newset, SIGABRT);
    pthread_sigmask(SIG_BLOCK, &newset, &oldset);

    result = send(fd, buf, len, 0);

    while (sigtimedwait(&newset, &si, &ts) >= 0 || errno != EAGAIN)
        ;
    pthread_sigmask(SIG_SETMASK, &oldset, 0);

    return result;
}
std::string getRoute(std::string reply)
{
    int pos = reply.find_first_of("GET");
    std::cout << pos << std::endl;
    if (pos < 0 || pos + 4 > reply.length())
        return "";
    std::cout << reply.substr(pos + 4).find_first_of(' ', pos + 4) << std::endl;
    std::cout << reply << std::endl;
    std::string route = reply.substr(pos + 4, reply.find_first_of(' ', pos + 4) - pos - 4);

    return route;
}

void response(int new_socket, struct sockaddr_in client)
{
    // arg_struct args = arguments;
    //
    char *message, server_reply[6000];
    {
        /* data */
    };
    puts("CLIENT: ");

    if (new_socket < 0)
    {
        printf("accept failed with error code ");
    }

    puts("Connection accepted");
    puts(inet_ntoa(client.sin_addr));
    printf("%d", strcmp(inet_ntoa(client.sin_addr), "10.0.1.70"));
    if (strcmp(inet_ntoa(client.sin_addr), "127.0.0.1") == 0)
    {
        if (recv(new_socket, server_reply, 6000, 0) < 0)
        {
            puts("recv failed");
        }

        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        puts("Reply received\n");
        puts(server_reply);
        std::string route = getRoute(server_reply);
        std::cout << getRoute(server_reply) << std::endl;
        std::string combined;
        if (route == "/home")
        {
            message = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: ";
            std::string rep = std::string("Hello from home route ") + inet_ntoa(client.sin_addr) + ":" + std::to_string(ntohs(client.sin_port));
            combined = std::string(message) + std::to_string(strlen(rep.c_str())) + "\r\n\r\n" + rep;
        }
        else if (route == "/mac")
        {
            message = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: ";
            std::string str = "<div style='width: 100%; height: 100%; text-align: center; vertical-align: center;'><button style='padding: 10px; color: green; background-color: gray;' onclick='fetch(`http://localhost:1234`).then(() => console.log(123))'>Hello</button></div>";
            std::string rep = str + inet_ntoa(client.sin_addr) + ":" + std::to_string(ntohs(client.sin_port));
            combined = std::string(message) + std::to_string(strlen(rep.c_str())) + "\r\n\r\n" + rep;
        }
        else
        {

            // close(s);
            // usleep(3000000);
            // Reply to client
            message = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: ";
            //"\r\n\r\n<html><body>Hello World</body></html>";
            std::string rep = std::string(server_reply) + " " + inet_ntoa(client.sin_addr) + ":" + std::to_string(ntohs(client.sin_port));
            std::cout << "REP" << std::endl;
            std::cout << rep << std::endl;
            std::cout << std::to_string(strlen(rep.c_str())) << std::endl;
            combined = std::string(message) + std::to_string(strlen(rep.c_str())) + "\r\n\r\n" + rep;
        }
        // message = message + "<html><body>Hello World";
        // combined = combined + "</body></html>";

        // message = "HTTP1.1/ 200 OK; Header content-type: text/html Hello Client , I have received your connection. But I have to go now, bye \n";
        // send(new_socket , message , strlen(message) , 0);
        int numSent;
        unsigned char *pdata = (unsigned char *)combined.c_str();
        int datalen = strlen(combined.c_str());

        std::cout << new_socket << " pdata: " << pdata << "\n datalen: " << datalen << std::endl;
        int error_code;
        socklen_t error_code_size = sizeof(error_code);
        int status = getsockopt(new_socket, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
        std::cout << "Status: " << status << std::endl;
        std::cout << "Code: " << error_code << std::endl;
        if (status == 0)
        {
            ssize_t r = write_nosigpipe(new_socket, pdata, datalen);

            std::cout << "Отправлено: " << r << std::endl;
        }
        close(new_socket);

        /* while (datalen > 0) {
           numSent =
           printf("NUMSENT: %d", numSent);
           if (numSent == -1) return -1;
           pdata += numSent;
           datalen -= numSent;
         }
         */
    }
    else
    {
        // без recv нельзя через сокет отправить назад будет
        if (recv(new_socket, server_reply, 6000, 0) < 0)
        {
            puts("recv failed");
        }
        printf("Packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        puts("Cant recieve from this address\n");
        puts("Reply received (denied)\n");
        puts(server_reply);
        std::string deniedString = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: 7\r\n\r\nDENIED!";
        unsigned char *pdata = (unsigned char *)deniedString.c_str();
        int datalen = strlen(deniedString.c_str());
        send(new_socket, pdata, datalen, 0);
    }
    // new_socket = NULL;
}

int main(int argc, char *argv[])
{
    // WSADATA wsa;
    int s, new_socket;
    struct sockaddr_in server;
    struct sockaddr_in client;

    fd_set readfds;

    // printf("\nInitialising Winsock...");
    /*  if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
      {
          printf("Failed. Error Code : %d",WSAGetLastError());
          return 1;
      }*/

    printf("Initialised.\n");

    // Create a socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Could not create socket");
    }

    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    // inet_addr("10.0.1.70");
    // INADDR_ANY;
    server.sin_port = htons(1234);
    /*
        client.sin_family = AF_INET;
        client.sin_addr.s_addr = inet_addr("192.168.1.74");
        //inet_addr("10.0.1.70");
        //INADDR_ANY;
        client.sin_port = htons( 5000 );
         */
    // Bind
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Bind failed with error code");
    }

    puts("Bind done");

    // Listen to incoming connections
    listen(s, 3);
    // std::vector<arg_struct> args;

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(s, &readfds);

        int c;

        // Accept and incoming connection
        puts("Waiting for incoming connections...");
        /* for(unsigned long i = 0; i < 6000; i++) {
             server_reply[i] = '\0';
         }
         */
        c = sizeof(struct sockaddr_in);
        struct sockaddr_in cl;
        new_socket = accept(s, (struct sockaddr *)&cl, (socklen_t *)&c);
        // pthread_t some_thread;
        int sock;
        // arg_struct struc = ;

        memcpy(&sock, &new_socket, (size_t)sizeof(new_socket));
        // memcpy(&cl, &client, (size_t)sizeof(client));

        // тут использовался массив объектов с сокетами и клиентами, как будто бы и без него работает, но это не точно

        // args.push_back(arg_struct(sock, cl));
        arg_struct a = arg_struct(sock, cl);
        // args.back();
        //  args.pop_back();
        std::thread thr(response, a.new_socket, a.client);
        thr.detach();
        /* if(args1.new_socket == NULL) {
         args1.new_socket = sock;
         args1.client = cl;
         pthread_create(&some_thread, NULL, response, (void *)&args1);
         if(args1.new_socket != NULL && args2.new_socket != NULL) {
             pthread_join(&some_thread, NULL);
         }
         }
         else {
             args2.new_socket = sock;
         args2.client = cl;
         pthread_create(&some_thread, NULL, response, (void *)&args2);
         if(args1.new_socket != NULL && args2.new_socket != NULL) {
             pthread_join(&some_thread, NULL);
         }
         }*/
    }
    // getchar();

    // close(s);

    // closesocket(s);
    // WSACleanup();

    return 0;
}