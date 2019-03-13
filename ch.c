#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <time.h> 

#define PORT1s 8081
#define PORT1r 8082
#define PORT2s 8083
#define PORT2r 8084
#define PORT3s 8085
#define PORT3r 8086
#define PORT4s 8087
#define PORT4r 8088

int main(int argc, char const *argv[]) 
{ 
	int server_fds[4];
	int server_fdr[4];

	int new_sockets[4]; 
	int new_socketr[4]; 

	struct sockaddr_in addresss[4]; 
	struct sockaddr_in addressr[4];
	
	int addrlens[4], addrlenr[4];
	for(int i=0; i<4; i++){
		addrlens[i] = sizeof(addresss[i]);
	    addrlenr[i] = sizeof(addressr[i]);
	}
	
	int opts[4]={1,1,1,1}, optr[4]={1,1,1,1}; 	
	
	 
	int presentst=4;
	//printf("hi");
	// Creating socket file descriptor 
	for(int i=0; i<presentst; i++){
		if ((server_fds[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
			perror("socket failed"); 
			exit(EXIT_FAILURE); 
		}

		if ((server_fdr[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
			perror("socket failed"); 
			exit(EXIT_FAILURE); 
		}  
	}
	// Forcefully attaching socket to the port 8080
	for(int i=0; i<presentst; i++){
		if (setsockopt(server_fds[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,	&opts[i], sizeof(opts[i]))){ 
			perror("setsockopt opt"); 
			exit(EXIT_FAILURE); 
		} 
		addresss[i].sin_family = AF_INET; 
		addresss[i].sin_addr.s_addr = INADDR_ANY; 
		addresss[i].sin_port = htons( 8080 + 2*i+1 );
	}
	for(int i=0; i<presentst; i++){
		if (setsockopt(server_fdr[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,	&optr[i], sizeof(optr[i]))){ 
			perror("setsockopt opt"); 
			exit(EXIT_FAILURE); 
		} 
		addressr[i].sin_family = AF_INET; 
		addressr[i].sin_addr.s_addr = INADDR_ANY; 
		addressr[i].sin_port = htons( 8080 + 2*(i+1));
	}

	//printf("hi");
	// Forcefully attaching socket to the port 8080 
	for(int i=0; i<presentst; i++){
		if (bind(server_fds[i], (struct sockaddr *)&addresss[i], sizeof(addresss[i]))<0){ 
			perror("bind failed for address sender"); 
			exit(EXIT_FAILURE); 
		} 
		if (listen(server_fds[i], 3) < 0){ 
			perror("listen failed for address sender"); 
			exit(EXIT_FAILURE); 
		} 
	}
    for(int i=0; i<presentst; i++){
		if (bind(server_fdr[i], (struct sockaddr *)&addressr[i], sizeof(addressr[i]))<0){ 
			perror("bind failed for address reciever"); 
			exit(EXIT_FAILURE); 
		} 
		if (listen(server_fdr[i], 3) < 0){ 
			perror("listen failed for address reciever"); 
			exit(EXIT_FAILURE); 
		} 
	}
	for(int i=0; i<presentst; i++){
		if ((new_sockets[i] = accept(server_fds[i], (struct sockaddr *)&addresss[i], (socklen_t*)&addrlens[i]))<0){ 
			perror("accept failed for address"); 
			exit(EXIT_FAILURE); 
		}
	}
    for(int i=0; i<presentst; i++){
		if ((new_socketr[i] = accept(server_fdr[i], (struct sockaddr *)&addressr[i], (socklen_t*)&addrlenr[i]))<0){ 
			perror("accept failed for address"); 
			exit(EXIT_FAILURE); 
		}
	}
    
    printf("channel\n");
    int p;
    printf("enter p(0-9):");
    scanf("%d",&p);

    char buffer[1024] = {0};
    int valread;
    
    int sender_num = 0;
    int flag=0;
    while(flag==0){
    	valread = read( new_sockets[sender_num] , buffer, 1024); 
	    printf("now in channel: %s\n", buffer);
	    int receiver_num = buffer[4]-48-1;
	    printf("send to station  %d\n",receiver_num);
	    int prob = buffer[6]-48;
	    printf("current probability from station  %d\n", prob);

	    if(prob<=p){
	    	send(new_socketr[receiver_num] , buffer , strlen(buffer) , 0);
	    	valread = read( new_socketr[receiver_num], buffer, 1024);
	    	buffer[0]='1';
	    	buffer[1]='\0';
	        send(new_sockets[sender_num], buffer, strlen(buffer), 0);	
	        flag=1;        
	    }
	    else{
	    	buffer[0]='0';
	    	buffer[1]='\0';
	    	send(new_sockets[sender_num], buffer, strlen(buffer), 0);
	    } 
    }     

    printf("station 1 flushed!!!\n\n\n\n");


    printf("enter p(0-9):");
    scanf("%d",&p);
    sender_num = 1;
    flag=0;
    while(flag==0){
    	valread = read( new_sockets[sender_num] , buffer, 1024); 
	    printf("now in channel: %s\n", buffer);
	    int receiver_num = buffer[4]-48-1;
	    printf("send to station  %d\n",receiver_num);
	    int prob = buffer[6]-48;
	    printf("current probability from station  %d\n", prob);

	    if(prob<=p){
	    	send(new_socketr[receiver_num] , buffer , strlen(buffer) , 0);
	    	valread = read( new_socketr[receiver_num], buffer, 1024);
	    	buffer[0]='1';
	    	buffer[1]='\0';
	        send(new_sockets[sender_num], buffer, strlen(buffer), 0);	
	        flag=1;        
	    }
	    else{
	    	buffer[0]='0';
	    	buffer[1]='\0';
	    	send(new_sockets[sender_num], buffer, strlen(buffer), 0);
	    } 
    }     

    printf("station 2 flushed!!!\n\n\n\n");


    printf("enter p(0-9):");
    scanf("%d",&p);
    sender_num = 2;
    flag=0;
    while(flag==0){
    	valread = read( new_sockets[sender_num] , buffer, 1024); 
	    printf("now in channel: %s\n", buffer);
	    int receiver_num = buffer[4]-48-1;
	    printf("send to station  %d\n",receiver_num);
	    int prob = buffer[6]-48;
	    printf("current probability from station  %d\n", prob);

	    if(prob<=p){
	    	send(new_socketr[receiver_num] , buffer , strlen(buffer) , 0);
	    	valread = read( new_socketr[receiver_num], buffer, 1024);
	    	buffer[0]='1';
	    	buffer[1]='\0';
	        send(new_sockets[sender_num], buffer, strlen(buffer), 0);	
	        flag=1;        
	    }
	    else{
	    	buffer[0]='0';
	    	buffer[1]='\0';
	    	send(new_sockets[sender_num], buffer, strlen(buffer), 0);
	    } 
    }     
	
	printf("station 3 flushed!!!\n\n\n\n");
	return 0; 
} 
