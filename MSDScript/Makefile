CXX=c++
CFLAGS=-std=c++11
#CFLAGS=-Wall -O2 --std=c++14

#H=Expr.hpp catch.h parse.hpp cmdline.hpp val.h
CXXSOURCE=Expr.cpp main.cpp Test.cpp subTest.cpp parse.cpp cmdline.cpp parse_tests.cpp
OBJS=main.o cmdline.o Test.o subTest.o parse_tests.o parse.o Expr.o val.o Env.o
header = headers



# "make (all)" will build both executables
#all: msdscript test_msdscript


makemake serial_testmake serial_testtestmake clen\anmake make reserial_testmake conccurrent_test

# 1st executable
# ----------------------------------------------------------------------------------------------------------------------
msdscript: $(OBJS)
	$(CXX) $(CFLAGS) -o msdscript $(OBJS) -fsanitize=undefined -fno-sanitize-recover=undefined

main.o: main.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) main.cpp

cmdline.o: cmdline.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) cmdline.cpp

Expr.o: Expr.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) Expr.cpp

Test.o: Test.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) Test.cpp

subTest.o: subTest.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) subTest.cpp

parse_tests.o: parse_tests.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) parse_tests.cpp

parse.o: parse.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) parse.cpp

val.o: val.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) val.cpp

Env.o: Env.cpp $(header)
	$(CXX) $(CFLAGS) -c -I$(header) Env.cpp


# ----------------------------------------------------------------------------------------------------------------------
.PHONY: clean
clean:
	rm -f *.o *.out *.gch msdscript test_msdscript

.PHONY: interp
interp: msdscript
	./msdscript --interp

.PHONY: print
print: msdscript
	./msdscript --print

.PHONY: test
test: msdscript
	./msdscript --test

.PHONY: prettyprint
prettyprint: msdscript
	./msdscript --pretty_print







# 2nd executable
# ----------------------------------------------------------------------------------------------------------------------
test_msdscript: exec.o test_msdscript.o
	$(CXX) $(CFLAGS) -o test_msdscript exec.o test_msdscript.o

exec.o: exec.cpp $(header)/exec.h
	$(CXX) $(CFLAGS) -c -I$(header) exec.cpp

test_msdscript.o: test_msdscript.cpp headers/test_msdscript.h
	$(CXX) $(CFLAGS) -c -I$(header) test_msdscript.cpp

.PHONY: cleantest
cleantest:
	rm -f *.o *.out *.gch test_msdscript

runtest: test_msdscript
	./test_msdscript


# ----------------------------------------------------------------------------------------------------------------------
.PHONY: doc
doc:
	cd documentation && doxygen

.PHONY: showdoc
showdoc:
	cd documentation/html && open index.html