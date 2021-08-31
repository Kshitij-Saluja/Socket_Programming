#include<iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unordered_map>
#define PORT 4444
using namespace std;
int main(){

        int sockfd, ret;
         struct sockaddr_in serverAddr;
         unordered_map <string,string> m;
        int newSocket;
        struct sockaddr_in newAddr;

        socklen_t addr_size;

        char buffer[1024];
        pid_t childpid;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0){
                printf("[-]Error in connection.\n");
                exit(1);
        }
        printf("[+]Server Socket is created.\n");

        memset(&serverAddr, '\0', sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if(ret < 0){
                printf("[-]Error in binding.\n");
                exit(1);
        }
        printf("[+]Bind to port %d\n", 4444);
        if(listen(sockfd, 10) == 0){
                printf("[+]Listening....\n");
        }else{
                printf("[-]Error in binding.\n");
        }

        while(1){
                newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
                if(newSocket < 0){
                        exit(1);
                }
                printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

                if((childpid = fork()) == 0){
                        close(sockfd);
                        while(1){
                                recv(newSocket, buffer, 1024, 0);
                                if(strcmp(buffer, ":exit") == 0){
                                        printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                                        break;
                                }else{
					string sending="";
					string check="";
					string ans="";
					char filename[100];
                                        for(int i=0;i<5;i++){
						check.push_back(buffer[i]);
					}
					if(check=="Fetch")
			{
						int k=0;
						for(int i=5;i<1024;i++){
							if(buffer[i]=='t')
								k++;
							ans.push_back(buffer[i]);
							if(k==2)
								break;
						}
					FILE *dem;
					for(int i=0;i<ans.size();i++)
						filename[i]=ans[i];
					filename[ans.size()]='\0';
					dem=fopen(filename,"r");
					if(dem==NULL){
						sending="Not exists";
					}
					else{
						char c=fgetc(dem);
						while(c!=EOF){
							sending.push_back(c);
							c=fgetc(dem);
						}
					}
					}
					else{
						sending="No fetch";
					}
					for(int i=0;i<sending.size();i++)
						buffer[i]=sending[i];
					buffer[sending.size()]='\0';
                                        printf("Client %s %d: %s\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port), buffer);
                                        send(newSocket, buffer, strlen(buffer), 0);
                                        bzero(buffer, sizeof(buffer));
                                }
                        }
                }

        }

        close(newSocket);


        return 0;
}

