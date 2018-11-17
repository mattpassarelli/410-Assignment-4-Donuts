//Waiter.cpp. Done Mostly by Sam Reinhart
//Sam: 90%
//Matt: 10%

#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/datastructs.h"
#include "../includes/externs.h"
#include "../includes/constants.h"
#include "../includes/file_IO.h"

using namespace std;

Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	/**
	 * run getNext with the order we have.
	 * Possible outputs are SUCCESS, NO_ORDERS,
	 * and COULDN_NOT_OPEN_FILE. All self
	 * explanatory
	 */
	if (myIO.getNext(anOrder) == SUCCESS) {
		return SUCCESS;
	} else if (myIO.getNext(anOrder) == NO_ORDERS) {
		return NO_ORDERS;
	} else {
		return COULD_NOT_OPEN_FILE;
	}
}

void Waiter::beWaiter() {

	/**
	 * Blank ORDER Object that we'll use
	 * to set ORDER information on then
	 * push to queue
	 */
	ORDER myOrder;

	/**
	 * Grab the next order from the file. If it is a SUCCESS
	 * then we lock the mutex, push it to the queue, unlock the
	 * mutex (the point of having it in brackets), then notify bakers
	 * once all of the orders are pulled in
	 */
	while (Waiter::getNext(myOrder) == SUCCESS) {

		{
			unique_lock<mutex> lck(mutex_order_inQ);
			order_in_Q.push(myOrder);
		}
		cv_order_inQ.notify_all();

	}

	/**
	 * Once we're done with the orders,
	 * we do one final mutex lock so we can
	 * set the WaiterIsFinished to true,
	 * unlock, then one final notify
	 */
	{
		unique_lock<mutex> lck(mutex_order_inQ);
		b_WaiterIsFinished = true;
	}

	cv_order_inQ.notify_all();
}

