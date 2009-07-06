#include "GlobalLogger.h"

vector<LoggerMessage> GlobalLogger::messages;

void GlobalLogger::log(string message) {
	LoggerMessage cur;
	cur.message = message;
	cur.time = ofGetElapsedTimef();
	messages.push_back(cur);
}

void GlobalLogger::save(string filename) {
	ofstream logFile;
	logFile.open(ofToDataPath(filename).c_str());
	for(int i = 0; i < messages.size(); i++)
		logFile << messages[i].time << "," << messages[i].message << endl;
	logFile.close();
}
