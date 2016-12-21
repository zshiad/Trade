#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>
#include "proj1.h"

Client::Client(const char * hostname,int portno){
  struct hostent *server;
  struct sockaddr_in serv_addr;
  cliprice=(float*)mmap(NULL,sizeof *cliprice,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *cliprice=-1;
  clitime=(time_t*)mmap(NULL,sizeof *clitime,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *clitime=time(NULL);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("ERROR opening socket");
    exit(1);
  }
  server = gethostbyname(hostname); 
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    perror("ERROR connecting");
    exit(1);
  }
}

void Client::get_price(){
  int n;
  char buffer[256],pristri[256];
  float isprice;
  bzero(buffer,256);
  n = read(sockfd,buffer,255);
  if (n < 0) perror("ERROR reading from socket");
  strncpy(pristri,&buffer[1],4);
  pristri[4]=0;
  isprice=atof(pristri);
  if(isprice>0){
  *cliprice=isprice;
  *clitime=time(NULL);
  }
  printf("%s\n",buffer);
}
  
void Client::gen_buy_request(){
  int n;
  char buffer[256];
  time_t *buytime;
  bzero(buffer,256);
  sleep(1);
  getchar();
  *clitime=time(NULL);
  printf("from client: client buy at $%.2f at %.24s\r\n",*cliprice,ctime(&(*clitime)));
  sprintf(buffer,"$%.2f at %.24s\r",*cliprice,ctime(&(*clitime)));
  n = write(sockfd,buffer,strlen(buffer));
  if (n < 0) 
    perror("ERROR writing to socket");

}
  
  
  
  
  
  

