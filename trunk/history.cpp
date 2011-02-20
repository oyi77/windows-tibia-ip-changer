#include "history.h"

bool History::saveHistory(string new_host){
    ifstream file("./history");
    if(!file.is_open())
		return false;

	vector<string>out;
    string line; 
    while(getline(file, line)){
		if(line != new_host)
			out.push_back(line);
    }
    out.insert(out.begin(), new_host);

    ofstream plik("./history");
    for(int i = 0; i < int(out.size()); i++)
		plik<<out.at(i)<<endl; 
	plik.close();
    return true;
}

vector<string> History::getHistory(){
	vector<string>tab;
    ifstream file("./history");
    if(!file.is_open())
		return tab;

    string line; 
    while(getline(file, line))
		tab.push_back(line);
	
    return tab;
}
