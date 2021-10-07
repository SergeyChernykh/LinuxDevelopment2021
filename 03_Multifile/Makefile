.RECIPEPREFIX = ~

CFLAGS=-fPIC
LD_LIBRARY_PATH = LD_LIBRARY_PATH=`pwd`

TEST_FILE = log
GENERATES = prog prog-a prog-so README *.$(TEST_FILE) *.so
TRASH = *.o *~ o.* *.a
OUTPUT = 1>$@ 2>/dev/null
ARG0 = 
ARG1 = arg1
ARG3 = arg1 arg2 arg3

all:    README prog prog-a prog-so test

prog:   const.o fun.o prog.o

prog-a: prog.o liboutput_static.a
~		cc -L. $< -loutput_static -o $@

prog-so: prog.o liboutput.so
~		cc -L. $< -loutput -o $@

README: prog
~		./$< 2> $@

fun.o:  outlib.h
prog.o: outlib.h

liboutput_static.a: const.o fun.o
~		ar -rcs $@ $^

liboutput.so: const.o fun.o
~		cc -shared $^ -o $@

test: test_0 test_1 test_3
test_0: test_0_0 test_0_1 test_0_3
test_1: test_1_0 test_1_1 test_1_3
test_3: test_3_0 test_3_1 test_3_3

%_0.$(TEST_FILE): %
~		$(LD_LIBRARY_PATH) ./$< $(ARG0) $(OUTPUT)

%_1.$(TEST_FILE): %
~		$(LD_LIBRARY_PATH) ./$< $(ARG1) $(OUTPUT)

%_3.$(TEST_FILE): %
~		$(LD_LIBRARY_PATH) ./$< $(ARG3) $(OUTPUT)

test_0_0: prog_0.$(TEST_FILE) prog-a_0.$(TEST_FILE)
~		cmp $^
test_0_1: prog_0.$(TEST_FILE) prog-so_0.$(TEST_FILE)
~		cmp $^
test_0_3: prog-a_0.$(TEST_FILE) prog-so_0.$(TEST_FILE)
~		cmp $^

test_1_0: prog_1.$(TEST_FILE) prog-a_1.$(TEST_FILE)
~		cmp $^
test_1_1: prog_1.$(TEST_FILE) prog-so_1.$(TEST_FILE)
~		cmp $^
test_1_3: prog-a_1.$(TEST_FILE) prog-so_1.$(TEST_FILE)
~		cmp $^

test_3_0: prog_3.$(TEST_FILE) prog-a_3.$(TEST_FILE)
~		cmp $^
test_3_1: prog_3.$(TEST_FILE) prog-so_3.$(TEST_FILE)
~		cmp $^
test_3_3: prog-a_3.$(TEST_FILE) prog-so_3.$(TEST_FILE)
~		cmp $^


clean:
~		rm -f $(TRASH)

distclean:      clean
~		rm -rf $(GENERATES)