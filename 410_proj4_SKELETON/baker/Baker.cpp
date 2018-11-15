#include <mutex>

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

	//TODO remove cout functions

	Box box;
	DONUT donut;
	donut.myType = GLAZED;

	for (int i = 0; i < anOrder.number_donuts; i++) {
		if (box.size() < DOZEN) {
			box.addDonut(donut);
			cout << "Adding Donut. Box has " << box.size()
					<< " donuts out of a total of " << anOrder.number_donuts
					<< " donuts." << endl;
		}
		if (box.size() == DOZEN) {
			cout << "Box has " << box.size()
					<< " donuts. Should have DOZEN. Pushing... " << endl;
			anOrder.boxes.push_back(box);
			box.clear();
			cout << "Cleaning box. Now has " << box.size()
					<< " donuts. Should have 0" << endl;
		}
		if (box.size() > 0 && i == anOrder.number_donuts - 1) {
			cout << "Box has " << box.size() << " donut. Pushing... " << endl;
			anOrder.boxes.push_back(box);
		}
	}

	cout << "we have " << anOrder.boxes.size() << " boxes. We should have "
			<< (anOrder.number_donuts / 12) + 1 << " boxes." << endl;
}

void Baker::beBaker() {

	//TODO uncomment those comment blocks for "production"


	while (true) {
		unique_lock<mutex> lck(mutex_order_outQ);

		cout << "Waiting for Baking to signal" << endl;

		while (order_in_Q.empty()) {
			cv_order_inQ.wait(lck);
		}

		cout << "Baker signal received. Starting..." << endl;

		if (order_in_Q.size() > 0) {
			cout << "Starting order..." << endl;
			bake_and_box(order_in_Q.front());

			cout << "Pushing Order to vector..." << endl;
			order_out_Vector.push_back(order_in_Q.front());

			cout << "Removing from IN Q" << endl;
			order_in_Q.pop();
		}

		if (order_in_Q.empty() && b_WaiterIsFinished) {
			cout<<"Breaking from beBaker"<<endl;
			break;
		}

		//TODO remove this for "production"
//		if (order_in_Q.empty()) {
//			cout << "Breaking from beBaker..." << endl;
//			break;
//		}
	}
}
