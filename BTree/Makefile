CXX = g++
CXXFLAGS = -std=c++11

test.out : test.o
	$(CXX) $(CXXFLAGS) -o test.out test.o

test.o : tree.h node.h
	$(CXX) $(CXXFLAGS) -c test.cpp

a.out : main.o
	$(CXX) $(CXXFLAGS) -o a.out main.o

main.o : tree.h
	$(CXX) $(CXXFLAGS) -c main.cpp 

re :
	rm main.o test.o && make
