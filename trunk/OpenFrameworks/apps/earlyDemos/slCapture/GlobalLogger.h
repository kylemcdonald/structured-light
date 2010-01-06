#pragma once

#include <fstream>
#include "ofMain.h"

struct LoggerMessage {
	string message;
	double time;
};

class GlobalLogger {
public:
	static void log(string message);
	static void save(string filename);
private:
	static vector<LoggerMessage> messages;
};
