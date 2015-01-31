// a_star.cpp
// libraries
#include <queue>
#include <string>
#include <math.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
using namespace std;

//count number of nodes generated in A* search
int numNodes = 0;

//define node structure
struct node {
	string n; // name of node
	int x; // x-coordinate of node
	int y; // y-coordinate of node
	int f; // f-value
	int g; // g-value
	int d; // depth of node in tree
	node *Parent;
	vector<node> Children;
};

//determine priority in priority queue
class ComparePriority {
public:
	bool operator()(node & n1, node & n2) {
		if (n1.f == n2.f && n1.n == n2.n) { // for nodes with same name and priority
			return n1.d < n2.d; // place node further down the tree first in priority queu
		}
		return n1.f > n2.f; // sort prioirty queue based on lower f-value
	}
};

// A_star algorithm
string Astar(const string & Name, const int & xGoal, const int & yGoal, int numCities, node *allCities) {

	// declaring variables
	static priority_queue<node, vector<node>, ComparePriority> pq; // list of nodes added to search tree that haven't been expanded yet
	static node n0; // parent node
	static node n1; // node to help generate children
	static node n2; // node to help determine which children enter the queue
	static node n3; // node to help determine which children enter the queue
	static int x; // x-coordinate placeholder
	static int y; // y-coordinate placeholder

	// create start node A and push it into priority queue
	n0.n = Name;
	n0.x = xGoal;
	n0.y = yGoal;
	n0.f = 0;
	n0.g = 0;
	n0.d = 0;
	n0.Parent = NULL;
	numNodes++;
	pq.push(n0);

	//A* search
	while (!pq.empty()) {
	
		n0 = pq.top();
		x = n0.x;
		y = n0.y;
		pq.pop();

		//reached goal state
		if (numCities == 1) {return "A";} // base case: one city
		if (x == xGoal && y == yGoal && n0.d == numCities) { 
			string path = "A"; // print initial city the tsp started at
			for (int i = 0; i < numCities; i++) { // create string path representing the optimal path of the tsp
				if (n0.Parent == NULL) {break;}	
				else {path = path + n0.Parent->n;}
				n0 = *(n0.Parent);
			}
			while (!pq.empty()) { // empty priority queue
				pq.pop();
			}
			return path;
		}

		//generate child nodes
		for (int i = 0; i < numCities; i++) {
			x = allCities[i].x;
			y = allCities[i].y;
			n1 = allCities[i];
			n1.Parent = new node(n0);
			n1.d = n0.d + 1; // increase depth
			n1.g = n0.g + sqrt(abs(n1.x-n0.x)*abs(n1.x-n0.x)+abs(n1.y-n0.y)*abs(n1.y-n0.y)); // actual cost of tsp path so far
			n1.f = n1.g + sqrt(abs(n1.x-xGoal)*abs(n1.x-xGoal)+abs(n1.y-yGoal)*abs(n1.y-yGoal)); // g + heuristic function(euclidean distance)
			n0.Children.push_back(n1); // add children to parent node
		}
		
		//select child nodes to be added to priority queue
		for (int i = 0; i < numCities; i++) {
			n2 = n0.Children.at(i); // node that may be added to queue 
			n3 = n2; // node that backtracks up the path and is used to check if the child node(n2) is already in current path
			if (n2.n == "A" && n2.d != numCities) {} // do not add start node into queue until tsp has traversed all other cities
			else if (n2.n == "A" && n2.d == numCities) { // add start node into queue once tsp has traversed all other cities
				numNodes++;
				pq.push(n2);
			}
			else { // avoid adding node into queue that is already in the current path
				for (int i = 0; i <= n2.d; i++) {
					if (n3.n == n2.n && i != 0) {break;}
					if (n3.Parent == NULL || n2.Parent == NULL) {
						numNodes++;
						pq.push(n2);
						break;
					}
					else {
						n3 = *(n3.Parent);
					}
				}
			}
		}			
	}
}	

int main(){
	
	// read in nodes
	static int cities;
	node nTemp;
	scanf("%d", &cities); // read in # of cities
	node allCities[cities]; // array of nodes to be evaluated
	for (int i = 0; i < cities; i++) { // read in name, x-coordinate, and y-coordinate of node;
		char tmp[3];
		string Name;
		int x;
		int y;
		scanf("%2s %d %d", tmp, &x, &y);
		Name = tmp;
		nTemp.n = Name;
		nTemp.x = x;
		nTemp.y = y;
		nTemp.f = 0;
		nTemp.g = 0;
		nTemp.d = 0;
		nTemp.Parent[1];
		allCities[i] = nTemp; // add node to array
	}

	// time how long it takes to find a solution
	srand(time(NULL));
	clock_t start = clock();
	string path = Astar(allCities[0].n, allCities[0].x, allCities[0].y, cities, allCities);
	clock_t end = clock();	
	double time_elapsed = double(end - start);
	cout << "Time to calculate the path (ms): " << time_elapsed << endl;
	cout << "Number of Cities: " << cities << endl;
	cout << "Number of Nodes Generated: " << numNodes << endl;
	cout << "Path: " << path << endl;
	return 0;
}
