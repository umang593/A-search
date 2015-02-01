// a_star_as.cpp
// libraries
#include <queue>
#include <string>
#include <math.h>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;


// define city structure
class City {
public:
	string n;
	int x;
	int y;
	City () {}
	City(string Name, int xPos, int yPos) {
		n = Name;
		x = xPos;
		y = yPos;
	}
	string getName() {return n;}
	int getX() {return x;}
	int getY() {return y;}
	double distanceTo(City c) {
		int xDistance = abs(x - c.getX());
		int yDistance = abs(y - c.getY());
		double distance = sqrt(xDistance*xDistance + yDistance*yDistance);
		return distance;
	}
};

// define tour manager structure
vector<City> TourManager;

// define tour structure
class Tour {
public:
	// holds our tour of cities
	vector<City> tour;
	// distance travelled so far in the tour
	int distance;
	// construct a tour
	Tour() {distance = 0;}
	// constructs a tour from another tour
	Tour(vector<City> T) {tour = T;}
	// returns tour information
	vector<City> getTour() {return tour;}
	// sets a city in a certain position
	void setCity(int tourPosition, City city) {
		tour.at(tourPosition) = city;
	}
	// gets a city from the tour
	City getCity(int tourPosition) {
		return tour.at(tourPosition);
	}
	// returns number of cities in tour
	int tourSize() {return tour.size();}
	// creates random tour
	void createRandomTour() {
		for (int i = 0; i < TourManager.size(); i++) {
			tour.push_back(TourManager.at(i));
		}
		random_shuffle(tour.begin(),tour.end());
	}
	// gets the total distance of the tour
	int getDistance() {
		int tourDistance = 0;
		for (int i = 0; i < tourSize(); i++) {
			City fromCity = getCity(i);
			City toCity;
			if (i+1 < tourSize()) {
				toCity = getCity(i+1);
			}
			else {
				toCity = getCity(0);
			}
			tourDistance = tourDistance + fromCity.distanceTo(toCity);
		}
		distance = tourDistance;
		return distance;
	}
			
};

// Simulated annealing function
double SimulatedAnnealing(double VNew, double VOld, double temperature) {
	if (VOld < VNew) {
		return 1.0;
	}
	else {
		return exp(-(VOld-VNew)/temperature);
	}
}


int main () {
	// Create and add cities to tour;
	srand(time(NULL));
	int cities;
	scanf("%d", &cities);
	for (int i = 0; i < cities; i++) {
		char tmp[3];
		string Name;
		int x;
		int y;
		scanf("%2s %d %d", tmp, &x, &y);
		Name = tmp;
		City nTemp(Name, x, y);
		TourManager.push_back(nTemp);
	}

	//set initial temperature
	double temp = 36;

	//cooling rate
	double coolingRate = 0.1;

	// initialize initial solution;
	Tour current = Tour();
	current.createRandomTour();

	cout << "Initial solution distance: " << current.getDistance() << endl;

	// current best solution
	Tour best = Tour(current.getTour());
		
	while (temp > 1) {
	// create wew neighbour tour
	Tour New = Tour(current.getTour());

	// get random position in tour
	int tourPos1 = rand() %  New.tourSize();
	int tourPos2 = rand() %  New.tourSize();

	// get the cities at selected position in tour
	City citySwap1 = New.getCity(tourPos1);
	City citySwap2 = New.getCity(tourPos2);

	// swap 
	New.setCity(tourPos2, citySwap1);
	New.setCity(tourPos1, citySwap2);

	// generate V values
	double VOld = current.getDistance();
	double VNew = New.getDistance();

	// accept neighbour?
	if (SimulatedAnnealing(VNew, VOld, temp) > ((double)rand()/(RAND_MAX))) { 
		current = Tour(New.getTour());
	}
	
	// best solution?
	if (current.getDistance() < best.getDistance()) {
		best = Tour(current.getTour());
	}

	// decrease temperature by cooling rate
	temp = temp*(1-coolingRate);

	}
	cout << "Final solution distance: " << best.getDistance() << endl;
	string path = "";
	for (int i = 0; i < best.tourSize(); i++) {
		path = path + best.getCity(i).n;
	}
	cout << "Tour: " << path << endl;
}
	
