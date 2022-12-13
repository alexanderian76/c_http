/* Server code */
/* TODO : Modify to meet your need */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
//#include <sys/sendfile.h>

#define PORT_NUMBER     5000
#define SERVER_ADDRESS  "10.0.1.7"
#define FILE_TO_SEND    "test.txt"

int main(int argc, char **argv)
{
        int server_socket;
        int peer_socket;
        socklen_t       sock_len;
        ssize_t len;
        struct sockaddr_in      server_addr;
        struct sockaddr_in      peer_addr;
        int fd, f;
        int sent_bytes = 0;
        char file_size[256];
        struct stat file_stat;
        int offset;
        int remain_data;
        char *message, server_reply[6000];
        off_t *l = 0;

        /* Create server socket */
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1)
        {
                printf( "Error creating socket --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Zeroing server_addr struct */
        memset(&server_addr, 0, sizeof(server_addr));
        /* Construct server_addr struct */
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
        server_addr.sin_port = htons(PORT_NUMBER);

        /* Bind */
        if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
        {
                printf( "Error on bind --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Listening to incoming connections */
        if ((listen(server_socket, 5)) == -1)
        {
                printf( "Error on listen --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        fd = open(FILE_TO_SEND, O_RDONLY);
        if (fd == -1)
        {
                printf( "Error opening file --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Get file stats */
        if (fstat(fd, &file_stat) < 0)
        {
                printf( "Error fstat --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
        close(fd);
        printf( "File Size: \n%d bytes\n", file_stat.st_size);
while(1) {
        sock_len = sizeof(struct sockaddr_in);
        /* Accepting incoming peers */
        peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
        if (peer_socket == -1)
        {
                printf( "Error on accept --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
        printf( "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));

        sprintf(file_size, "%d", file_stat.st_size);


        /* Sending file size */
        len = send(peer_socket, file_size, sizeof(file_size), 0);
        if (len < 0)
        {
              printf("Error on sending greetings --> %s", strerror(errno));

              exit(EXIT_FAILURE);
        }

        printf("Server sent %d bytes for the size\n", len);
        
        offset = 0;
        long long off = 0;
        
        remain_data = file_stat.st_size;
        /* Sending file data */
        
        //f = open(FILE_TO_SEND, O_RDONLY);
        FILE *f = fopen(FILE_TO_SEND, "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        char *string = malloc(fsize + 1);
        fread(string, fsize, 1, f);


        //printf("%d\n", recv(peer_socket, server_reply, BUFSIZ, 0));
        long long i = 0;
        while(i < fsize) {
                int j = 0;
                for(j = 0; j < BUFSIZ; j++) 
                {
                        server_reply[j] = string[i + j];
                }
                i += j;
                send(peer_socket, server_reply, BUFSIZ, 0);
        }
        close(f);
        //send(peer_socket, , BUFSIZ, 0);
      /*  while (((sent_bytes = sendfile(f, peer_socket, off, l, NULL, 0)) > 0) && (remain_data > 0))
        {
                printf("1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
                remain_data -= sent_bytes;
                printf("2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, remain_data);
        }*/
       printf("%s", strerror(errno));
}
        close(peer_socket);
        close(server_socket);
        close(fd);
        return 0;
}