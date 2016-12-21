SHI Zhenmei
ID 20329335
ITSC zshiad


Server:
1. Constructor:
   create a server 
   randomly generate price[0.00,99.00] every 10s and print in server konsole every 1s 
2. gen_price:
   send the price to client every 1s
3. process_buy_request:
   deal with buy request and attempt++
   randomly generate a delay[0,4]
   decide whether the client is successful or failed and give feedback to the client 

Client:
1. Constructor:
   create a client set up a connection
2. get_price:
   get information(including price and feedback) from the server and print it
   get price from the information
3. gen_buy_request
   use getchar() to generate a buy request at most 1 time per second 
  
Other point:
1. default port number is 51717
2. use mmap to share price and time in all forks
3. type"server_main 51717"and"client_main csl2wkxx 51717" is ok

There is no extension