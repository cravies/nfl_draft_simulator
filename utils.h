#include <vector>
#include <random>
#include <string>
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

void sort_by_position_value(vector<player> &players) {
    // let's sort by positional value
    // based on analysis at https://overthecap.com/positional-value-in-the-nfl
    map<string, double> posValue;
    posValue["QB"]=2.94;
    posValue["ED"]=1.63;
    posValue["WR"]=1.46;
    posValue["T"]=(1.37+1.03)/2; // halfway between LT and RT, model doesn't seperate yet
    posValue["DI"]=1.37;
    posValue["CB"]=1.23;
    posValue["LB"]=1.13;
    posValue["S"]=1.04;
    posValue["G"]=0.99;
    posValue["RB"]=0.89;
    posValue["TE"]=0.85;
    posValue["C"]=0.77;
    cout << "done" << endl;
}

void load_players(string filename, vector<player> &players) {
    // temp vars for player readin
    string name;
    string pos;
    // load players
    ifstream inputPlayers(filename);
    int count = 1;
    while (inputPlayers >> name >> pos) {
        player p;
        p.name = regex_replace(name, regex("_"), " ");
        p.pos = pos;
        p.num = count;
        players.push_back(p);
        count += 1;
    }
    // sort by positional value
    // sort_by_position_value(players);
}

void load_teams(string filename, vector<team> &teams) {
    // temp vars for player readin
    string name;
    string pos;
    int num;
    // temp vars for team readin
    string needs;
    float reach_prob;
    string QB;
    // load teams
    ifstream inputTeam(filename);
    while (inputTeam >> name >> needs >> reach_prob >> QB) {
        team t;
        t.name = regex_replace(name, regex("_"), " ");
        t.needs = split(needs,',');
        t.reach_prob = reach_prob;
        t.taking_QB = (QB=="Y") ? true : false;
        cout << name << needs[0] << reach_prob << QB << "\n";
        teams.push_back(t);
    }
}