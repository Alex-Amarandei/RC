#include "libs.h"

void welcomeMenu(int sock);
void searchMenu(int sock, const char* username);
void listMenu(int sock, const char* username);
void recommendMenu(int sock, const char* username);
void mainMenu(int sock, const char* username);
void loginPasswordClient(int sock, const char* username);
void loginClient(int sock);
void askForPasswordClient(int sock, const char* username);
void signUpClient(int sock);
void quitClient(int sock);
void showCriteria();

void rateBook(int sock, const char* username, const char* book) {
	char buf[BUFFER_SIZE] = "";
	char command[BUFFER_SIZE] = "";
	//char result[BUFFER_SIZE] = "";

	printf("\n\n<rating value (1 to 5)> [%s]: ", username); fflush(stdout);

	fgets(command, BUFFER_SIZE, stdin); trim(command); 

	if(strcmp(command, "/back") == 0) return;
	else if(atoi(command) < 1 || atoi(command) > 5) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }

	sprintf(buf, "RATEBOOK/%s/%s/%s\n", username, book, command);

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending rating to server!\n\n") == 0) return;
	
	strcpy(buf, "");
	
	if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error reading server response!\n\n") == 0) return;

	if(strcmp(buf, "NONE") == 0) {
		printf("\n\n[Sorry] Your request didn't execute successfully.\n\n");
	} else {
		strcpy(buf, ""); strcpy(buf, book); revertApostrophe(buf);
		printf("\n\n[Success] Your book (%s) has been rated successfully.\n\n", buf);
	}
}

