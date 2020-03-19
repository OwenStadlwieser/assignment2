CXX = g++
CXXFLAGS = -Wall
server: server.o digraph.o wdigraph.o dijkstra.o
	$(CXX) server.o digraph.o wdigraph.o  dijkstra.o -o server
digraph.o: digraph.cpp digraph.h
	$(CXX) digraph.cpp -c 
wdigraph.o: wdigraph.cpp wdigraph.h
	$(CXX) wdigraph.cpp -c
dijkstra.o: dijkstra.cpp dijkstra.h
	$(CXX) dijkstra.cpp -c
server.o: server.cpp 
	$(CXX) server.cpp -c 


clean:
	rm -rf server server.o digraph digraph.o  a.out wdigraph.o dijkstra.o
