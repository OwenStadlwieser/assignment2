#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include "dijkstra.h"
#include "wdigraph.h"
#include "digraph.h"
using namespace std;

typedef pair<int, long long> PIL;
struct Point{
    long long lon;
    long long lat;
};

int numPoints = 0;

/*
	Description: Computes the Manhattan distance between two vertices of graph. 
	Arguments:  const Point& pt1, const Point& pt2
	Returns: long long
*/
long long manhattan(const Point& pt1, const Point& pt2)
{
    return abs(pt1.lon - pt2.lon) + abs(pt1.lat - pt2.lat);
}

/*
    Description: Computes the Manhattan distance between two vertices of graph. 
    Arguments:  const Point& pt1, const Point& pt2
    Returns: long long
*/
long long manhattan(const Point& pt1, int lon, int lat)
{
    return abs(pt1.lon - lon) + abs(pt1.lat - lat);
}

/*
	Description: Parses the text file describing the road network then builds the weighted directed
				graph and stores the coordinates of every vertex.
	Arguments: string filename, WDigraph& graph, unordered_map<int, Point>&points
	Returns: WDigraph* 
*/
WDigraph* read_city_graph_undirected(string filename, WDigraph& graph, unordered_map<int, Point>&points) 
{
    ifstream file(filename);
    string str;
    vector<int> pos;
    Point point;
    if(file.is_open())
    {
    	string::size_type sz;
    	while(getline(file, str))
    	{
    		// if line of file is vertex
    		if(str[0] == 'V')
    		{
    			size_t found = str.find(",");
    			size_t found1 = str.find(",", found+1);
                size_t found2 = str.find(",", found1+1);
                // vertex identifier
    			string index1s = str.substr(found+1,(found1-1)-(found));
    			// vertex latitude
                string lats = str.substr(found1+1, (found2-1)- found1);
                // vertex longitude
                string lons = str.substr(found2+1, (str.length()-1)-found2);
                lats.erase(lats.begin()+2);
                lons.erase(lons.begin()+4);
                // converting strings found in file to ints
    			int index = stoi(index1s, &sz);
                long long lat = stoi(lats);
                long long lon = stoi(lons);
                lat = lat/10;
                lon = lon /10;
                // building graph
    			graph.addVertex(index);
                point.lon = lon;
                point.lat = lat;
                points[index] = point;
                numPoints++;
    		}
    		// if line of file is edge
    		else if(str[0] == 'E')
    		{
    			size_t found = str.find(",");
    			size_t found1 = str.find(",", found+1);
    			size_t found2 = str.find(",", found1+1);
    			// vertex 1 (start)
    			string index1s = str.substr(found+1, (found1-1)-(found));
    			// vertex 2 (end)
    			string index2s = str.substr(found1+1,(found2-1)-(found1));
    			int index1 = stoi(index1s, &sz);
    			int index2 = stoi(index2s, &sz);
    			// 
    			graph.addEdge(index1, index2, manhattan(points[index1], points[index2]));
    		}

    	}
    	file.close();

    }
    return &graph;
}

/*
	Description: Finds the closest vertex to the one requested.
	Arguments: long long lat, long long lon, unordered_map<int, Point>& points
	Returns: int
*/
int getClosestVertex(long long lat, long long lon, unordered_map<int, Point>&points)
{
    long long minDist = 0;
    int minIndex;
    int i = 0;
    for(auto& e : points)
    {
        long long dist = manhattan(e.second, lon, lat);
        if(dist < minDist || (i == 0))
        {
            minDist = dist;
            minIndex = e.first;
        }
        i++;
    }
    return minIndex;
}

/*
	Description: Takes the request from one file, computes the route and outputs it to another file.
	Arguments: string ifilename, string ofilename,  WDigraph& graph, unordered_map<int, Point>&points
	Returns: void
*/
void inputToOutput(string ifilename, string ofilename,  WDigraph& graph, unordered_map<int, Point>&points){
    unordered_map<int, PIL> searchTree;
    list<int> path;
    ifstream ifile(ifilename);
    ofstream ofile(ofilename);
    string str;
    if(ifile.is_open() && ofile.is_open()){
        while(getline(ifile, str)){
            if (str[0] == 'R'){
                size_t space1 = str.find(" ");
                size_t space2 = str.find(" ", space1+1);
                size_t space3 = str.find(" ", space2+1);
                size_t space4 = str.find(" ", space3+1);
                // starting latitude
                string lats1 = str.substr(space1+1, (space2-1)-(space1));
                // starting longitude
                string lons1 = str.substr(space2+1, (space3-1)-(space2));
                // ending latitude
                string lats2 = str.substr(space3+1, (space4-1)-(space3));
                // ending longitude
                string lons2 = str.substr(space4+1, (str.length()-1)-(space4));
                long long lat1 = stoi(lats1);
                long long lon1 = stoi(lons1);                
                long long lat2 = stoi(lats2);
                long long lon2 = stoi(lons2);
                int vertex1 = getClosestVertex(lat1, lon1, points);
                int vertex2 = getClosestVertex(lat2, lon2, points);
                // computes least cost path between two vertices
                dijkstra(graph, vertex1, searchTree);
                //no path found
                if (searchTree.find(vertex2) == searchTree.end()) {
                  ofile << "N 0" << endl;
                }
                // number of vertices in path
                else {
                    int stepping = vertex2;
                    int count = 0;
                    while (stepping != vertex1) {
                        path.push_front(stepping);
                        count++;
                        // crawl up the search tree one step
                        stepping = searchTree[stepping].first;
                    }
                    path.push_front(vertex1);
                    ofile << "N " << count+1 << endl;
                }
		        list<int>::iterator it = path.begin();
	            while (getline(ifile, str)){
		            if(str[0] == 'A'){            
		                // waypoints of path
		                if(it != path.end()){
		                    ofile << "W " << points[*it].lat << " " << points[*it].lon << endl;
		                    it++;
		                }
		                else{
		                    ofile << "E" << endl;
		                    break;
		                }
		            }   
		            else{
		            	break;
		            }
	            }
            }
        }
        ifile.close();
        ofile.close();
    }
}

int main(int argc, char *argv[]) {
    WDigraph graph;
    unordered_map<int, Point> points;
    WDigraph* g = read_city_graph_undirected("edmonton-roads-2.0.1.txt", graph, points);
    if(argc == 3)
    {	
    	// argv[1] is input file, argv[2] is output file
        inputToOutput(argv[1], argv[2], graph, points);
		return 0;
    }
}