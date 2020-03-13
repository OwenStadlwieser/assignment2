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

int main(int argc, char *argv[]) {
    WDigraph graph;
    unordered_map<int, Point> points;
    unordered_map<int, PIL> searchTree;
    if(argc == 2)
    {
        WDigraph* g = read_city_graph_undirected(argv[1],graph, points);
    }
    while(true)
    {

        char first;
        cin.clear();
        cin >> first;
        if(first == 'R')
        {
            long long lat1, lon1, lat2, lon2;
            cin >> lat1 >> lon1 >> lat2 >> lon2;
            int vertex1 = getClosestVertex(lat1, lon1, points);
            int vertex2 = getClosestVertex(lat2, lon2, points);
            dijkstra(graph, vertex1, searchTree);
                // read off a path
            list<int> path;
            if (searchTree.find(vertex2) == searchTree.end()) {
              cout << "Vertex " << vertex2 << " not reachable from " << vertex1 << endl;
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
                cout << "N " << count+1 << endl;
                char arduino;
                for (auto it : path) {
                    cin >> arduino;
                    if(arduino == 'A'){
                        cout << "W: " << points[it].lat << " " << points[it].lon << endl;;
                    }
                }
                cout << "E" << endl;
            }
        }
    }
}