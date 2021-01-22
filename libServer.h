#include "libs.h"

void getISBN(MYSQL* conn, const char* buff, char* isbn);

int getUserRating(MYSQL* conn, const char* username, const char* buff) {
	char book[BUFFER_SIZE] = ""; strcpy(book, buff); replaceApostrophe(book);
    char isbn[BUFFER_SIZE] = "", response[BUFFER_SIZE] = ""; getISBN(conn, book, isbn); 
    char query[BUFFER_SIZE] = ""; sprintf(query, "SELECT rating FROM lists WHERE isbn = '%s' AND username = '%s';", isbn, username);
    int rowsNumber = 0;
    MYSQL_RES *result;
    MYSQL_ROW row;

    errorCheck(mysql_query(conn, query), "\n\n[server] Error requesting user rating!\n");

    errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query results!\n");

    rowsNumber = mysql_num_rows(result);

    if(rowsNumber == 1) {
		row = mysql_fetch_row(result); 

		strcpy(response, row[0] ? row[0] : "0"); 
		mysql_free_result(result); 

      return atoi(response);
    } 

    mysql_free_result(result);
    return 0;
}

void updatePoints(MYSQL* conn, const char* username, const char* isbn, int increment) {
	char query[BUFFER_SIZE] = ""; 

	sprintf(query, "INSERT INTO `book_points` (`username`, `isbn`, `points`) VALUES ('%s', '%s', `points` + %d) ON DUPLICATE KEY UPDATE `points` = `points` + %d;", username, isbn, increment, increment);

	errorCheck(mysql_query(conn, query), "\n\n[server] Error updating book points!\n");
}

