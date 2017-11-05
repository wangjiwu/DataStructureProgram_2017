#include <utility> 
#include <iostream>
#include <vector>
#include <numeric>      // std::accumulate
#include "Runway.hpp"
#include "Plane.hpp"
#include "random.hpp"
using namespace std;
typedef Plane Queue_entry;
vector<int> arriving_num;
vector<int> takeoff_num;
void initialize(int &end_time, int &queue_limit)
{	

	cerr << "This program simulates an airport with only one runway." << endl
	<< "One plane can land or depart in    each unit of time." << endl;
	cerr << "Up to what number of planes can be waiting to land "
	<< "or take off at any time? " << flush;
	cin	>> queue_limit;
	cerr << "How many units of time will the simulation run?" << flush;
	cin	>> end_time;
	
	while (1) {
		cout<<"key in the circle you want to customaize about landing"<<endl;
		for(int i=0;i<end_time;i++) {
			int n;
			cin>>n;
			if (n < 0) {
				cerr << "right posstive num" << endl;
				i--;
				continue;
			}
	    	arriving_num.push_back(n);
		}

		cout<<"key in the circle you want to customaize about departure"<<endl;
		for(int i=0;i<end_time;i++) {
	    	int n;
	    	cin>>n;
	    	if (n < 0) {
				cerr << "right posstive num" << endl;
				i--;
				continue;
			}
	    	takeoff_num.push_back(n);
		}

		if (accumulate(arriving_num.begin(), arriving_num.end(), 0)/end_time +
		accumulate(takeoff_num.begin(), takeoff_num.end(), 0)/end_time > 1) {
			cerr << "Safety Warning: This airport will become saturated. "<< endl;
			arriving_num.clear();
			takeoff_num.clear();
		} else {
			break;
		}

	}
}



/*
Pre:	The user specifies the number of time units in the simulation,
the maximal queue sizes permitted,
and the expected arrival and departure rates for the airport.
Post: The program prints instructions and initializes the parameters
end_time, queue_limit, arrival_rate, and departure_rate to
the specified values.
Uses: utility function user_says_yes
*/

void run_idle(int time)
{
	cout << time << ": Runway is idle." << endl; //o
} 
/*
Post: The specified time is printed with a message that the runway is idle.
*/

int main()
{
	int end_time;	//	time to run simulation
	int queue_limit;	//	size of Runway queues
	int flight_number = 0;

	initialize(end_time, queue_limit);
	
	Runway small_airport(queue_limit);
	
	for (int current_time = 0; current_time < end_time; current_time++) {
	//	loop over time intervals
		int number_arrivals = arriving_num[current_time];
	//	current arrival requests
		for (int i = 0; i < number_arrivals; i++) {
			Plane current_plane(flight_number++, current_time, arriving);
			if (small_airport.can_land(current_plane) != success)
				current_plane.refuse();
		}
		int number_departures= takeoff_num[current_time];
	//	current departure requests
		for (int j = 0; j < number_departures; j++) {
			Plane current_plane(flight_number++, current_time, departing);
			if (small_airport.can_depart(current_plane) != success)
				current_plane.refuse();
		}
		Plane moving_plane;
		switch (small_airport.activity(current_time, moving_plane)) {
	//	Let at most one Plane onto the Runway at current_time.
			case land:
			moving_plane.land(current_time);
			break;
			case take_off:
			moving_plane.fly(current_time);
			break;
			case idle:
			run_idle(current_time);
		}
	}
	small_airport.shut_down(end_time);
}
//	Airport simulation program
/*
Pre:	The user must supply the number of time intervals the simulation
is to run, the expected number of planes arriving, the expected
number of planes departing per time interval, and the maximum
allowed size for runway queues.
Post: The program performs a random simulation of the airport, showing
the status of the runway at each time interval, and prints out a
summary of airport operation at the conclusion.
Uses: Classes Runway, Plane, Random and functions run_idle, initialize.
*/
