#ifndef __HISTORY_H__
#define __HISTORY_H__
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class History
{
public:
    bool saveHistory(string);
	vector<string> getHistory();
};
#endif