void alreadyRead(int fd, MYSQL* conn, const char* buff) {
	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
	char toUser[BUFFER_SIZE] = "";
	char book[BUFFER_SIZE] = "";
	char username[BUFFER_SIZE] = "";
	char isbn[BUFFER_SIZE] = ""; 
	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char *p = strtok(strchr(buf, '/') + 1, "/"); 
	strcpy(username, p); p = strtok(NULL, "/"); strcpy(book, p);
	getISBN(conn, book, isbn);

	strcpy(buf, ""); sprintf(buf, "SELECT type FROM lists l JOIN clients c ON l.username = c.username WHERE l.username = '%s' AND l.isbn = '%s';", username, isbn); 

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error checking read status!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result);

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result);
		if(strcmp(row[0], "read") == 0)
			strcpy(toUser, "READ");
		else strcpy(toUser, "LATER");

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;

	} else {
		strcpy(toUser, "NO");

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void usernameExists(int fd, MYSQL* conn, const char* buff) {
	char fromUser[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strchr(buf, '/') + 1; strcpy(fromUser, p);

	strcpy(buf, ""); sprintf(buf, "SELECT username FROM clients WHERE username = '%s'", fromUser);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error verifying username existence!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result);

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result);
  		if(strcmp(row[0], fromUser) == 0) {
				strcpy(toUser, "EXISTING"); 
			} else strcpy(toUser, "AVAILABLE");

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "AVAILABLE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void checkPassword(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char password[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[LONG_TEXT] = ""; strcpy(buf, buff);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p);
	p = strtok(NULL, "/"); strcpy(password, p); 

	strcpy(buf, ""); sprintf(buf, "SELECT password FROM clients WHERE username = '%s'", username);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error verifying password matching!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); 

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result);
  		if(strcmp(row[0], password) == 0) {
				strcpy(toUser, "GRANTED"); 
			} else strcpy(toUser, "DENIED");

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "DENIED"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void newPassword(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char password[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p);
	p = strtok(NULL, "/"); strcpy(password, p); 

	if(!goodFormat(password) || strlen(password) < 4) {
		wrongFormat(fd); return;
	}

	strcpy(buf, ""); sprintf(buf, "INSERT INTO `clients` VALUES('%s', '%s')", username, password);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error creating user!\n");

	//errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	strcpy(toUser, "CREATED"); 

	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void searchDefault(int fd, MYSQL* conn, const char* buff) {
	char title[BUFFER_SIZE] = "", username[BUFFER_SIZE] = "";
  	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1;
  	int previousRating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strtok(strchr(buf, '/') + 1, "/"); strcpy(username, p); 
	p = strtok(NULL, "/"); 
	if(p) strcpy(title, p); 

	strcpy(buf, ""); sprintf(buf, "SELECT b.title, b.year, a.first_name, a.last_name, b.isbn FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE LOWER(b.title) IN (SELECT b.title FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE LOWER(b.title) LIKE LOWER('%%%s%%') OR LOWER(a.first_name) LIKE LOWER('%%%s%%') OR LOWER(a.last_name) LIKE LOWER('%%%s%%') ORDER BY b.title) ORDER BY b.title;", title, title, title);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); 

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(previousRating != -1)
					sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);

		    	strcpy(previous, row[0]); previousRating = getRating(conn, row[0]);
		    	updatePoints(conn, username, row[4], 1);
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating); mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void searchOneFilter(int fd, MYSQL* conn, const char* buff) {
	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "";
  	char criteria[BUFFER_SIZE] = "";
  	char value[BUFFER_SIZE] = "";
  	char query[BUFFER_SIZE] = "", username[BUFFER_SIZE] = "";
  	char buf[LONG_TEXT] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1, previousRating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p); 
	p = strtok(NULL, "/"); if(p) strcpy(criteria, p);
	p = strtok(NULL, "/"); if(p) strcpy(value, p);

	sprintf(query, "SELECT b.title, b.year, a.first_name, a.last_name, b.isbn FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE LOWER(b.title) IN (SELECT DISTINCT b.title FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code JOIN book_genres bg ON b.isbn = bg.isbn JOIN genres g ON bg.sub_genre = g.sub_genre JOIN publishers pub ON b.pub_id = pub.pub_id WHERE ");

	if(strcmp(criteria, "title") == 0) sprintf(query + strlen(query), "LOWER(b.title) LIKE LOWER('%%%s%%')", value);
	else if(strcmp(criteria, "author") == 0) sprintf(query + strlen(query), "LOWER(a.first_name) LIKE LOWER('%%%s%%') OR LOWER(a.last_name) LIKE LOWER('%%%s%%')", value, value);
	else if(strcmp(criteria, "genre") == 0) sprintf(query + strlen(query), "LOWER(g.genre) LIKE LOWER('%%%s%%')", value);
	else if(strcmp(criteria, "sub-genre") == 0) sprintf(query + strlen(query), "LOWER(g.sub_genre) LIKE LOWER('%%%s%%')", value);
	else if(strcmp(criteria, "year") == 0) sprintf(query + strlen(query), "b.year >= '%s'", value);
	else if(strcmp(criteria, "publisher") == 0) sprintf(query + strlen(query), "LOWER(pub.name) LIKE LOWER('%%%s%%')", value);
	else if(strcmp(criteria, "rating") == 0) sprintf(query + strlen(query), "b.title IN (SELECT b.title FROM lists l JOIN books b ON l.isbn = b.isbn GROUP BY l.isbn HAVING AVG(l.rating) >= %d)", atoi(value));
	else sprintf(query + strlen(query), "b.isbn LIKE LOWER('%%%s%%')", value);

	sprintf(query + strlen(query), " ORDER BY b.title) ORDER BY b.title;");

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); 
 
	strcpy(toUser, "");

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(previousRating != -1)
					sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);

		    	strcpy(previous, row[0]); previousRating = getRating(conn, row[0]); updatePoints(conn, username, row[4], 2);
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating); mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void searchMultipleFilter(int fd, MYSQL* conn, const char* buff) {
	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "";
  	char criteria[BUFFER_SIZE] = "";
  	char query[LONG_TEXT] = "", username[BUFFER_SIZE] = "";
  	char buf[LONG_TEXT] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1, previousRating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(query, "SELECT b.title, b.year, a.first_name, a.last_name, b.isbn FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE LOWER(b.title) IN (SELECT DISTINCT b.title FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code JOIN book_genres bg ON b.isbn = bg.isbn JOIN genres g ON bg.sub_genre = g.sub_genre JOIN publishers pub ON b.pub_id = pub.pub_id WHERE ");

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(criteria, p); 
	sprintf(query + strlen(query), "LOWER(b.title) LIKE LOWER('%%%s%%') AND ", criteria);

	p = strtok(NULL, "/"); strcpy(criteria, p); 
	sprintf(query + strlen(query), "(LOWER(a.first_name) LIKE LOWER('%%%s%%') OR LOWER(a.last_name) LIKE LOWER('%%%s%%')) AND ", criteria, criteria);
	
	p = strtok(NULL, "/"); strcpy(criteria, p); 
	sprintf(query + strlen(query), "LOWER(g.genre) LIKE LOWER('%%%s%%') AND ", criteria);

	p = strtok(NULL, "/"); strcpy(criteria, p);
	sprintf(query + strlen(query), "LOWER(g.sub_genre) LIKE LOWER('%%%s%%') AND ", criteria);
	
	p = strtok(NULL, "/"); strcpy(criteria, p);
	sprintf(query + strlen(query), "b.year >= %d AND ", atoi(criteria));

	p = strtok(NULL, "/"); strcpy(criteria, p);
	sprintf(query + strlen(query), "LOWER(pub.name) LIKE LOWER('%%%s%%') AND ", criteria);

	p = strtok(NULL, "/"); strcpy(criteria, p);
	sprintf(query + strlen(query), "b.title IN (SELECT b.title FROM lists l JOIN books b ON l.isbn = b.isbn GROUP BY l.isbn HAVING AVG(l.rating) >= %d) AND ", atoi(criteria));

	p = strtok(NULL, "/"); strcpy(criteria, p);
	sprintf(query + strlen(query), "b.isbn LIKE LOWER('%%%s%%')", criteria);

	p = strtok(NULL, "/"); strcpy(username, p);

	sprintf(query + strlen(query), " ORDER BY b.title) ORDER BY b.title;"); 

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); //printf("//// %d ////\n", rowsNumber);

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(previousRating != -1)
					sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);

		    	strcpy(previous, row[0]); previousRating = getRating(conn, row[0]); updatePoints(conn, username, row[4], 3);
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating); mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result); cap(toUser);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void showHierarchy(int fd, MYSQL* conn, const char* buff) {	
	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "empty";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	strcpy(buf, "SELECT genre, sub_genre FROM genres");

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error searching for genres!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); 

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser) - 2, ", ");
				sprintf(toUser + strlen(toUser), "%s", row[1] ? row[1] : "NULL");
			} else {
		    	strcpy(previous, row[0]);
			
			    sprintf(toUser + strlen(toUser), "%d) %s \n\n///// SUBGENRES: %s", crt++, row[0], row[1]);
			}

    		sprintf(toUser + strlen(toUser), "\n\n");
		}

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void downloadTitle(int fd, MYSQL* conn, const char* buff) {	
	char toUser[BUFFER_SIZE] = "";
  	char link[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, strchr(buff, '/') + 1);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(link, "SELECT link FROM books WHERE title = '%s'", buf);

	errorCheck(mysql_query(conn, link), "\n\n[server] Error searching for download link!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); //printf("//// %d ////\n", rowsNumber);

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result); 
		strcpy(toUser, row[0]);

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void getSubGenres(MYSQL* conn, const char* buff, char* subgenres) {
	char toUser[BUFFER_SIZE] = "";
  	char query[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(query, "SELECT g.sub_genre FROM genres g JOIN book_genres bg ON g.sub_genre = bg.sub_genre JOIN books b ON bg.isbn = b.isbn WHERE title = '%s'", buf); 

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for subgenres!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); 

	if(rowsNumber >= 1) {
		for(int i = 1; i < rowsNumber; i++) {
			row = mysql_fetch_row(result); 
			sprintf(toUser + strlen(toUser), "%s, ", row[0]);
		}

		row = mysql_fetch_row(result); sprintf(toUser + strlen(toUser), "%s", row[0]);

		mysql_free_result(result);

		strcpy(subgenres, toUser);
	} else {
		mysql_free_result(result);

		strcpy(subgenres, "ERROR/");
	}
}

void getGenres(MYSQL* conn, const char* buff, char* genres) {
	char toUser[BUFFER_SIZE] = "";
  	char query[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(query, "SELECT DISTINCT g.genre FROM genres g JOIN book_genres bg ON g.sub_genre = bg.sub_genre JOIN books b ON bg.isbn = b.isbn WHERE title = '%s'", buf); 

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for genres!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result);

	if(rowsNumber >= 1) {
		for(int i = 1; i < rowsNumber; i++) {
			row = mysql_fetch_row(result); 
			sprintf(toUser + strlen(toUser), "%s, ", row[0]);
		}

		row = mysql_fetch_row(result); sprintf(toUser + strlen(toUser), "%s", row[0]);

		mysql_free_result(result);

		strcpy(genres, toUser);
	} else {
		mysql_free_result(result);

		strcpy(genres, "ERROR/");
	}
}

void getAuthors(MYSQL* conn, const char* buff, char* authors) {
	char toUser[BUFFER_SIZE] = "";
  	char query[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(query, "SELECT a.first_name, a.last_name FROM authors a JOIN publications p ON a.aut_code = p.aut_code JOIN books b ON p.isbn = b.isbn WHERE title = '%s'", buf);

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for authors!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result);

	if(rowsNumber >= 1) {
		for(int i = 1; i < rowsNumber; i++) {
			row = mysql_fetch_row(result); 
			sprintf(toUser + strlen(toUser), "%s %s & ", row[0], row[1]); 
		}

		row = mysql_fetch_row(result); sprintf(toUser + strlen(toUser), "%s %s", row[0], row[1]); 

		mysql_free_result(result);

		strcpy(authors, toUser);
	} else {
		mysql_free_result(result);

		strcpy(authors, "ERROR/");
	}
}

void getISBN(MYSQL* conn, const char* buff, char* isbn) {
	char toUser[BUFFER_SIZE] = "";
  	char query[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(query, "SELECT isbn FROM books WHERE title = '%s'", buf); 

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for isbn!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); //printf("//// %d ////\n", rowsNumber);

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result);
		strcpy(toUser, row[0]);
		mysql_free_result(result); 

		strcpy(isbn, toUser);
	} else {
		mysql_free_result(result);

		strcpy(isbn, "ERROR/");
	}
}

