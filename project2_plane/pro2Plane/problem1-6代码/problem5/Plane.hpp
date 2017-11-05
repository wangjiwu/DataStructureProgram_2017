#ifndef PLANE
#define PLANE 
#include "Plane.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
enum Plane_status {null, arriving, departing}; 


class Plane {
public:
	Plane();
	Plane(int flt, int time, Plane_status status,int total_times);
	void refuse() const;
	void land(int time) const;
	void fly(int time) const;
	int started() const;
	int oil_level=rand();
	int flt_num;
private:

	
	int clock_start;
	Plane_status state;
};


typedef Plane Queue_entry; 
enum Error_code {success,overflow ,underflow,fail};

class Queue 
{  
public:   
	Queue();   
	bool empty()const;  
	Error_code serve();   
	Error_code append(const Queue_entry &item);  
	Error_code retrieve(Queue_entry &item)const; 
	Queue_entry& retrieve();
	int count; 
protected: 
	 
	int front,rear;   
	Queue_entry entry[1000]; 
}; 

class Extended_queue:public Queue 
{  
public:   
	int size()const{
		return count;
	}
}; 
Plane::Plane(int flt, int time, Plane_status status,int total_times)
{
	
	flt_num = flt;
	clock_start = time;
	state = status;
	oil_level=static_cast<int>(rand()/(total_times-time));
	cout << "Plane number " << flt << " ready to ";
	if (status == arriving)
		cout << "land." << endl;
	else
		cout << "take off." << endl;
} 
/*
Post:	The Plane data members flt_num, clock_start,
and state are set to the values of the parameters flt,
time and status, respectively.
*/


Plane::Plane()
{
	flt_num = -1;
	clock_start = -1;
	state = null;
}
/*
Post:	The Plane data members flt_num, clock_start,
state are set to illegal default values.
*/


void Plane::refuse() const
{
	cout << "Plane number " << flt_num;
	if (state == arriving)
		cout << " directed to another airport" << endl;
	else
		cout << " told to try to takeoff again later" << endl;
}
/*
Post: Processes a Plane wanting to use Runway, when
the Queue is full.
*/


void Plane::land(int time) const
{
	int wait = time - clock_start;
	cout << time << ": Plane number " << flt_num << " landed after "
	<< wait << " time unit" << ((wait == 1) ? "" : "s")
	<< " in the landing queue." << endl;
}
/*
Post: Processes a Plane that is landing at the specified time.
*/


void Plane::fly(int time) const
{
	int wait = time - clock_start;
	cout << time << ": Plane number " << flt_num << " took off after "
	<< wait << " time unit" << ((wait == 1) ? "" : "s")
	<< " in the takeoff queue." << endl;
}
/*
Post: Process a Plane that is taking off at the specified time.
*/


int Plane::started() const
{
	return clock_start;
} 
/*
Post: Return the time that the Plane entered the airport system.
*/

Queue::Queue() 
{   
	count=0;  
	front=0;  
	rear=999; 
}   

bool Queue::empty()const 
{   
	return count==0; 
}   
Queue_entry& Queue::retrieve()
{
	return entry[front];
}
Error_code Queue::append(const Queue_entry &item) 
{   
	if(count>1000)   
		return overflow;  
	count++;   
	rear=((rear+1)==1000)?0:(rear+1);  
	entry[rear]=item;  
	return success; 
}

Error_code Queue::serve() 
{   
	if(count<=0) 
		return underflow;  
	count--;   
	front=((front+1)==1000)?0:(front+1);  
	return success; 
}  

Error_code Queue::retrieve(Queue_entry &item)const 
{   
	if(count<=0) 
		return underflow;  
	item=entry[front];  
	return success; 
}
#endif
