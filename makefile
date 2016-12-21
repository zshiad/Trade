all: server_main client_main
server_main: server_main.o server.o
	gcc -o server_main server_main.o server.o  
client_main: client_main.o client.o
	gcc -o client_main client_main.o client.o
server_main.o: server_main.cpp proj1.h
	gcc -c server_main.cpp
client_main.o: client_main.cpp proj1.h
	gcc -c client_main.cpp
server.o: server.cpp
	gcc -c server.cpp
client.o: client.cpp
	gcc -c client.cpp
clean:
	rm *.o
