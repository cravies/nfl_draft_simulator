#include <vector>
#include <random>
#include "data_structures.h"

using namespace std;

// for string splitting team needs "G,C,LB" into array -> ["G","C","LB"]
template <typename Out>
void split(const string &s, char delim, Out result) {
    istringstream iss(s);
    string item;
    while (getline(iss, item, delim)) {
        *result++ = item;
    }
}
vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

float get_random() {
    // get random float from (0->1)
    static default_random_engine e;
    static uniform_real_distribution<> dis(0, 1); // rage 0 - 1
    return dis(e);
}

// see if an array contains something
template<class C, typename T>
bool contains(C&& c, T e) { 
    return find(begin(c), end(c), e) != end(c); 
}

// Write all our simulated picks to a file
void write_picks(vector<pick> &p, string fname) {
    // Open file fname
    ofstream MyFile(fname);
    // Write to the file
    string team;
    string player;
    for (int i=0; i<p.size(); i++) {
        MyFile << p[i].team.name << "," << p[i].player.name << "\n";
    }
    // Close the file
    MyFile.close();
}