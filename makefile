all:server client
LIBS = -lpthread
LIBSL = -ldl
server:server.c sqlite3.c 
	gcc  -w $^ -o $@ $(LIBS) $(LIBSL)
client:client.c sqlite3.c 
	gcc  -w $^ -o $@ $(LIBS) $(LIBSL)


clean:
	rm -f server
	rm -f client
