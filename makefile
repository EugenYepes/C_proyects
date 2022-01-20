SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS= $(patsubst %.c, %.o, $(SOURCES))
EJEC = ejec

CC			= gcc
CFLAGS		= -c -Wall -g
LDFLAGS		= -Wall
EXTRA_CFLAGS= 



# <Contiene la dependencia del target
# @Contiene el target

ejec: $(EJEC) 

$(EJEC): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	rm -f $(OBJS)

%.o:%.c Makefile
	$(CC) $(CFLAGS) (EXTRA_CFLAGS) $< -o $@
	rm -f $(OBJS)

clean:
	rm -f $(OBJS) $(EJEC)

new:clean ejec		
