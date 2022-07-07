STUDENT_ID := $(shell cat STUDENT_ID)
SUBMIT_DIR := .submit
SUBMIT_FILES:= *.c *.h Makefile readme EthicsOath.pdf
SUBMIT := $(STUDENT_ID)_assign5.tar.gz

TARGET = ish

# FIX THIS FILE
#CC = gcc209
#CFLAGS = -g

ish: ish.o dynarray.o lexsyn.o util.o token.o
	$(CC) $(CFLAGS) ish.o dynarray.o lexsyn.o util.o token.o -o ish
#ish:
#       $(CC) $(CCFLAGS) -o ish.c dynarray.h lexsyn.h util.h token.h

ish.o: ish.c dynarray.h lexsyn.h util.h token.h
	$(CC) $(CCFLAGS) -c $<

dfa.o : dfa.c dynarray.h
	$(CC) $(CCFLAGS) -c $<

dynarray.o : dynarray.c dynarray.h
	$(CC) $(CCFLAGS) -c $<

util.o : util.c token.h dynarray.h
	$(CC) $(CCFLAGS) -c $<

lexsyn.o : lexsyn.c token.h util.h
	$(CC) $(CCFLAGS) -c $<

submit:
	mkdir -p $(SUBMIT_DIR)
	cp $(SUBMIT_FILES) $(SUBMIT_DIR)
	cd $(SUBMIT_DIR) && tar -czf ../$(SUBMIT) *
	rm -rf $(SUBMIT_DIR)

clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean submit

all: $(TARGET)


