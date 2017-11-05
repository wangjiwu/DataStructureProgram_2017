#ifndef RUNWAY
#define RUNWAY
#include "Runway.hpp"
#include <iostream>
using namespace std;
#include "Plane.hpp"
//#include "queue.hpp"
#include "random.hpp"

enum Runway_activity {idle, land, take_off};

class Runway {
public:
	Runway(int limit);
	Error_code can_land(const Plane &current);
	Error_code can_depart(const Plane &current);
	Runway_activity activity(int time, Plane &moving);
	void queue_time_pp();
	bool check_oil();
	void shut_down(int time) const;
	int crush_num;
private:
	Extended_queue landing;
	Extended_queue takeoff;
	Extended_queue temp;
	int queue_limit;
	int num_land_requests;	// number of planes asking to land
	int num_takeoff_requests; // number of planes asking to take off
	int num_landings;	// number of planes that have landed
	int num_takeoffs;	// number of planes that have taken off
	int num_land_accepted;	// number of planes queued to land
	int num_takeoff_accepted; // number of planes queued to take off
	int num_land_refused;	// number of landing planes refused
	int num_takeoff_refused;	// number of departing planes refused
	int land_wait;	// total time of planes waiting to land
	int takeoff_wait;	// total time of planes waiting to take off
	int idle_time;	// total time runway is idle
	
};
Runway::Runway(int limit)
{
	queue_limit = limit;
	num_land_requests = num_takeoff_requests = 0;
	num_landings = num_takeoffs = 0;
	num_land_refused = num_takeoff_refused = 0;
	num_land_accepted = num_takeoff_accepted = 0;
	land_wait = takeoff_wait = idle_time = 0;
}
/*
Post: The Runway data members are initialized to record no
prior Runway use and to record the limit on queue sizes.
*/

bool Runway::check_oil()
{
	int flag=0,flight_num;
	for(;!landing.empty();)
	{
		if(landing.retrieve().oil_level<=0)
		{
			if(flag==0)
			{
				flag=1;
				flight_num=landing.retrieve().flt_num;
				landing.serve();
			}
			else
			{
					temp.append(landing.retrieve());
			landing.serve();
			}
		}
		else
		{
			temp.append(landing.retrieve());
			landing.serve();
		}
	}
	for(;!temp.empty();)
	{
		landing.append(temp.retrieve());
		temp.serve();
	}
	if(flag==1)
	{
	cout<<"plane number "<<flight_num<<" must get landing right now"<<endl;
	return 1;
	}
	return 0;
}




void Runway::queue_time_pp()
{
	int num_landing_queue=landing.count,num_takeoff_queue=takeoff.count;
	for(;!landing.empty();)
	{
		if(landing.retrieve().oil_level<=0)
		{
			cout<<"plane number"<<landing.retrieve().flt_num<<" has crushed"<<endl;
			cout<<"this landing_queue has"<<landing.count<<"plane"<<((num_landing_queue>1)?" ":"s")<<endl;
			cout<<"this takeoff_queue has"<<takeoff.count<<"plane"<<((num_takeoff_queue)?"":"s")<<endl;
			crush_num++;
			landing.serve();
		}
		else
		{
			landing.retrieve().oil_level--;
			temp.append(landing.retrieve());
			landing.serve();
		}
	}
		for(;!temp.empty();)
		{
			landing.append(temp.retrieve());
			temp.serve();
		}
}

Error_code Runway::can_land(const Plane &current)
{
	Error_code result;
	if (landing.size() < queue_limit)
		result = landing.append(current);
	else
		result = fail;
	num_land_requests++;
	if (result != success)
		num_land_refused++;
	else
		num_land_accepted++;
	return result;
}
/*
Post: If possible, the Plane current is added to the
landing Queue; otherwise, an Error_code of overflow is
returned. The Runway statistics are updated.
Uses: class Extended_queue.
*/


Error_code Runway::can_depart(const Plane &current)
{
	Error_code result;
	if (takeoff.size() < queue_limit)
		result = takeoff.append(current);
	else
		result = fail;
	num_takeoff_requests++; 
	if (result != success)
		num_takeoff_refused++;
	else
		num_takeoff_accepted++;
	return result;
}
/*
Post:	If possible, the Plane current is added to the
takeoff Queue; otherwise, an Error_code of overflow is
returned. The Runway statistics are updated.
Uses: class Extended_queue.
*/


Runway_activity Runway::activity(int time, Plane &moving)
{
	Runway_activity in_progress;
	if (!landing.empty()) {
		landing.retrieve(moving);
		land_wait += time - moving.started();
		num_landings++;
		in_progress = land;
		landing.serve();
	}
	else if (!takeoff.empty()) {
		takeoff.retrieve(moving);
		takeoff_wait += time - moving.started();
		num_takeoffs++;
		in_progress = take_off;
		takeoff.serve();
	}
	else {
		idle_time++;
		in_progress = idle;
	}
	return in_progress;
}
/*
Post: If the landing Queue has entries, its front
Plane is copied to the parameter moving
and a result	land is returned. Otherwise,
if the takeoff Queue has entries, its front
Plane is copied to the parameter moving
and a result	takeoff is returned. Otherwise,
idle is returned. Runway statistics are updated.
Uses: class Extended_queue.
*/


void Runway::shut_down(int time) const
{
	cout << "Simulation has concluded after " << time
	<< " time units." << endl
	<< "Total number of planes processed "
	<< (num_land_requests + num_takeoff_requests) << endl
	<< "Total number of planes asking to land "
	<< num_land_requests << endl
	<< "Total number of planes asking to take off "
	<< num_takeoff_requests << endl 
	<< "Total number of planes accepted for landing "
	<< num_land_accepted << endl
	<< "Total number of planes accepted for takeoff "
	<< num_takeoff_accepted << endl
	<< "Total number of planes refused for landing "
	<< num_land_refused << endl
	<< "Total number of planes refused for takeoff "
	<< num_takeoff_refused << endl
	<< "Total number of planes that landed "
	<< num_landings << endl
	<< "Total number of planes that took off "
	<< num_takeoffs << endl
	<< "Total number of planes left in landing queue "
	<< landing.size() << endl
	<< "Total number of planes left in takeoff queue "
	<< takeoff.size() << endl;
	cout << "Percentage of time runway idle "
	<< 100.0 * (( float ) idle_time) / (( float ) time) << "%" << endl;
	cout << "Average wait in landing queue "
	<< (( float ) land_wait) / (( float ) num_landings)
	<< " time units";
	cout << endl << "Average wait in takeoff queue "
	<< (( float ) takeoff_wait) / (( float ) num_takeoffs)
	<< " time units" << endl;
	cout << "Average observed rate of planes wanting to land "
	<< (( float ) num_land_requests) / (( float ) time)
	<< " per time unit" << endl;
	cout << "Average observed rate of planes wanting to take off "
	<< (( float ) num_takeoff_requests) / (( float ) time)
	<< " per time unit" << endl;
	cout<<"crushed_number is "<<crush_num<<endl;
}
/*
Post: Runway usage statistics are summarized and printed.
*/
#endif
