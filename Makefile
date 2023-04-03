CFLAGS  += -g -O2 -Wall -pedantic
CFLAGS += $(shell pkg-config --cflags json-c) -lcurl
LDFLAGS += $(shell pkg-config --libs json-c) -lcurl

program: clean main.o repo.o install.o
	@$(CC) -o program main.o repo.o install.o $(LDFLAGS)

clean:
	@rm -f *.o program

main.o:
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/main.c

repo.o:
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/repo.c

install.o:
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/install.c
