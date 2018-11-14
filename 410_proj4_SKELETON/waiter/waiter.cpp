#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/datastructs.h"
#include "../includes/externs.h"
#include "../includes/constants.h"
#include "../includes/file_IO.h"

using namespace std;
ORDER myOrder;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	if(myIO.getNext(anOrder) == SUCCESS) {
		return SUCCESS;
	}
	else if(myIO.getNext(anOrder) == NO_ORDERS) {
		return NO_ORDERS;
	}
	else {
		return COULD_NOT_OPEN_FILE;
	}
}

void Waiter::beWaiter() {
	int i = 0;
	while(Waiter::getNext(myOrder) == SUCCESS) {
		i++;
		order_in_Q.push(myOrder);
		if(i == 1) {
			cv_order_inQ.notify_all();
		}
	}
	if(Waiter::getNext(myOrder) == NO_ORDERS) {
		b_WaiterIsFinished = true;
		cv_order_inQ.notify_all();
	}
}

