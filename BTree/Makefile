CXX = g++
CXXFLAGS = -std=c++11

test.out : test.o
	$(CXX) $(CXXFLAGS) -o test.out test.o

a.out : main.o
	$(CXX) $(CXXFLAGS) -o a.out main.o

test.o : tree.h node.h
	$(CXX) $(CXXFLAGS) -c test.cpp

main.o : tree.h
	$(CXX) $(CXXFLAGS) -c main.cpp 

re :
	rm main.o test.o && make

run :
	rm test.o test.out && make && ./test.out
