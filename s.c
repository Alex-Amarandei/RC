#include "libServer.h"

int main () {
    SA_IN server; 
    SA_IN client;
    fd_set readDescriptors, activeDescriptors;    
    struct timeval timeout;    
    int serverSocket, newClient, fd, numberOfDescriptors, nread, optionValue = 1;
    unsigned int length;

    MYSQL *conn = mysql_init(NULL); srand(time(0));

    errorDBCheck(conn, "[server] Error establishing connection to database!\n");

    errorDBCheck(mysql_real_connect(conn, "localhost", "root", "7345MySql35", "readsprofiler", 3306, NULL, 0), "[server] Error connecting to the database!\n"); 

    errorCheck(serverSocket = socket (AF_INET, SOCK_STREAM, 0), "\n[server] Error creating socket!\n");
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue));

    memset (&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    errorCheck(bind (serverSocket, (struct sockaddr *) &server, sizeof (struct sockaddr)), "\n[server] Error binding!\n");

    errorCheck(listen (serverSocket, MAX_BEFORE_REJECT), "\n[server] Error listening!\n");
  
    FD_ZERO (&activeDescriptors);
    FD_SET (serverSocket, &activeDescriptors);
  
    numberOfDescriptors = serverSocket;

    system("clear\n");

    for(int i = 0; i < 24; i++) {
        printf("🖥  "); fflush(stdout); usleep(20000);
    }

    printf("\n\n[server] Waiting for eager readers at port %d! Don't be shy!\n\n", PORT);
        
    while (1) {
        timeout.tv_sec = 1; timeout.tv_usec = 0; //waitpid(-1, NULL, WNOHANG);
      
        memcpy((char *) &readDescriptors, (char *) &activeDescriptors, sizeof(readDescriptors));

        errorCheck(select (numberOfDescriptors + 1, &readDescriptors, NULL, NULL, &timeout), "\n\n[server] Error selecting!\n");
    
        if (FD_ISSET (serverSocket, &readDescriptors)) {
            length = sizeof(client); memset(&client, 0, sizeof(client));

            newClient = accept (serverSocket, (struct sockaddr *) &client, &length);

            if (newClient < 0) {
                perror("\n\n[server] Error accepting new client!\n\n"); continue;
            }

            if (numberOfDescriptors < newClient) numberOfDescriptors = newClient;
            
            FD_SET (newClient, &activeDescriptors);

            printf("[server] Reader #%d from %s is now online!\n\n", newClient, convertAddress(client));
            fflush (stdout);
        }
        
        for(fd = 0; fd <= numberOfDescriptors; fd++) {
            if(FD_ISSET(fd, &readDescriptors) && fd != serverSocket) {
                ioctl(fd, FIONREAD, &nread);
                if(nread == 0) {
                    close(fd);
                    FD_CLR(fd, &activeDescriptors);
                    printf("[server] Handled reader #%d's request.\n\n", fd);
                }
                else {
                    pid_t pid;
                    errorCheck(pid = fork(), "\n[server] Error forking!\n");
                    if(pid == 0) {
                        handle(fd, conn); close(newClient);
                        exit(-1);
                    }
                }
            }
        }
          
    }       
}       