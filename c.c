#include "libClient.h"

extern int errno;
int port;

int main (int argc, char *argv[]) {
  int sock;
  struct sockaddr_in server;

  if (argc != 3) {
    printf ("\n[client] The syntax for connecting is %s <address> <port>.\n", argv[0]);
    return -1;
  }

  port = atoi (argv[2]); srand(time(0));

  errorCheck(sock = socket (AF_INET, SOCK_STREAM, 0), "\n[client] Error creating socket!\n");
  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);

  errorCheck(connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr)), "\n[client] Error connecting to the server!\n");

  system("clear\n");

  printf("\n\n");

  for(int i = 0; i < 24; i++) {
      printf("📖 "); fflush(stdout); usleep(20000);
  }

  welcomeMenu(sock);

  return 0;
}









