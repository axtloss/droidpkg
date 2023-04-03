CFLAGS  += -g -O2 -Wall -pedantic
CFLAGS += $(shell pkg-config --cflags json-c)
LDFLAGS += $(shell pkg-config --libs json-c)

program: clean main.o repo.o
	@$(CC) -o program main.o repo.o $(LDFLAGS)

clean:
	@rm -f *.o program

main.o:
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/main.c

repo.o:
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/repo.c
