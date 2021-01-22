all:
	gcc -I/usr/local/mysql/include -L/usr/local/mysql/lib -Wall c.c -lmysqlclient -o c
	gcc -I/usr/local/mysql/include -L/usr/local/mysql/lib -Wall s.c -lmysqlclient -o s

clean: 
	rm -f c
	rm -f s