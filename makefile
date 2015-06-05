OBJS=main.o session.o PI.o util.o parseconf.o
ARG=-Wall -g
miniftp:$(OBJS)
	gcc $(ARG) -o miniftp $(OBJS) -lcrypt
main.o:session.h parseconf.h
PI.o:util.o
	gcc $(ARG) -c PI.c util.h PI.h 
util.o:
	gcc $(ARG) -c util.c
session.o:PI.o
	gcc $(ARG) -c session.c PI.h
.PHONY:clean
clean:
	rm miniftp $(OBJS)

