//Baker.cpp Done Mostly by Matt Passarelli
//Matt: 90%
//Same: 10%

#include <mutex>
#include <thread>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/constants.h"
using namespace std;

//ID is just a number used to identify this particular baker
//(used with PRINT statements)
Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {

	/**
	 * Objects that are used for the ORDERS that are
	 * being fulfilled.
	 * Using GLAZED as opposed to anything else since
	 * that's what we're told to use in externs.h
	 */
	Box box;
	DONUT donut;
	donut.myType = GLAZED;

	/**
	 * Iterate through the ORDER's specificed number of dounts
	 * stopping at DOZEN to push the box to the order, then
	 * continues if there are still donuts. Once we reach the
	 * last donut in the ORDER, finish that box and push it.
	 */
	for (int i = 0; i < anOrder.number_donuts; i++) {
		if (box.size() < DOZEN) {
			box.addDonut(donut);
		}
		if (box.size() == DOZEN) {
			anOrder.boxes.push_back(box);
			box.clear();
		}
		if (box.size() > 0 && i == anOrder.number_donuts - 1) {
			anOrder.boxes.push_back(box);
		}
	}
}

void Baker::beBaker() {

	/**
	 * Here we wait until Waiter signifies or until the
	 * order_in_Q is empty
	 */
	while (!b_WaiterIsFinished || !order_in_Q.empty()) {

		//lock the mutex with a unique lock
		unique_lock<mutex> lck(mutex_order_inQ);

		//here we wait for the condition variables to be signaled
		//to continues
		while (!b_WaiterIsFinished && !order_in_Q.empty())
			cv_order_inQ.wait(lck);

		if (!order_in_Q.empty()) {

			//do the ORDER
			bake_and_box(order_in_Q.front());

			/**
			 * Lock the order out mutex so we can push to the final
			 * Order out Vector
			 * Then unlock
			 */
			mutex_order_outQ.lock();

			order_out_Vector.push_back(order_in_Q.front());

			mutex_order_outQ.unlock();

			//Remove the ORDER we just did from the Queue
			order_in_Q.pop();

			//unlock the unique lock so other threads can grab
			//then notifity
			lck.unlock();
			cv_order_inQ.notify_all();

			/**----------------------------------------------------
			 * Okay, let me rant about this method right here
			 *
			 * It took days to figure out why, after no matter
			 * what we tried, only one thread would do all of
			 * the work, despite having multiple threads
			 * running. Turns out, that, at the very least, my
			 * processor is too fast (it's like 3.2-3.5GHz or so)
			 * and that as soon as one thread unlocks to allow
			 * another thread to grab the mutex, the one that just
			 * unlocked itself grabs it again, and again, and again,
			 * until the operations are finished. It's so fucking
			 * weird and I have no idea why.
			 *
			 * So, by making the thread that's running wait, even,
			 * just 1 damn nanosecond, it's long enough for another
			 * thread to grab the mutex so someone else can do the
			 * work. Once the current thread waits, it's honestly
			 * just luck on what other thread grabs it, but at
			 * least this makes it so the same thread doesn't
			 * grab the mutex like a greedy little snake.
			 * ----------------------------------------------------
			 */
			this_thread::sleep_for(chrono::nanoseconds(1));
		}
	}
}

