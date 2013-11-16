#include "server.h"


int main(void)
{
    int listenfd =0,connfd =0;
    struct sockaddr_in serv_addr;
    char SendBuff[1024];
    int  numrv;  
    
    //create socket
    listenfd = socket(AF_INET,SOCK_STREAM, 0);
    printf("socket success\n");
    memset(&serv_addr,'0',sizeof(serv_addr));
    memset(SendBuff,'0',sizeof(SendBuff));
    

    // set up server's structure.
    serv_addr.sin_family = AF_INET; // connect different machine
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // from any ip address 
    serv_addr.sin_port = htons(5000); // port 5000 

    //start binding.
    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  
    if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
    }

    // loop for accepting client request.
   while(1)
    {
     
      connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL); //         accept awaiting request
      printf("accept success\n ");
     // strcpy(SendBuff, "Message from server");
     // write(connfd, SendBuff, strlen(SendBuff));
      
      
      close(connfd);    
      sleep(1);
    }
 
 
   return 0;
  

}

