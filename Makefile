CXX = g++
CXXFLAGS = -Wall
main: main.o digraph.o wdigraph.o dijkstra.o
	$(CXX) main.o digraph.o wdigraph.o  dijkstra.o -o main
digraph.o: digraph.cpp digraph.h
	$(CXX) digraph.cpp -c 
wdigraph.o: wdigraph.cpp wdigraph.h
	$(CXX) wdigraph.cpp -c
dijkstra.o: dijkstra.cpp dijkstra.h
	$(CXX) dijkstra.cpp -c
main.o: main.cpp 
	$(CXX) main.cpp -c 






clean:
	rm -rf main main.o digraph digraph.o  a.out wdigraph.o dijkstra.o