void showMore(int sock, const char* username, const char* book) {
	char buf[BUFFER_SIZE] = "";
	char response[LONG_TEXT] = "";

	sprintf(buf, "MORE/%s/%s", username, book);

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending book title to server!\n\n") == 0) return;

	if(errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error reading server response!\n\n") == 0) return;

	if(strcmp(response, "NONE") == 0) {
		printf("\n\n[Sorry] Your request returned no results.\n\n");
	} else {
		printf("\n\n[Let's see] As you requested, %s: \n\n", username);

		printf("%s\n\n", response);
	}
}

void addToReadLater(int sock, const char* username, const char* book) {
	char buf[BUFFER_SIZE] = "";
	int status = isRead(sock, username, book);

	switch(status) {
		case 0: {
			printf("\n\n[Oops] You have already added this book to your list.\n\n");
			return;
		}

		case 1: {
			printf("\n\n[Oops] That's what happens when you read a lot of books, %s. You forget about them!\n\n", username);
			printf("[Oops] You have already read this book.\n\n");
			return; 
		}

		default: break;
	}

	sprintf(buf, "LATER/%s/%s", username, book);

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending book title to server!\n\n") == 0) return;

	strcpy(buf, "");

	if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error reading server response!\n\n") == 0) return;

	if(strcmp(buf, "NONE") == 0) {
		printf("\n\n[Sorry] Your request didn't execute successfully.\n\n");
	} else {
		strcpy(buf, ""); strcpy(buf, book); revertApostrophe(buf);
		printf("\n\n[Success] Your book (%s) has now been added to your ToRead List.\n\n", buf);
	}
}

void addToRead(int sock, const char* username, const char* book) {
	char buf[BUFFER_SIZE] = "";

	sprintf(buf, "TRANSFER/%s/%s", username, book);

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending book title to server!\n\n") == 0) return;

	strcpy(buf, "");

	if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error reading server response!\n\n") == 0) return;

	sprintf(buf, "ALREADY/%s/%s", username, book);

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending book title to server!\n\n") == 0) return;

	strcpy(buf, "");

	if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error reading server response!\n\n") == 0) return;

	//fflush(stdout);

	if(strcmp(buf, "NONE") == 0) {
		printf("\n\n[Sorry] Your request didn't execute successfully.\n\n");
	} else {
		strcpy(buf, ""); strcpy(buf, book); revertApostrophe(buf);
		printf("\n\n[Success] Your book (%s) has now been added to your Read List.\n\n", buf);
	}
}

void downloadBook(int sock, const char* username, const char* book) {
	char buf[BUFFER_SIZE] = "";
	char result[BUFFER_SIZE] = "";

	sprintf(buf, "DOWNLOAD/%s", book);

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending book title to server!\n\n") == 0) return;

	strcpy(buf, "");

	if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error reading server response!\n\n") == 0) return;

	//fflush(stdout);

	if(strcmp(buf, "NONE") == 0) {
		printf("\n\n[Sorry] Your request returned no results.\n\n");
	} else {
		sprintf(result, "open %s\n", buf);

		errorCheck(system(result), "\n\n[client] Error downloading book!\n\n");	

		//sleep(2); 
		//fflush(stdout); 
		addToRead(sock, username, book);
	}
}

void searchTitle(int sock, const char* username) {
	char title[BUFFER_SIZE] = "";
	char toSend[BUFFER_SIZE] = "";
	char buf[LONG_TEXT] = "";
	char command[BUFFER_SIZE] = "";

	while(1) {
		printf("\n\n[ReadsProfiler] Type either the title or the author's name (they may also be incomplete) or type </back> to go back to the Search Menu.\n\n");
		printf("<default search> [%s]: ", username); fflush(stdout);

		fgets(title, BUFFER_SIZE, stdin); trim(title);

		if(strcmp(title, "/back") == 0) return;

		sprintf(toSend, "SEARCH/%s/%s", username, title);

		if(errorCheck(write(sock, toSend, strlen1(toSend)), "\n\n[client] Error sending book title to server!\n\n") == 0) return;

		strcpy(buf, "");

		if(errorCheck(read(sock, buf, LONG_TEXT), "\n\n[client] Error reading server response!\n\n") == 0) return;

		if(strcmp(buf, "NONE") == 0) {
			printf("\n\n[Sorry] Your search returned no results.\n\n");
		} else {
			printf("\n\n[Let's see] As requested, %s:\n\n", username);
			printf("%s\n\n", buf); 

			//fflush(stdout);

			printf("[Choose Menu] You now have the following options:\n\n///////// 1. <more/result_number> (learn more about the book)\n\n///////// 2. <later/result_number> (add book to ReadLater list)\n\n///////// 3. <download/result_number> (download the book)\n\n[Hint] Alternatively you may type </back> to search again.\n\n");

			printf("[%s]: ", username); fflush(stdout);

			fgets(command, BUFFER_SIZE, stdin); trim(command);

			if(strcmp(command, "/back") == 0) continue;

			else if(!(strstr(command, "more/") || strstr(command, "later/")|| strstr(command, "download/"))) printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
			else {
				///// SNIPPET
				int i;
				char *cif = strchr(command, '/') + 1;
				char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
				char *beginning = strstr(buf, toSearch); 

				if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); continue; }

				beginning = beginning + strlen(toSearch);

				char book[BUFFER_SIZE] = "";
				
				for(i = 0; beginning[i + 1] != '('; ) i++; 

				beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

				///// END SNIPPET

				if(strstr(command, "more/")) showMore(sock, username, book);
				else if(strstr(command, "later/")) addToReadLater(sock, username, book);
				else if(strstr(command, "download/")) downloadBook(sock, username, book);
			}
		}
	}
}

void showCriteria() {
	printf("\n\n[Criteria Menu] You can search books by:\n\n///////// 1. title\n\n///////// 2. author\n\n///////// 3. genre\n\n///////// 4. sub-genre\n\n///////// 5. year (will be interpreted as <books written since>)\n\n///////// 6. publisher\n\n///////// 7. rating (will be interpreted as <rating equal or above>)\n\n///////// 8. ISBN (for the connaiseurs 🥸 )\n\n");
	//////
}

