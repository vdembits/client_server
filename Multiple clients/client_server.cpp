#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

#include <sys/errno.h>

#define PORT 4444

using namespace std;

bool f(int x){
    while(x == 0){}
    //sleep(10);
    bool res =  ((x % 5) == 0) ? true : false;
    //cout<<res<<endl;
    return res;
}

bool g(int x){
    //sleep(10);
    bool res = ((x % 2) == 0) ? true : false;
    //cout<<res<<endl;
    return res;
}

int main(){

    int sockfd, ret, status1, status2;
    struct sockaddr_in serverAddr, serverAddr2;
    int newSocket, newSocket2;
    struct sockaddr_in newAddr;
    bool flag = false;

    socklen_t addr_size;

    char buffer[10], buffer2[10];
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
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int enable = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        perror("Error printed by perror");
        printf("[-]Error in binding.\n");
        exit(1);
    }
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    printf("[+]Bind to port %d\n", PORT);

    if(listen(sockfd, 10) == 0){
        printf("[+]Listening....\n");
    }
    else{
        printf("[-]Error in binding 2.\n");
    }


    int child_client, child_client2;
    child_client = fork();
    

    if((child_client) == 0)
    {
        int clientSocket, ret;
        struct sockaddr_in serverAddr;
        char buffer[10];

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(clientSocket < 0){
            printf("[-]Error in connection.\n");
            exit(1);
        }
        printf("[+]Client Socket is created.\n");

        memset(&serverAddr, '\0', sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if(ret < 0){
            printf("[-]Error in connection.\n");
            exit(1);
        }
        printf("[+]Client connected to Server.\n");
        recv(clientSocket, buffer, 10, 0);
        int res = f(atoi(buffer));
        //cout << "\nValue received: "<< atoi(buffer)<<endl;
        string result = to_string(res);
        send(clientSocket, result.c_str(), strlen(result.c_str()), 0);
        exit (1);
    }
    

    else if((child_client2 = fork()) == 0)
    {
        int clientSocket, ret;
        struct sockaddr_in serverAddr;
        char buffer[10];

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if(clientSocket < 0){
            printf("[-]Error in connection.\n");
            exit(1);
        }
        printf("[+]Client Socket 2 is created.\n");

        memset(&serverAddr, '\0', sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(PORT);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if(ret < 0){
            printf("[-]Error in connection.\n");
            exit(1);
        }
        printf("[+]Client 2 connected to Server.\n");
        recv(clientSocket, buffer2, 10, 0);
        int res = g(atoi(buffer2));
        //cout << "\nValue received: "<< atoi(buffer)<<endl;
        string result = to_string(res);
        //cout<<result<<endl;
        send(clientSocket, result.c_str(), strlen(result.c_str()), 0);
        exit (1);
    }
    
    newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
    if(newSocket < 0)
        exit(1);
    printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    newSocket2 = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
    if(newSocket2 < 0)
        exit(1);
    printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
            string value;
            cout << "\nEnter value of X:";
            cin>>value;
            send(newSocket, (value.c_str()), strlen(value.c_str()), 0);
            send(newSocket2, (value.c_str()), strlen(value.c_str()), 0);
            usleep(1000);
             while(waitpid(child_client, &status1, WNOHANG)== 0 || waitpid(child_client2, &status2, WNOHANG) == 0){
                    sleep(1);
                    if (flag == false)
                    {
                        cout << "...Ops! It looks like your function take too much time to calculate a result.\nPress: 0 - to continue, 1 to stop getting messages, -1 - to exit\n";
                        int decision;
                        cin >> decision;
                        if(decision == -1)
                            return 0;
                        if(decision == 1)
                            flag = true;
                    }
                    if (waitpid(child_client, &status1, WNOHANG)== 0 || waitpid(child_client2, &status2, WNOHANG) == 0)
                        sleep(2);
                }
            recv(newSocket, &buffer, 10, 0);
            cout << "\nClient 1 returned: "<< buffer<<endl;
            recv(newSocket2, &buffer2, 10, 0);
            cout << "Client 2 returned: "<< buffer2<<endl;
            bool result = atoi(buffer) || atoi(buffer2);
            cout<< "\nResult: "<< result << "\n";

    exit (1);
}
