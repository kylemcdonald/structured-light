#pragma once

#include "ofxDirList.h"

class ofxNumericalDirList : public ofxDirList {
public:
	string getName(int pos) {
		return numericalNameArray[pos];
	}
	string getPath(int pos) {
		return directory + numericalNameArray[pos];
	}
	// doesn't let you mix extensions
	int listDir(string directory) {
		numericalNameArray.clear();
		int size = ofxDirList::listDir(directory);
		vector<int> ints;
		int intNum;
		string extension;
		for(int i = 0; i < size; i++) {
			string cur = ofxDirList::getName(i);
			int pos = cur.rfind(".");
			stringstream strNum(cur.substr(0, pos));
			extension = cur.substr(pos, cur.length());
			strNum >> intNum;
			ints.push_back(intNum);
		}
		sort(ints.begin(), ints.end());
		for(int i = 0; i < size; i++) {
			stringstream curName;
			curName << ints[i] << extension;
			numericalNameArray.push_back(curName.str());
		}
		this->directory = ofToDataPath(directory);
		return size;
	}

// because ofxDirList is private, we have to make our own
protected:
	string directory;
	vector<string> numericalNameArray;
};
