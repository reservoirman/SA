# A simple Makefile

# List the object files in one place
OBJ=namechecking.o objects.o linkedlist.o

# The first target is the default if you just say "make".  In this
# case, "build" relies on "sample", because I want the executable to be
# called "sample"
all: clean namechecking objects linkedlist objput objget objlist initializer

namechecking:
	gcc -c namechecking.c

objects:
	gcc -c objects.c

linkedlist:
	gcc -c linkedlist.c

objput: objput.c
	gcc -o objput $(OBJ) objput.c

objget: objget.c
	gcc -o objget $(OBJ) objget.c
	
objlist: objlist.c
	gcc -o objlist $(OBJ) objlist.c

initializer: initializer.c
	gcc -o initializer $(OBJ) initializer.c

build:	sample

# "sample" requires a set of object files
# $@ is a special variable: the target of the operation, in this case sample
# $? is the
sample: $(OBJ)
	cc -o $@ $(OBJ)

test_objput: test_objput.c
	gcc -o test_objput $(OBJ) test_objput.c

# Before testing, we must compile.  
# Lines preceeded by @ aren't echoed before executing
# Execution will stop if a program has a non-zero return code;
# precede the line with a - to override that
test:	clean testclean namechecking objects linkedlist objput test_objput  
	@echo "Testing objput:"
	./test_objput 
	@echo "------------"
	

exec: all
	./initializer $(userfile)

clean:
	rm -f objput objlist objget initializer *.core *.o

testclean:
	rm -f test_objput
