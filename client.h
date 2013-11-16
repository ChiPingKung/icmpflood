#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <arpa/inet.h>
 
#define BUFFER_SIZE 400
#define PACKET_DELAY_USEC 30
#define DEF_NUM_PACKETS 100

void SetupLayer(struct icmphdr* icmp,struct ip* ip);
void SetSocketOption(int s);


#endif
