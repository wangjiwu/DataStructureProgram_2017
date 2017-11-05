#ifndef PLANE
#define PLANE 

enum Plane_status {null, arriving, departing}; 

class Plane {
public:
	Plane();
	Plane(int flt, int time, Plane_status status);
	void refuse() const;
	void land(int time) const;
	void fly(int time) const;
	int started() const;
private:
	int flt_num;
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
protected: 
	int count;  
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

#endif
