/*
 * udp_server.c
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

#define BUFLEN 512  // Max length of buffer
#define PORT 8888   // The port on which to listen for incoming data

void die(char *s) {

    perror(s);
    exit(1);
}

int main(void) {

	// structures describing Internet socket addresses
	struct sockaddr_in si_me, si_other;

    int sfd, slen = sizeof(si_other), recv_len;
    char buf[BUFLEN];

    // create a UDP socket
    if ((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {

    	die("socket()");
    }

    // zero out the sockaddr_in structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET; // AF_INET defined in socket.h
    si_me.sin_port = htons(PORT); // htons() converts uint16_t from host byte order to network byte order
    si_me.sin_addr.s_addr = htonl(INADDR_ANY); // htonl() converts uint32_t from host byte order to network byte order

    // server must bind socket to port (clients don't need bind())
    if (bind(sfd, (struct sockaddr *) &si_me, sizeof(si_me)) == -1) {

    	die("bind()");
    }

    // keep listening for data
    while(1) {

        printf("Waiting for data...\n");
        memset(buf, '\0', BUFLEN);

        // try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(sfd, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1) {

        	die("recvfrom()");
        }

        // print the details of the client and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n", buf);

        // reply to the client with the same data
        if (sendto(sfd, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1) {

        	die("sendto()");
        }
    }

    close(sfd);
    return 0;
}