void oneFilter(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";
	char buf[BUFFER_SIZE] = "";
	char response[LONG_TEXT] = "";
	bool isRating = 0;

	while(1) {
		showCriteria(); 

		printf("\n\n[ReadsProfiler] Please choose a criteria from the ones above by typing its name.\n\n");
		printf("<search by> [%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "/back") == 0) return;
		else if(!((strcmp(command, "title") == 0) || (strcmp(command, "author") == 0) || (strcmp(command, "genre") == 0) || (strcmp(command, "sub-genre") == 0) || (strcmp(command, "year") == 0) || (strcmp(command, "publisher") == 0) || strcmp(command, "rating") == 0 || (strcmp(command, "isbn") == 0)))
			printf("\n\n[Sorry] That is not a valid criteria. Please type one from the list above or type </back> to get back to the filter menu.\n\n");
		else {
			if(strcmp(command, "rating") == 0) isRating = 1;

			strcpy(buf, ""); sprintf(buf, "ONE/%s/%s/", username, command);

			printf("\n\n<filter by: %s> [%s]: ", command, username); fflush(stdout);

			fgets(command, BUFFER_SIZE, stdin); trim(command);

			if(isRating) if(atoi(command) < 1 || atoi(command) > 5) {
				printf("\n\n[Sorry] Your search returned no results.\n\n"); continue;
			}

			if(strcmp(command, "/back") == 0) return;
			else {
				sprintf(buf + strlen(buf), "%s", command);

				if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending criteria to server!\n\n") == 0) return;
				
				if(errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error reading server response!\n\n") == 0) return;

				if(strcmp(response, "NONE") == 0) {
					printf("\n\n[Sorry] Your search returned no results.\n\n");
				} else {
					printf("\n\n[Let's see] As requested, %s:\n\n", username);
					printf("%s\n\n", response);

					printf("[Choose Menu] You now have the following options:\n\n///////// 1. <more/result_number> (learn more about the book)\n\n///////// 2. <later/result_number> (add book to ToRead list)\n\n///////// 3. <download/result_number> (download the book)\n\n[Hint] Alternatively you may type </back> to search again.\n\n");

					printf("[%s]: ", username); fflush(stdout);
					strcpy(command, "");
					fgets(command, BUFFER_SIZE, stdin); trim(command);

					if(strcmp(command, "/back") == 0) continue;
					else if(!(strstr(command, "more/") || strstr(command, "later/")|| strstr(command, "download/"))) printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
					else {
					///// SNIPPET
						int i;
						char *cif = strchr(command, '/') + 1;
						char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
						char *beginning = strstr(response, toSearch);

						if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); continue; }

						beginning = beginning + strlen(toSearch);

						char book[BUFFER_SIZE] = "";
						
						for(i = 0; beginning[i + 1] != '('; ) i++; 

						beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

						///// END SNIPPET

						if(strstr(command, "more/")) showMore(sock, username, book);
						else if(strstr(command, "later/")) addToReadLater(sock, username, book);
						else if(strstr(command, "download/")) downloadBook(sock, username, book);
					}
				}
			}
		}
	}
}