void showMoreServer(int fd, MYSQL* conn, const char* buff) {
	char buf[BUFFER_SIZE] = ""; strcpy(buf, strchr(buff, '/') + 1);
	char authors[BUFFER_SIZE] = "", genres[BUFFER_SIZE] = "", subgenres[BUFFER_SIZE] = "";
	char query[BUFFER_SIZE] = "", toUser[LONG_TEXT] = "", book[BUFFER_SIZE] = "", username[BUFFER_SIZE] = "";
	int rowsNumber = 0, rating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char *p = strtok(buf, "/"); strcpy(username, p);
	p = strtok(NULL, "/"); if(p) strcpy(book, p);

	sprintf(query, "SELECT b.title, b.year, p.name, b.isbn, b.description FROM books b JOIN publishers p ON b.pub_id = p.pub_id WHERE b.title = '%s' ORDER BY b.title;", book);

	errorCheck(mysql_query(conn, query), "\n\n[server] Error searching for book info!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); 

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result); 
		getAuthors(conn, book, authors); getGenres(conn, book, genres); getSubGenres(conn, book, subgenres); rating = getRating(conn, book);

		if(rating == 0)
			sprintf(toUser, "/ TITLE: %s (%s)\n\n// AUTHORS: %s\n\n/// GENRES: %s\n\n//// SUBGENRES: %s\n\n///// PUBLISHER: %s\n\n////// RATING: %s\n\n/////// ISBN: %s\n\n//////// DESCRIPTION: %s\n\n", row[0], row[1], authors, genres, subgenres, row[2], "Not Rated", row[3], row[4]);
		else
			sprintf(toUser, "/ TITLE: %s (%s)\n\n// AUTHORS: %s\n\n/// GENRES: %s\n\n//// SUBGENRES: %s\n\n///// PUBLISHER: %s\n\n////// RATING: %d\n\n/////// ISBN: %s\n\n//////// DESCRIPTION: %s\n\n", row[0], row[1], authors, genres, subgenres, row[2], rating, row[3], row[4]);

		mysql_free_result(result);

		if(strstr(toUser, "ERROR/")) {
			strcpy(toUser, "NONE"); 

			if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
		}

		updatePoints(conn, username, row[3], 4);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void addToReadList(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
	char book[BUFFER_SIZE] = "";
	char isbn[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p);
	p = strtok(NULL, "/"); strcpy(book, p); getISBN(conn, book, isbn);

	if(strstr(isbn, "ERROR/")) {
		printf("\n\n[Sorry] Couldn't add book to Read List.\n\n");

		strcpy(toUser, "NONE");

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}

	strcpy(buf, ""); sprintf(buf, "INSERT IGNORE INTO `lists` VALUES('%s', 'read', '%s', NULL)", username, isbn);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error adding book to Read List!\n");

	strcpy(toUser, "OK"); updatePoints(conn, username, isbn, 6);

	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void addToReadLaterList(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
	char book[BUFFER_SIZE] = "";
	char isbn[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[LONG_TEXT] = ""; strcpy(buf, buff);

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p);
	p = strtok(NULL, "/"); strcpy(book, p); getISBN(conn, book, isbn);

	if(strstr(isbn, "ERROR/")) {
		printf("\n\n[Sorry] Couldn't add book to ToRead List.\n\n");

		strcpy(toUser, "NONE");

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}

	//////////// CHECK TO SEE IF BOOK IS IN READ LIST, IF SO, SEND ERROR MESSAGE, IF NOT, ADD

	strcpy(buf, ""); sprintf(buf, "INSERT IGNORE INTO `lists` VALUES('%s', 'to_read', '%s', NULL)", username, isbn);
	//printf("????? %s ??????\n", buf);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error adding book to ToRead List!\n");

	//errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	strcpy(toUser, "OK"); updatePoints(conn, username, isbn, 5);

	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void viewReadList(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "/empty";
  	int previousRating = -1;
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strchr(buf, '/') + 1; strcpy(username, p);

	strcpy(buf, ""); sprintf(buf, "SELECT DISTINCT b.title, b.year, a.first_name, a.last_name FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code JOIN lists l ON b.isbn = l.isbn WHERE LOWER(b.title) IN (SELECT b.title FROM books b JOIN lists l ON b.isbn = l.isbn WHERE l.username = '%s' AND l.type = 'read' ORDER BY b.title) ORDER BY b.title;", username);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); // printf("//// %d ////\n", rowsNumber);

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(previousRating != -1) {
					if(previousRating == 0) sprintf(toUser + strlen(toUser), "\n\n///// RATING: Not Rated\n\n");
					else sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);
				}

		    	strcpy(previous, row[0]); previousRating = getUserRating(conn, username, row[0]);
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		if(previousRating != -1) {
			if(previousRating == 0) sprintf(toUser + strlen(toUser), "\n\n///// RATING: Not Rated\n\n");
			else sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);
		}

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void viewToReadList(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1, previousRating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strchr(buf, '/') + 1; strcpy(username, p);  

	strcpy(buf, ""); sprintf(buf, "SELECT b.title, b.year, a.first_name, a.last_name FROM books b JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE LOWER(b.title) IN (SELECT b.title FROM books b JOIN lists l ON b.isbn = l.isbn WHERE l.username = '%s' AND l.type = 'to_read' ORDER BY b.title) ORDER BY b.title;", username);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); //printf("//// %d ////\n", rowsNumber);

	// int num_fields = mysql_num_fields(result); 

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(previousRating != -1)
					sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);

		    	strcpy(previous, row[0]); previousRating = getRating(conn, row[0]);
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating); mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void transferBook(int fd, MYSQL* conn, const char* buff) { 
	char username[BUFFER_SIZE] = "";
  	char book[BUFFER_SIZE] = "";
  	char isbn[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p);
	p = strtok(NULL, "/"); strcpy(book, p); getISBN(conn, book, isbn);

	strcpy(buf, ""); sprintf(buf, "UPDATE IGNORE `lists` SET type = 'read' WHERE type = 'to_read' AND username = '%s' AND isbn = '%s';", username, isbn);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error removing book from read!\n");

	//errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	strcpy(toUser, "TRANSFERED");

	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void rateTitle(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char book[BUFFER_SIZE] = "";
  	char rate[BUFFER_SIZE] = "";
  	char isbn[BUFFER_SIZE] = "";
  	char toUser[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rating = 0;

	char* p = strtok(strchr(buf, '/'), "/"); strcpy(username, p);
	p = strtok(NULL, "/"); strcpy(book, p); getISBN(conn, book, isbn);
	p = strtok(NULL, "/"); strcpy(rate, p); rating = atoi(rate);

	strcpy(buf, ""); sprintf(buf, "UPDATE IGNORE `lists` SET rating = %d WHERE type = 'read' AND username = '%s' AND isbn = '%s';", rating, username, isbn); cap(buf); 

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error rating book!\n");

	//errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	strcpy(toUser, "RATED");

	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void communityTasteServer(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1, previousRating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strchr(buf, '/') + 1; strcpy(username, p); 

	strcpy(buf, ""); sprintf(buf, "SELECT b.title, b.year, a.first_name, a.last_name FROM book_points bp JOIN books b ON bp.isbn = b.isbn JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE b.title NOT IN (SELECT b.title FROM lists l JOIN books b ON l.isbn = b.isbn WHERE l.username = '%s') GROUP BY b.isbn, b.title, b.year, a.first_name, a.last_name ORDER BY SUM(bp.points) DESC;", username);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); //printf("//// %d ////\n", rowsNumber);

	// int num_fields = mysql_num_fields(result); 

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(crt == 6) break;

				if(previousRating != -1)
					sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);

		    	strcpy(previous, row[0]); previousRating = getRating(conn, row[0]); 
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating); mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void myTasteServer(int fd, MYSQL* conn, const char* buff) {
	char username[BUFFER_SIZE] = "";
  	char toUser[LONG_TEXT] = "";
  	char previous[BUFFER_SIZE] = "";
  	char buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
  	int rowsNumber = 0, crt = 1, previousRating = -1;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char* p = strchr(buf, '/') + 1; strcpy(username, p); 

	strcpy(buf, ""); sprintf(buf, "SELECT b.title, b.year, a.first_name, a.last_name FROM book_points bp JOIN books b ON bp.isbn = b.isbn JOIN publications p ON b.isbn = p.isbn JOIN authors a ON p.aut_code = a.aut_code WHERE bp.username = '%s' AND b.title NOT IN (SELECT b.title FROM lists l JOIN books b ON l.isbn = b.isbn WHERE l.username = '%s') GROUP BY b.isbn, b.title, b.year, a.first_name, a.last_name ORDER BY SUM(bp.points) DESC;", username, username);

	errorCheck(mysql_query(conn, buf), "\n\n[server] Error searching for books!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving query result!\n");

	rowsNumber = mysql_num_rows(result); //printf("//// %d ////\n", rowsNumber);

	// int num_fields = mysql_num_fields(result); 

	if(rowsNumber >= 1) {
		while ((row = mysql_fetch_row(result))) {
			if(strcmp(previous, row[0]) == 0) {
				sprintf(toUser + strlen(toUser), "& ");
				sprintf(toUser + strlen(toUser), "%s ", row[2] ? row[2] : "NULL");
				sprintf(toUser + strlen(toUser), "%s ", row[3] ? row[3] : "NULL");
			} else {
				if(crt == 6) break;

				if(previousRating != -1)
					sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating);

		    	strcpy(previous, row[0]); previousRating = getRating(conn, row[0]); 
			
			    sprintf(toUser + strlen(toUser), "%d) TITLE: %s (%s)\n\n///// AUTHORS: %s %s ", crt++, row[0], row[1], row[2], row[3]);
			}
		}

		sprintf(toUser + strlen(toUser), "\n\n///// RATING: %d\n\n", previousRating); mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	} else {
		strcpy(toUser, "NONE"); 

		mysql_free_result(result);

		if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
	}
}

