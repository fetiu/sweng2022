# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive -lncursesw
LDFLAGS=
MAIN_OBJS=main.o Matrix.o Window.o MsgPubSub.o View.o Model.o TimeCtrl.o KbdCtrl.o ttymodes.o Tetris.o CTetris.o

all:: main.exe

main.exe: $(MAIN_OBJS)
	$(CC) -pthread -o $@ $^ $(CFLAGS) $(LDFLAGS_TET)

%.o: %.c $(DEPS)
	$(CC) -c -pthread -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -pthread -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump
