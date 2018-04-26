/*
 * udp_client.c
 *
 *  Created on: Apr 25, 2018
 *      Author: Andrew Pineiro
 */

#include <stdio.h> // printf()
#include <string.h> // memset()
#include <stdlib.h> // exit()
#include <arpa/inet.h> // definitions for internet operations
#include <sys/socket.h> // Internet Protocol family
#include <unistd.h> // close()

#define SERVER "127.0.0.1"
#define BUFLEN 512  // Max length of buffer
#define PORT 8888   // The port on which to send data

void die(char *s) {

    perror(s);
    exit(1);
}

int main(void) {

	// structure describing an Internet socket address
    struct sockaddr_in si_other;

    int sfd, slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {

    	die("socket()");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER, &si_other.sin_addr) == 0) {

    	die("inet_aton()");
    }

    while(1) {

        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);

        // send the message
        if (sendto(sfd, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == -1) {

        	die("sendto()");
        }

        // receive a reply and print it
        // clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN);

        // try to receive some data, this is a blocking call
        if (recvfrom(sfd, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) {
            die("recvfrom()");
        }

        puts(buf);
    }

    close(sfd);
	return 0;
}