void anagramGame(int fd, MYSQL* conn, const char* buff) {
	char query[BUFFER_SIZE] = "", buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
	char book[BUFFER_SIZE] = "";
	char toUser[BUFFER_SIZE] = "";
	char anagram[BUFFER_SIZE] = "";
	int number = rand() % 40; 
	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char *p = strchr(buf, '/') + 1; strcpy(book, p); sprintf(query, "SELECT title FROM books LIMIT %d, 1;", (number * atoi(book)) % 40);

	errorCheck(mysql_query(conn, query), "\n\n[server] Error getting random book!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving random book!\n");

	rowsNumber = mysql_num_rows(result); strcpy(book, "");

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result); strcpy(book, row[0]); scrambleWord(book, anagram);

		sprintf(toUser, "%s/%s/", anagram, row[0]);
	} else {
		strcpy(toUser, "NONE");
	}

	mysql_free_result(result); 
	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void emojiGame(int fd, MYSQL* conn, const char* buff) {
	char query[BUFFER_SIZE] = "", buf[BUFFER_SIZE] = ""; strcpy(buf, buff);
	char book[BUFFER_SIZE] = "";
	char toUser[BUFFER_SIZE] = "";
	char emojis[BUFFER_SIZE] = "";
	int number = rand() % 40; 
	int rowsNumber = 0;
	MYSQL_RES *result;
	MYSQL_ROW row;

	char *p = strchr(buf, '/') + 1; strcpy(book, p); sprintf(query, "SELECT title, emojis FROM books LIMIT %d, 1;", (number * atoi(book)) % 40);

	errorCheck(mysql_query(conn, query), "\n\n[server] Error getting random book!\n");

	errorDBCheck(result = mysql_store_result(conn), "\n\n[server] Error retrieving random book!\n");

	rowsNumber = mysql_num_rows(result); strcpy(book, "");

	if(rowsNumber == 1) {
		row = mysql_fetch_row(result); 
		strcpy(book, row[0]); strcpy(emojis, row[1]);

		sprintf(toUser, "%s/%s/", emojis, book);
	} else {
		strcpy(toUser, "NONE");
	}

	mysql_free_result(result); 
	if(errorCheck(write (fd, toUser, strlen1(toUser)),
    		"\n\n[server] Error writing to client!\n") == 0) return;
}

