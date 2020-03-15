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
long long manhattan(const Point& pt1, const Point& pt2)
{
    return abs(pt1.lon - pt2.lon) + abs(pt1.lat - pt2.lat);
}

long long manhattan(const Point& pt1, int lon, int lat)
{
    return abs(pt1.lon - lon) + abs(pt1.lat - lat);
}

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
    		if(str[0] == 'V')
    		{
    			size_t found = str.find(",");
    			size_t found1 = str.find(",", found+1);
                size_t found2 = str.find(",", found1+1);
    			string index1s = str.substr(found+1,(found1-1)-(found));
                string lats = str.substr(found1+1, (found2-1)- found1);
                string lons = str.substr(found2+1, (str.length()-1)-found2);
                lats.erase(lats.begin()+2);
                lons.erase(lons.begin()+4);
    			int index = stoi(index1s, &sz);
                long long lat = stoi(lats);
                long long lon = stoi(lons);
                lat = lat/10;
                lon = lon /10;
    			graph.addVertex(index);
                point.lon = lon;
                point.lat = lat;
                points[index] = point;
                numPoints++;
    		}
    		else if(str[0] == 'E')
    		{
    			size_t found = str.find(",");
    			size_t found1 = str.find(",", found+1);
    			size_t found2 = str.find(",", found1+1);
    			string index1s = str.substr(found+1, (found1-1)-(found));
    			string index2s = str.substr(found1+1,(found2-1)-(found1));
    			int index1 = stoi(index1s, &sz);
    			int index2 = stoi(index2s, &sz);
    			graph.addEdge(index1, index2, manhattan(points[index1], points[index2]));
    		}

    	}
    	file.close();

    }
    return &graph;
}
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

void inputToOutput(string ifilename, string ofilename,  WDigraph& graph, unordered_map<int, Point>&points){
    unordered_map<int, PIL> searchTree;
    list<int> path;
    ifstream ifile(ifilename);
    ofstream ofile(ofilename);
    string str;
    if(ifile.is_open() && ofile.is_open()){
    	string::size_type sz;
        while(getline(ifile, str)){
            if (str[0] == 'R'){
                size_t space1 = str.find(" ");
                size_t space2 = str.find(" ", space1+1);
                size_t space3 = str.find(" ", space2+1);
                size_t space4 = str.find(" ", space3+1);
                string lats1 = str.substr(space1+1, (space2-1)-(space1));
                string lons1 = str.substr(space2+1, (space3-1)-(space2));
                string lats2 = str.substr(space3+1, (space4-1)-(space3));
                string lons2 = str.substr(space4+1, (str.length()-1)-(space4));
                long long lat1 = stoi(lats1);
                long long lon1 = stoi(lons1);                
                long long lat2 = stoi(lats2);
                long long lon2 = stoi(lons2);
                int vertex1 = getClosestVertex(lat1, lon1, points);
                int vertex2 = getClosestVertex(lat2, lon2, points);
                dijkstra(graph, vertex1, searchTree);
                    // read off a path
                if (searchTree.find(vertex2) == searchTree.end()) {
                  ofile << "N 0" << endl;
                }
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
        inputToOutput(argv[1], argv[2], graph, points);
		return 0;
    }
}