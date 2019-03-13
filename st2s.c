#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8083 

int main(int argc, char const *argv[]) 
{ 
	struct sockaddr_in address; 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

	char c[8];
	//information to be send
	c[0]='1';
	c[1]='0';
	c[2]='1';
	c[3]='0';
	
	c[4]='3';	  //destination station number 
	c[5]='2';	  //source station number
	
	
	c[7]='\0';
	
	int K=0;
    int flag=0;
    while(K<15 && flag==0){
    	int upper=10;
    	int num = rand()%upper;
		c[6]=(char)(num+48);
		send(sock , c , strlen(c) , 0 );
		printf("%d send data ", K+1);
		valread = read( sock , buffer, 1024); 
    	if(buffer[0]=='1'){
    		flag=1;
    		printf("no collision\n");
    		printf("\nsuccess\n");
    	}
    	else{
    		printf("collision\n");
    	}
    	K++; 
    }
	    
	return 0; 
} 