void handle(int fd, MYSQL* conn) {
	int bytes;
	char buf[BUFFER_SIZE] = "";

	while ((bytes = read (fd, buf, BUFFER_SIZE)) > 0) { 
	
		if(errorCheck(bytes, "\n\n[Server] Error in handle function!\n\n") == 0 || strlen(buf) < 3) return;

		printf ("\n\n[server] Message received #%s#\n", buf);
		      
		if(strstr(buf, "LOGIN/")) usernameExists(fd, conn, buf);
		else if(strstr(buf, "PASSWORD/")) checkPassword(fd, conn, buf);
		else if(strstr(buf, "SIGNUP/")) 
				if(goodFormat(strchr(buf, '/') + 1)) usernameExists(fd, conn, buf);
				else wrongFormat(fd);
		else if(strstr(buf, "NEW/")) newPassword(fd, conn, buf);
		else if(strstr(buf, "SEARCH/")) searchDefault(fd, conn, buf);
		else if(strstr(buf, "ONE/")) searchOneFilter(fd, conn, buf);
		else if(strstr(buf, "MULTIPLE/")) searchMultipleFilter(fd, conn, buf);
		else if(strstr(buf, "GENRE/")) showHierarchy(fd, conn, buf);
		else if(strstr(buf, "DOWNLOAD/")) downloadTitle(fd, conn, buf);
		else if(strstr(buf, "MORE/")) showMoreServer(fd, conn, buf);
		else if(strstr(buf, "ALREADY/")) addToReadList(fd, conn, buf);
		else if(strstr(buf, "SEEREAD/")) viewReadList(fd, conn, buf);
		else if(strstr(buf, "LATER/")) addToReadLaterList(fd, conn, buf);
		else if(strstr(buf, "SEENEXT/")) viewToReadList(fd, conn, buf);
		else if(strstr(buf, "TRANSFER/")) transferBook(fd, conn, buf);
		else if(strstr(buf, "RATEBOOK/")) rateTitle(fd, conn, buf);
		else if(strstr(buf, "STATUS/")) alreadyRead(fd, conn, buf);
		else if(strstr(buf, "MYTASTE/")) myTasteServer(fd, conn, buf);
		else if(strstr(buf, "COMMUNITYTASTE/")) communityTasteServer(fd, conn, buf);
		else if(strstr(buf, "ANAGRAM/")) anagramGame(fd, conn, buf);
		else if(strstr(buf, "EMOJIS/")) emojiGame(fd, conn, buf);
		else return;
	}
}