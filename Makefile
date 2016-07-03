CXX=g++
INCLUDE=-I./include
CXXFLAGS=-g3 -march=native -O4 -std=c++11 -fomit-frame-pointer -funroll-loops -fforce-addr -fexpensive-optimizations -msse2  -Wall -fPIC $(INCLUDE)
SRC=src/binom.c  src/main.c src/cmp.c  src/dmvhyper.c  src/dmvhyperLog.c  src/pmvhyper.c src/CoDO.c
OBJ=$(SRC:.c=.o)
PROGRAM=pmvhyper

	
all : $(PROGRAM) message


src/%.o: src/%.c
	$(CXX) $(CXXFLAGS) -c -o $@  $<


$(PROGRAM): $(OBJ) 
	$(CXX) $(CXXFLAGS)  -o $@ $(OBJ)	


matlab: $(PROGRAM) $(MEX)	
	mex matlab/mex_dmvhyper.cpp $(OBJ) $(INCLUDE)
	mex matlab/mex_pmvhyper.cpp $(OBJ) $(INCLUDE)


clean:
	rm -f *.mex*  src/*.o *~


message:
	echo "Executable: $(PROGRAM) has been created"	
