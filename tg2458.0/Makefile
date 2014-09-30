# A simple Makefile

# List the object files in one place
OBJ=aclchecking.o namechecking.o objects.o linkedlist.o

# The first target is the default if you just say "make".  In this
# case, "build" relies on "sample", because I want the executable to be
# called "sample"
all: clean aclchecking namechecking objects linkedlist objtestacl objgetacl objsetacl objput objget objlist initializer

aclchecking:
	gcc -c aclchecking.c

namechecking:
	gcc -c namechecking.c

objects:
	gcc -c objects.c

linkedlist:
	gcc -c linkedlist.c

objtestacl: objtestacl.c
	gcc -o objtestacl $(OBJ) objtestacl.c 

objgetacl: objgetacl.c
	gcc -o objgetacl $(OBJ) objgetacl.c 

objsetacl: objsetacl.c
	gcc -o objsetacl $(OBJ) objsetacl.c 

objput: objput.c
	gcc -o objput $(OBJ) objput.c

objget: objget.c
	gcc -o objget $(OBJ) objget.c
	
objlist: objlist.c
	gcc -o objlist $(OBJ) objlist.c

initializer: initializer.c
	gcc -o initializer $(OBJ) initializer.c

build:	all



test_objput: test_objput.c
	gcc -o test_objput $(OBJ) test_objput.c

# Before testing, we must compile.  
# Lines preceeded by @ aren't echoed before executing
# Execution will stop if a program has a non-zero return code;
# precede the line with a - to override that
test:	all  
	@echo "Testing objput:"
	./objput -u lk holla < README
	./objget -u lk holla
	./objlist -u lk
	./objtestacl -u lk -a w holla
	./objtestacl -u lk -a v holla
	./objgetacl -u lk holla
	./objsetacl -u lk holla < ALLACCESS

	@echo "------------"
	

exec: all
	./initializer $(userfile)

clean:
	rm -f objsetacl objgetacl objtestacl objput objlist objget initializer *.core *.o

testclean:
	rm -f test_objput