void multipleFilter(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";
	char buf[BUFFER_SIZE] = "";
	char response[LONG_TEXT] = "";

	while(1) {
		showCriteria(); 

		printf("\n\n[Hint] You will be prompted for values for each of the criteria available.\n\n///////// 1. <value> (type the value you wish to search by)\n\n///////// 2. <enter_key> (if you do not wish to take a certain criteria in consideration)\n\n");

		strcpy(buf, "MULTIPLE");

		printf("\n\n<title> [%s]: ", username); 

		fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);

		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command); 

		printf("\n\n<author> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command);

		printf("\n\n<genre> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command);

		printf("\n\n<sub-genre> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command);

		printf("\n\n<year> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "0");
		sprintf(buf + strlen(buf), "/%s", command);

		printf("\n\n<publisher> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command);

		printf("\n\n<rating> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command);

		printf("\n\n<isbn> [%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);
		if (strcmp(command, "/back") == 0) return;
		else if(strlen(command) == 0) strcpy(command, "%");
		sprintf(buf + strlen(buf), "/%s", command); sprintf(buf + strlen(buf), "/%s", username);

		if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending filters to server!\n\n") == 0) return;
		
		if(errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error reading server response!\n\n") == 0) return;

		if(strcmp(response, "NONE") == 0) {
			printf("\n\n[Sorry] Your search returned no results.\n\n");
		} else {
			printf("\n\n[Let's see] As requested, %s:\n\n", username);
			printf("%s\n\n", response);

			printf("[Choose Menu] You now have the following options:\n\n///////// 1. <more/result_number> (learn more about the book)\n\n///////// 2. <later/result_number> (add book to ToRead list)\n\n///////// 3. <download/result_number> (download the book)\n\n[Hint] Alternatively you may type </back> to search again.\n\n");

			printf("[%s]: ", username); fflush(stdout);

			fgets(command, BUFFER_SIZE, stdin); trim(command);

			if(strcmp(command, "/back") == 0) continue;
			else if(!(strstr(command, "more/") || strstr(command, "later/")|| strstr(command, "download/"))) printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
			else {
				///// SNIPPET
				int i;
				char *cif = strchr(command, '/') + 1;
				char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
				char *beginning = strstr(response, toSearch);

				if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); continue; }

				beginning = beginning + strlen(toSearch);

				char book[BUFFER_SIZE] = "";
				
				for(i = 0; beginning[i + 1] != '('; ) i++; 

				beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

				///// END SNIPPET

				if(strstr(command, "more/")) showMore(sock, username, book);
				else if(strstr(command, "later/")) addToReadLater(sock, username, book);
				else if(strstr(command, "download/")) downloadBook(sock, username, book);
			}
		}
	}
}

void searchFilter(int sock, const char* username) {
	char command[LONG_TEXT] = "";

	while(1) {
		printf("\n\n[Filter Menu] You now have the following options:\n\n///////// 1. one (choose to search by solely a specific criteria)\n\n///////// 2. multiple (filter by multiple criteria)\n\n<filter type> [%s]: ", username); fflush(stdout); 

		fgets(command, LONG_TEXT, stdin); trim(command); 

		if(strcmp(command, "/back") == 0) return;
		else if(strcmp(command, "one") == 0) oneFilter(sock, username);
		else if(strcmp(command, "multiple") == 0) multipleFilter(sock, username);
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}

void showGenres(int sock) {
	char buf[BUFFER_SIZE] = "GENRE/"; cap(buf);
	char genres[LONG_TEXT] = "";

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending genre hierarchy request!\n\n") == 0) return;

	if(errorCheck(read(sock, genres, LONG_TEXT), "\n\n[client] Error retrieving genre hierarchy from server!\n\n") == 0) return;

	printf("\n\n%s\n\n", genres);
}

void searchMenu(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";

	while(1) {
		printf("\n\n[Search Menu] You now have the following options:\n\n///////// 1. default (you will be using the search engine's default settings)\n\n///////// 2. filter (search by different criteria)\n\n///////// 3. show (show the genres hierarchy)\n\n///////// 4. /back (if you wish to go back to the main menu)\n\n");

		printf("\n\n[%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "default") == 0) searchTitle(sock, username);
		else if(strcmp(command, "filter") == 0) searchFilter(sock, username);
		else if(strcmp(command, "show") == 0) showGenres(sock);
		else if(strcmp(command, "/back") == 0) return;
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}

void showReadList(int sock, const char* username) {
	char buf[BUFFER_SIZE] = "";
	char command[BUFFER_SIZE] = "";
	char response[LONG_TEXT] = "";

	sprintf(buf, "SEEREAD/%s", username);

	if(errorCheck(write(sock, buf, BUFFER_SIZE), "\n\n[client] Error requesting ReadList.\n\n") == 0) return;

	if(errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error retrieving ReadList.\n\n") == 0) return;

	if(strcmp(response, "NONE") == 0) {
		printf("\n\n[Sorry] Your request returned no results.\n\n");
	} else {
		printf("\n\n[Let's see] As requested, %s:\n\n", username); printf("%s\n\n", response); 

		printf("\n\n[Book Menu] You now have the following option:\n\n");

		printf("///////// <rate/result_number>\n\n[Hint] Or type </back> to get to the List Menu.\n\n");

		printf("[%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "/back") == 0) return;
		else if(strstr(command, "rate/")) {
			///// SNIPPET
			int i;
			char *cif = strchr(command, '/') + 1;
			char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
			char *beginning = strstr(response, toSearch);

			if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }

			beginning = beginning + strlen(toSearch);

			char book[BUFFER_SIZE] = "";
			
			for(i = 0; beginning[i + 1] != '('; ) i++; 

			beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

			///// END SNIPPET

			rateBook(sock, username, book);
		} else { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }
	}
}

void showToReadList(int sock, const char* username) {
	char buf[BUFFER_SIZE] = "";
	char command[BUFFER_SIZE] = "";
	char response[LONG_TEXT] = "";

	sprintf(buf, "SEENEXT/%s", username);

	errorCheck(write(sock, buf, BUFFER_SIZE), "\n\n[client] Error requesting ToRead List.\n\n");
	
	errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error retrieving ToRead List.\n\n");

	if(strcmp(response, "NONE") == 0) {
		printf("\n\n[Sorry] Your request returned no results.\n\n");
	} else {
		printf("\n\n[Let's see] As requested, %s:\n\n", username);
		printf("%s\n\n", response); fflush(stdout);

		printf("[ToRead Menu] You now have the following option:\n\n///////// <download/result_number> (download the book)\n\n[Hint] Alternatively you may type </back> to get back to the List Menu again.\n\n");

		printf("[%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "/back") == 0) return;
		else if(strstr(command, "download/")) {
		///// SNIPPET
			int i;
			char *cif = strchr(command, '/') + 1;
			char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
			char *beginning = strstr(response, toSearch);

			if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }

			beginning = beginning + strlen(toSearch);

			char book[BUFFER_SIZE] = "";
			
			for(i = 0; beginning[i + 1] != '('; ) i++; 

			beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

			///// END SNIPPET

			downloadBook(sock, username, book);
		} else { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }
	}
}

void listMenu(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";

	while(1) {
		printf("\n\n[List Menu] Choose which of your lists you would like to check out:\n\n///////// 1. read\n\n///////// 2. to_read\n\n[Hint] Or type </back> to get to the main menu.\n\n");

		printf("[%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "read") == 0) showReadList(sock, username);
		else if(strcmp(command, "to_read") == 0) showToReadList(sock, username);
		else if(strcmp(command, "/back") == 0) return;
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}

void myTaste(int sock, const char* username) {
	char buf[BUFFER_SIZE] = ""; sprintf(buf, "MYTASTE/%s", username);
	char response[LONG_TEXT] = "";
	char command[BUFFER_SIZE] = "";

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending recommendation request!\n") == 0) return;

	if(errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error retrieving recommendation!\n") == 0) return;

	if(strcmp(response, "NONE") == 0) {
		printf("\n\n[Sorry] Your search returned no results.\n\n");
	} else {
		printf("\n\n[Let's see] As requested, %s:\n\n", username);
		printf("%s\n\n", response);

		printf("[Choose Menu] You now have the following options:\n\n///////// 1. <more/result_number> (learn more about the book)\n\n///////// 2. <later/result_number> (add book to ToRead list)\n\n///////// 3. <download/result_number> (download the book)\n\n[Hint] Alternatively you may type </back> to search again.\n\n");

		printf("[%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "/back") == 0) return;

		else if(!(strstr(command, "more/") || strstr(command, "later/")|| strstr(command, "download/"))) printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
		else {
			///// SNIPPET
			int i;
			char *cif = strchr(command, '/') + 1;
			char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
			char *beginning = strstr(response, toSearch); 

			if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }

			beginning = beginning + strlen(toSearch);

			char book[BUFFER_SIZE] = "";
			
			for(i = 0; beginning[i + 1] != '('; ) i++; 

			beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

			///// END SNIPPET

			if(strstr(command, "more/")) showMore(sock, username, book);
			else if(strstr(command, "later/")) addToReadLater(sock, username, book);
			else if(strstr(command, "download/")) downloadBook(sock, username, book);
		}
	}
}

void communityTaste(int sock, const char* username) {
	char buf[BUFFER_SIZE] = ""; sprintf(buf, "COMMUNITYTASTE/%s", username);
	char response[LONG_TEXT] = "";
	char command[BUFFER_SIZE] = "";

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending recommendation request!\n") == 0) return;

	if(errorCheck(read(sock, response, LONG_TEXT), "\n\n[client] Error retrieving recommendation!\n") == 0) return;

	if(strcmp(response, "NONE") == 0) {
		printf("\n\n[Sorry] Your search returned no results.\n\n");
	} else {
		printf("\n\n[Let's see] As requested, %s:\n\n", username);
		printf("%s\n\n", response);

		printf("[Choose Menu] You now have the following options:\n\n///////// 1. <more/result_number> (learn more about the book)\n\n///////// 2. <later/result_number> (add book to ToRead list)\n\n///////// 3. <download/result_number> (download the book)\n\n[Hint] Alternatively you may type </back> to search again.\n\n");

		printf("[%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "/back") == 0) return;

		else if(!(strstr(command, "more/") || strstr(command, "later/")|| strstr(command, "download/"))) printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
		else {
			///// SNIPPET
			int i;
			char *cif = strchr(command, '/') + 1;
			char toSearch[100] = ""; sprintf(toSearch, "%s) TITLE: ", cif); 
			char *beginning = strstr(response, toSearch); 

			if(beginning == NULL) { printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n"); return; }

			beginning = beginning + strlen(toSearch);

			char book[BUFFER_SIZE] = "";
			
			for(i = 0; beginning[i + 1] != '('; ) i++; 

			beginning[i] = '\0'; strcpy(book, beginning); replaceApostrophe(book);

			///// END SNIPPET

			if(strstr(command, "more/")) showMore(sock, username, book);
			else if(strstr(command, "later/")) addToReadLater(sock, username, book);
			else if(strstr(command, "download/")) downloadBook(sock, username, book);
		}
	}
}

void recommendMenu(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";

	while(1) {
		printf("\n\n[Recommendations Menu] You now have the following options for receiving a Top 5 Books:\n\n///////// 1. my_taste (the recommendation will be based on your search and reading habits)\n\n///////// 2. community (the recommendation will be based on the community habits)\n\n");

		printf("[%s]: ", username); fflush(stdout); fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "my_taste") == 0) myTaste(sock, username);
		else if(strcmp(command, "community") == 0) communityTaste(sock, username);
		else if(strcmp(command, "/back") == 0) return;
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}

void emoji(int sock, const char* username) {
	char buf[BUFFER_SIZE]= ""; sprintf(buf, "EMOJIS/%d", rand() % 40);
	char received[BUFFER_SIZE] = "", answer[BUFFER_SIZE] = "", response[BUFFER_SIZE] = "";

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error requesting emoji game!\n") == 0) return;

	if(errorCheck(read(sock, received, BUFFER_SIZE), "\n\n[client] Error retrieving emoji game!\n") == 0) return;

	if(strcmp(received, "NONE") == 0) {
		printf("\n\n[Sorry] There have been some technical difficulties. Try again a tad later!\n");
		return;
	}

	char *p = strtok(received, "/"); strcpy(buf, p);
	p = strtok(NULL, "/"); strcpy(answer, p);

	while(1) {
		printf("\n\n[Emoji Game] This is the Emoji Description: %s\n\n", buf);

		printf("[Emoji Game] Can you guess it?\n\n");

		printf("<exact answer> [%s]: ", username); fflush(stdout);

		strcpy(response, ""); fgets(response, BUFFER_SIZE, stdin); trim(response);

		if(strcmp(response, "/back") == 0) return;

		if(strcmp(response, answer) == 0) {
			printf("\n\n[Congratulations] Wow, you did it! 🎉 You are among the very few eager readers talented at this game!\n\n"); 
			return;
		} else {
			printf("\n\n[Oops] It's ok to make mistakes, we are all human after all!\n\n");
			printf("[Solution] The correct answer was: %s\n\n", answer);
			return;
		}
	}
}

void anagram(int sock, const char* username) {
	char buf[BUFFER_SIZE]= ""; sprintf(buf, "ANAGRAM/%d", rand() % 40);
	char received[BUFFER_SIZE] = "", answer[BUFFER_SIZE] = "", response[BUFFER_SIZE] = "";

	if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error requesting anagram game!\n") == 0) return;

	if(errorCheck(read(sock, received, BUFFER_SIZE), "\n\n[client] Error retrieving anagram game!\n") == 0) return;

	if(strcmp(received, "NONE") == 0) {
		printf("\n\n[Sorry] There have been some technical difficulties. Try again a tad later!\n");
		return;
	}

	char *p = strtok(received, "/"); strcpy(buf, p);
	p = strtok(NULL, "/"); strcpy(answer, p);

	while(1) {
		printf("\n\n[Anagram Game] This is the Anagram: %s\n\n", buf);

		printf("[Anagram Game] Can you guess it?\n\n");

		printf("<exact answer> [%s]: ", username); fflush(stdout);

		strcpy(response, ""); fgets(response, BUFFER_SIZE, stdin); trim(response);

		if(strcmp(response, "/back") == 0) return;

		if(strcmp(response, answer) == 0) {
			printf("\n\n[Congratulations] Wow, you did it! 🎉 You are among the few eager readers talented at this game!\n\n"); 
			return;
		} else {
			printf("\n\n[Oops] It's ok to make mistakes, we are all human after all!\n\n");
			printf("[Solution] The correct answer was: %s\n\n", answer);
			return;
		}

	}
}

void gameMenu(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";

	printf("\n\n[Game Menu] Hi, %s! Welcome to the Reader's Arcade! 🎮 \n\n", username);

	while(1) {
		printf("\n\n[Game Menu] You now have the following options:\n\n///////// 1. emoji (guess the book by three emojis)\n\n///////// 2. anagram (guess the book title from a scramble)\n\n");

		printf("\n\n[%s]: ", username); fflush(stdout);

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "emoji") == 0) emoji(sock, username);
		else if(strcmp(command, "anagram") == 0) anagram(sock, username);
		else if(strcmp(command, "/back") == 0) return;
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}

void mainMenu(int sock, const char* username) {
	char command[BUFFER_SIZE] = "";

	printf("\n\n[Welcome Back] Hi, %s! Happy to see you! 🌟 \n\n", username);

	while(1) {
		printf("\n\n[Main Menu] You now have the following options:\n\n///////// 1. search (search for books by title or filter them by different criteria)\n\n///////// 2. lists (see your saved book lists)\n\n///////// 3. recommendations (get a custom Top 5 Books)\n\n///////// 4. games (* NEW *) (something extra)\n\n///////// 5. logout (if you wish to log out of your account)\n\n///////// 6. quit (if you wish to exit the app)\n\n");

		printf("\n\n[%s]: ", username); fflush(stdout); strcpy(command, "");

		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if(strcmp(command, "search") == 0) searchMenu(sock, username);
		else if(strcmp(command, "lists") == 0) listMenu(sock, username);
		else if(strcmp(command, "recommendations") == 0) recommendMenu(sock, username);
		else if(strcmp(command, "games") == 0) gameMenu(sock, username);
		else if(strcmp(command, "logout") == 0) { printf("\n\n[Bye-Bye] See you soon, %s! Happy offline reading! 📚 \n\n", username); return; }
		else if(strcmp(command, "quit") == 0) quitClient(sock);
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}

void askForPasswordClient(int sock, const char* username) {
	char buf[BUFFER_SIZE] = "";
	char password[BUFFER_SIZE] = "";

	printf("\n\n[ReadsProfiler] Lovely! Now, just one step left. Please provide a password that meets the same requirements as your readerID and is at least 4 characters long.\n\n");

	while(1) {
		printf("\n\n[%s] Please enter your desired password: ", username); fflush(stdout);

		fgets(password, BUFFER_SIZE, stdin); trim(password);

		if(strcmp(password, "/back") == 0) return;

		strcpy(buf, ""); sprintf(buf, "NEW/%s/%s", username, password);

		if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending new user data to server!\n\n") == 0) return;

		strcpy(buf, ""); 

		if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error receiving response from server!\n\n") == 0) return;

		if(strcmp(buf, "FORMAT") == 0) {
			printf("\n\n[Sorry] Unfortunately, your password does not meet the aforementioned requirements. Type a password to try again, or type </back> to get to the Welcome Menu.\n\n");
		} else if(strcmp(buf, "CREATED") == 0) {
			printf("\n\n[Sorry] Congratulations, %s! You will now be directed to the Welcome Menu where you can now login to your new account.\n\n", username);
			welcomeMenu(sock);
			return;
		}
	}
}

void loginPasswordClient(int sock, const char* username) {
	char password[BUFFER_SIZE] = "";
	char buf[BUFFER_SIZE] = "";

	while(1) {
		printf("\n\n[%s] Enter your password: ", username); fflush(stdout);

		fgets(password, BUFFER_SIZE, stdin); trim(password);

		if(strcmp(password, "/back") == 0) return;

		sprintf(buf, "PASSWORD/%s/%s", username, password);

		if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending command to the server!\n\n") == 0) return;

		strcpy(buf, ""); 

		if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error receiving result from server!\n\n") == 0) return;

		if(strcmp(buf, "GRANTED") == 0) {
			mainMenu(sock, username);
			return;
		} else {
			printf("\n\n[Sorry] Your password isn't what it used to be. \n\n[Sorry] Type your password to try logging in again, or type </back> to get to the Welcome Menu.\n\n"); 
		}
	}	
}

void loginClient(int sock) {
	char username[BUFFER_SIZE] = "";
	char toSend[BUFFER_SIZE] = "";
	char got[BUFFER_SIZE] = "";

	while(1) {
		printf("\n\n[ReadsProfiler] Enter your readerID: "); fflush(stdout);

		fgets(username, BUFFER_SIZE, stdin); trim(username);
		
		if(strcmp(username, "/back") == 0) return;

		strcpy(toSend, "LOGIN/"); strcat(toSend, username); cap(toSend);

		if(errorCheck(write(sock, toSend, strlen1(toSend)), "\n\n[client] Error sending command to server!\n") == 0) return;

		if(errorCheck(read(sock, got, BUFFER_SIZE), "\n\n[client] Error reading from server!\n") == 0) return;

		if(strcmp(got, "EXISTING") == 0) { 
			loginPasswordClient(sock, username); return; }
		else printf("\n\n[Sorry] The readerID: %s doesn't seem to exist. Perhaps it is just spelled wrong.\n\n[client] Type another readerID to try logging in again, or type </back> to get to the Welcome Menu.\n\n", username);
	}
}

void signUpClient(int sock) {
	char buf[BUFFER_SIZE] = "";
	char username[BUFFER_SIZE] = "";

	printf("\n\n[ReadsProfiler] We are so pleased to see another eager reader join the family! :)\n\n");

	while(1) {
		printf("[Note] * Please note that your readerID can only be formed with alphanumerical characters or underscores\n\n");

		printf("\n\n[ReadsProfiler] Please enter your desired readerID: "); fflush(stdout);

		fgets(username, BUFFER_SIZE, stdin); trim(username);

		if(strcmp(username, "/back") == 0) return;

		strcpy(buf, ""); sprintf(buf, "SIGNUP/%s", username);

		if(errorCheck(write(sock, buf, strlen1(buf)), "\n\n[client] Error sending username to server!\n\n") == 0) return;

		strcpy(buf, ""); 

		if(errorCheck(read(sock, buf, BUFFER_SIZE), "\n\n[client] Error receiving response from server!\n\n") == 0) return;

		if(strcmp(buf, "AVAILABLE") == 0) askForPasswordClient(sock, username);
		else if(strcmp(buf, "FORMAT") == 0) printf("\n\n[Sorry] Unfortunately, your readerID does not meet the aforementioned requirements. Type another readerID to try again, or type </back> to get to the Welcome Menu.\n\n");
		else if(strcmp(buf, "EXISTING") == 0) printf("\n\n[Sorry] Great minds come together all the time. Another eager reader has already taken this exact readerID. Type another readerID to try again, or type </back> to get to the Welcome Menu.\n\n");
	}
}

void quitClient(int sock) {
	close(sock);
	printf("\n\n[Bye-Bye] Thanks for checking us out! See you soon! Happy offline reading! 📚 \n\n");
	exit(0);
}

void welcomeMenu(int sock) {
	char command[BUFFER_SIZE] = "";

	printf("\n\n///////////////////////// ReadsProfiler v1.0 //////////////////////////\n\n");
	printf("[Welcome] Hi there, eager reader! 🤓 Welcome to ReadsProfiler v1.0! 📖 \n\n");

	while(1) {
		printf("\n\n[Welcome Menu] You now have the following options:\n\n///////// 1. login (if you already have an account)\n\n///////// 2. sign-up (if you do not have an account yet)\n\n///////// 3. quit (if you wish to exit the app)\n\n");

		printf("[Note] * Remember that you can type </back> at all times to turn around and switch course!\n\n");

		printf("[user]: "); fflush(stdout);


		fgets(command, BUFFER_SIZE, stdin); trim(command);

		if (strcmp(command, "login") == 0) loginClient(sock);
		else if (strcmp(command, "sign-up") == 0) signUpClient(sock);
		else if (strcmp(command, "quit") == 0) quitClient(sock);
		else printf("\n\n[Sorry] You have entered an ill-typed command, please try again!\n\n");
	}
}