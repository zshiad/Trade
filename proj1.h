#ifndef PROJ1_H
#define PROJ1_H
class Client{
  int sockfd;
  float *cliprice;
  time_t *clitime;
public:
  Client(const char * hostname,int portno=51717);
  void get_price();
  void gen_buy_request ();
};


class Server{
  int sockfd;
  float *serprice;
  float *attempt;
  time_t *sertime;
  time_t *present;
  float *clientconnect;
  

public:
  Server(int partno=51717);
  int sock();
  void gen_price(int sock);
  void process_buy_request (int sock);
};


#endif
