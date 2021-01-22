#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <mysql.h>
#include <stdbool.h>

#define PORT 9573
#define MAX_BEFORE_REJECT 200
#define BUFFER_SIZE 1000
#define LONG_TEXT 131072
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

extern int errno;

void scrambleWord(const char* initial, char* final) {
    int length = strlen(initial), poz = 0;

    for(int i = 0; i < length; i++)
        final[i] = '_';

    for(int i = 0; i < length; i++) {
        poz = rand() % length;
        while(final[poz] != '_') {
            if(poz > 0) poz--;
            else break;
        }
        

        if(final[poz] != '_') {
            poz = length - 1;
            while(final[poz] != '_') poz--;
        }

        final[poz] = initial[i];
    }
}

int strlen1(const char* s) {
  return strlen(s) + 1;
  ///
}

void debug(const char* s) {
  printf("\n\nFROM DEBUGGER: %s||\n\n", s);
  ///
}

int errorCheck(int returnCode, const char* message) {
  //printf("Cod: %d\n\n", returnCode);

  if(returnCode < 0) {
    perror(message); exit(errno);
  }

  return returnCode;
}

int errorDBCheck(const void* returnCode, const char* message) {
	if(returnCode == NULL) {
		perror(message); exit(errno);
	}
	return 0;
}

char * convertAddress (struct sockaddr_in address) {
  static char str[25];
  char port[7];

  strcpy (str, inet_ntoa (address.sin_addr)); 
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));  
  strcat (str, port);
  return (str);
}

char* trim(char* string) {
	string[strlen(string) - 1] = '\0';
	return string;
}

char* cap(char* string) {
	string[strlen(string)] = '\0';
	return string;
}

void replaceApostrophe(char* s) {
  char aux[BUFFER_SIZE] = "";
  int i, j, length = strlen(s); 

  for(i = 0, j = 0; i < length; i++, j++)
    if(s[i] == '\'') {
      aux[j++] = '\\'; aux[j] = '\'';
    } else aux[j] = s[i];

  cap(aux); strcpy(s, aux);
}

void revertApostrophe(char* s) {
  char aux[BUFFER_SIZE] = "";
  int i, j, length = strlen(s); 

  for(i = 0, j = 0; i < length; i++, j++)
    if(s[i] == '\\' && s[i + 1] == '\'') {
      j--;
    } else aux[j] = s[i];

  cap(aux); strcpy(s, aux);
}

bool goodFormat(char* p) {
	int lg = strlen(p);

	for(int i = 0; i < lg; i++)
		if(!((p[i] >= 'a' && p[i] <= 'z') || (p[i] >= 'A' && p[i] <= 'Z') || (p[i] >= '0' && p[i] <= '9') || (p[i] == '_') || (p[i] == '-'))) return false;
	return true;
}

void wrongFormat(int fd) {
	char buf[BUFFER_SIZE] = "";

	strcpy(buf, "FORMAT");

	errorCheck(write(fd, buf, BUFFER_SIZE), "\n\n[server] Error sending format mistake message!\n\n");
}

////////////////////////////////////////////////////////////////////////////

int getRating(MYSQL* conn, const char* buff) {
  char query[BUFFER_SIZE] = "";
  char buf[BUFFER_SIZE] = ""; strcpy(buf, buff); 
  if(!strchr(buf, '\\')) replaceApostrophe(buf);
  int rowsNumber = 0;
  MYSQL_RES *result;
  MYSQL_ROW row;

  sprintf(query, "SELECT FLOOR(AVG(l.rating)), b.title FROM lists l JOIN books b ON l.isbn = b.isbn GROUP BY l.isbn HAVING LOWER(b.title) = LOWER('%s');", buf);

  errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for rating!\n");

  errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

  rowsNumber = mysql_num_rows(result);

  if(rowsNumber == 1) {
    row = mysql_fetch_row(result);
    mysql_free_result(result); 

    return atoi(row[0]);
  } 

  mysql_free_result(result);
  return 0;
}

int isRead(int sock, const char* username, const char* book) {
  char buf[BUFFER_SIZE] = "";

  sprintf(buf, "STATUS/%s/%s", username, book);

  if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending book title to server!\n\n") == 0) return 0;

  strcpy(buf, "");

  if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error reading server response!\n\n") == 0) return 0;

  if(strcmp(buf, "READ") == 0) return 1;
  else if(strcmp(buf, "LATER") == 0) return 0;
  return 2;
}

