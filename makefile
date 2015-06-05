OBJS=main.o session.o PI.o util.o parseconf.o
ARG=-Wall
miniftp:$(OBJS)
	gcc $(ARG) -o miniftp $(OBJS) -lcrypt
main.o:session.h parseconf.h
PI.o:util.o
	gcc $(ARG) -c PI.c util.h 
util.o:
	gcc $(ARG) -c util.c
.PHONY:clean
clean:
	rm miniftp $(OBJS)

