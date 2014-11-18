# A simple Makefile

# List the object files in one place
OBJ=objectcrypto.o keyset.o crypto.o objectprograms.o messaging.o aclchecking.o namechecking.o objects.o linkedlist.o

PRG=objlist objget objput objsetacl objtestacl objgetacl

# The first target is the default if you just say "make".  In this
# case, "build" relies on "sample", because I want the executable to be
# called "sample"
all: clean objectcrypto keyset crypto messaging aclchecking namechecking objects linkedlist objectprograms angel objtestacl objgetacl objsetacl objput objget objlist initializer

angel:
	gcc -o angel.out $(OBJ) angel.c -lssl -lcrypto

objectcrypto:
	gcc -c objectcrypto.c 

keyset:
	gcc -c keyset.c

crypto:
	gcc -c crypto.c

objectprograms:
	gcc -c objectprograms.c

messaging:
	gcc -c messaging.c

aclchecking:
	gcc -c aclchecking.c

namechecking:
	gcc -c namechecking.c

objects:
	gcc -c objects.c

linkedlist:
	gcc -c linkedlist.c

objtestacl: objtestacl.c
	gcc -o objtestacl $(OBJ) objtestacl.c -lssl -lcrypto

objgetacl: objgetacl.c
	gcc -o objgetacl $(OBJ) objgetacl.c -lssl -lcrypto

objsetacl: objsetacl.c
	gcc -o objsetacl $(OBJ) objsetacl.c -lssl -lcrypto

objput: objput.c
	gcc -lssl -lcrypto -o objput $(OBJ) objput.c -lssl -lcrypto

objget: objget.c
	gcc -lssl -lcrypto -o objget $(OBJ) objget.c -lssl -lcrypto
	
objlist: objlist.c
	gcc -o objlist $(OBJ) objlist.c -lssl -lcrypto

initializer: initializer.c
	gcc -o initializer $(OBJ) initializer.c -lssl -lcrypto

build:	all



test_objput: test_objput.c
	gcc -o test_objput $(OBJ) test_objput.c -lssl -lcrypto

# Before testing, we must compile.  
# Lines preceeded by @ aren't echoed before executing
# Execution will stop if a program has a non-zero return code;
# precede the line with a - to override that
test:	all  
	bash run1.sh

	@echo "------------"
	

exec: all
	./initializer $(userfile)

clean:
	rm -f objsetacl objgetacl objtestacl objput objlist objget initializer *.core *.o

testclean:
	rm -f test_objput
