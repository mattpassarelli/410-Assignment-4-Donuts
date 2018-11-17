/**
 * Logger.cpp. Done by both Matt and Sam
 * Matt: 50%
 * Sam: 50%
 */

#include "../includes/logger.h"
using namespace std;

Logger::Logger(std::string filename) {
	this->filename = filename;
}

Logger::~Logger() {
}

//open close and clear the log file
void Logger::clearlogfile() {

	/**
	 * What better way to make sure 2 threads
	 * don't try to latch on the same process
	 * than by literally locking the entire
	 * method, lol, until it finishes or, in theory,
	 * an exception is thrown and the method
	 * terminates
	 */
	lock_guard<mutex> lock(m);
	myFile.open(filename, std::fstream::trunc);

	//close file
	if (myFile.is_open())
		myFile.close();
}

void Logger::log(std::string data) {

	/**
	 * What better way to make sure 2 threads
	 * don't try to latch on the same process
	 * than by literally locking the entire
	 * method, lol, until it finishes or, in theory,
	 * an exception is thrown and the method
	 * terminates
	 */

	lock_guard<mutex>lock(m);
	myFile.open(filename, std::fstream::app);
	if (!myFile.is_open())
		return;

	std::string myline;

	myFile << data;

	//close file
	if (myFile.is_open())
		myFile.close();
}
