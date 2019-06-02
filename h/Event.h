/*
 * event.h
 *
 *  Created on: Jun 1, 2019
 *      Author: OS1
 */

#ifndef H_EVENT_H_
#define H_EVENT_H_

typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};

#endif /* H_EVENT_H_ */
