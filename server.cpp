#include <stdio.h>
#include <math.h>
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

Server::Server(int portno){
  struct sockaddr_in serv_addr;
  serprice=(float*)mmap(NULL,sizeof *serprice,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *serprice=(float)(rand()%100);
  attempt=(float*)mmap(NULL,sizeof *attempt,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *attempt=0;
  clientconnect=(float*)mmap(NULL,sizeof *clientconnect,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *clientconnect=0;
  sertime=(time_t*)mmap(NULL,sizeof *sertime,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *sertime=time(NULL);
  present=(time_t*)mmap(NULL,sizeof *present,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *present=time(NULL);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0){
    perror("ERROR opening socket");
    exit(1);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    perror("ERROR on binding");
  int pid=fork();
  if(pid<0){
  perror("ERROR on fork");
  exit(1);
  }
  if(pid==0){
  while(1){
  sleep(1);
  *present=time(NULL);
  if(*present-*sertime>10){
  *serprice=(float)(rand()%100);
  *sertime=time(NULL);
  }
  if(*clientconnect>0.5){
  printf("$%.2f at %.24s\r\n\n",*serprice,ctime(&(*present)));
  }
  } 
  }  
}

int  Server::sock(){return sockfd;}
void Server::gen_price(int sock){
  int n;
  char buffer[256];
  bzero(buffer,256);
  *clientconnect=1;
  sleep(1);
  sprintf(buffer,"$%.2f at %.24s\r\n",*serprice,ctime(&(*present)));
  n = write(sock,buffer,strlen(buffer));
  if (n < 0) 
    perror("ERROR writing to socket");
}
void Server::process_buy_request (int sock){
  int n;
  char buffer[256], pristri[256], receive[256];
  float buyprice;
  int delay;
  bzero(buffer,256);
  n = read(sock,buffer,255);
  if (n < 0) perror("ERROR reading from socket");
  if(buffer[0]=='$'){
  strncpy(pristri,&buffer[1],4);
  strcpy(receive,buffer);
  pristri[4]=0;
  buyprice=atof(pristri);
  bzero(buffer,256);
  delay=rand()%5;
  sleep(delay); 
  *attempt=*attempt+1;
  printf("num buy attemps = %d\n",(int)*(attempt));
  if(buyprice==*serprice){
    sprintf(buffer,"client buy at %s\nserver sell at $%.2f at %.24s\r\nall times approx\nbuy SUCCESSFUL from server!\n",receive,*serprice,ctime(&(*present)));
    n = write(sock,buffer,strlen(buffer));
    if (n < 0) 
      perror("ERROR writing to socket");
  }
  else{
    sprintf(buffer,"client buy at %s\nserver sell at $%.2f at %.24s\r\nall times approx\nbuy FAILED from server!\n",receive,*serprice,ctime(&(*present)));
    n = write(sock,buffer,strlen(buffer));
    if (n < 0) 
      perror("ERROR writing to socket");
  }
  }
}
  
  
