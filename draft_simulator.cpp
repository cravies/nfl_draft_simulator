#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
#include <string>
#include <regex>
#include <random>
#include <sstream>
#include <algorithm>

using namespace std;

// Player struct
struct player {
    string name; // player name
    string pos; // position
    int num; // PFF big board ranking
};

// Team struct
// Note that for teams with multiple picks there will be multiple of these in the array
struct team {
    string name;
    // array of csv positional team needs (per PFF)
    // i.e [G,C,WR]
    vector<string> needs;
    // what percentage of the time will the team 
    // reach for a play who plays the position of need
    float reach_prob; 
};

struct pick {
    player player;
    team team;
};

// mock draft simulator class
class draftSimulator {
    vector<player> players;
    vector<team> teams;
    vector<pick> picks;

    public: 

        // constructor
        draftSimulator(vector<player> bb, vector<team> tms) {
            // player rankings and positions
            players=bb;
            // teams with needs and reach probabilities
            teams=tms;
        }

        // board printer
        void print_board() {
            cout << "board getter\n";
            for (player p: players) {
                cout << "player name: " << p.name << "\n";
                cout << "player position: " << p.pos << "\n";
                cout << "PFF big board ranking" << p.num << "\n";
            }
        }

        // team printer
        void print_teams() {
            cout << "team getter\n";
            for (team t: teams) {
                cout << "team name: " << t.name << "\n";
            }
        }

        float get_random() {
            // get random float from (0->1)
            static default_random_engine e;
            static uniform_real_distribution<> dis(0, 1); // rage 0 - 1
            return dis(e);
        }

        // see if an array contains something
        template<class C, typename T>
        bool contains(C&& c, T e) { return find(begin(c), end(c), e) != end(c); };

        void erase_need(team &t, string need) {
            // erase a need from a team
            t.needs.erase(remove(t.needs.begin(), t.needs.end(), need), t.needs.end());
            cout << "erased " << need << "\n";
        }

        void print_needs(team t) {
            cout << "Team " << t.name << " needs ";
            for (string n : t.needs) {
                cout << "," << n;
            }
            cout << "\n";
        }

        void reach(vector<team> &myteams, vector<player> &big_board, pick &mypick, int t) {
            string pos;
            cout << "~~~~~~~~~~~~~\n" << myteams[t].name << " reaching. \n";
            for (int i=0; i<big_board.size(); i++) {
                cout << big_board[i].name << "\n";
                pos = big_board[i].pos;
                if (contains(myteams[t].needs,pos)) {
                    cout << myteams[t].name << " reached for " << big_board[i].name << "\n";
                    // get rid of need from team needs list
                    print_needs(myteams[t]);
                    // erase for every version of team
                    for (int i=0; i<myteams.size(); i++) {
                        if (myteams[i].name==myteams[t].name) {
                            erase_need(myteams[i],pos);
                        }
                    }
                    print_needs(myteams[t]);
                    // pick player
                    mypick.player = big_board[i];
                    // erase player from big board
                    big_board.erase(big_board.begin() + i);
                    return;
                }
            }
        }

        void make_pick(vector<team> &myteams, int t, vector<player> &big_board, pick &mypick) {
            string pos;
            mypick.team = myteams[t];
            // see if we reach for a position of need.
            // other wise just take best player available (big_board[0])
            if (get_random() < myteams[t].reach_prob && contains(myteams[t].needs,big_board[0].pos)==false) {
                // we are reaching. If we find a player in a 
                // position of need, grab em.
                reach(myteams, big_board, mypick, t);
                return;
            }
            // otherwise take BPA (best player available)
            mypick.player = big_board[0];
            // if they match a need, erase from needs array
            pos = big_board[0].pos;
            if (contains(myteams[t].needs,pos)) {
                // erase for every version of team
                for (int i=0; i<myteams.size(); i++) {
                    if (myteams[i].name==myteams[t].name) {
                        erase_need(myteams[i],pos);
                    }
                }
            }
            // erase player (BPA) from big board
            big_board.erase(big_board.begin() + 0);
            return;
        }

        // simulate a draft and add results to picks
        void mock_draft(int sims) {
            // simulate sims # of times
            for (int i=0; i<sims; i++) {
                // want our own player queue cause we will be popping picks
                vector<player> big_board = players;
                // also want our own team needs because we will be popping them as well
                // i.e texans won't pick quarterback at pick 2 and pick 12!
                vector<team> my_teams = teams;
                pick cur_pick;
                player p;
                // let each team pick
                for (int j=0; j<my_teams.size(); j++) {
                    // pick is made, erased from big board.
                    make_pick(my_teams, j, big_board, cur_pick);
                    picks.push_back(cur_pick);
                }
            }
        }

        // print results of all the mock drafts we have simulated
        vector<pick> get_picks() {
            return picks;
        }
};

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

int main(){

    int count = 0;

    // temp vars for player readin
    string name;
    string pos;
    int num;

    // temp vars for team readin
    string needs;
    float reach_prob;

    // big board and team vectors
    vector <player> players;
    vector <team> teams;

    // load players
    ifstream inputPlayers("players.txt");
    while (inputPlayers >> name >> pos >> num) {
        player p;
        p.name = regex_replace(name, regex("_"), " ");
        p.pos = pos;
        p.num = num;
        players.push_back(p);
    }

    // load teams
    ifstream inputTeam("teams.txt");
    while (inputTeam >> name >> needs >> reach_prob) {
        team t;
        t.name = regex_replace(name, regex("_"), " ");
        t.needs = split(needs,',');
        t.reach_prob = reach_prob;
        teams.push_back(t);
    }

    // simulate draft 1000 times
    draftSimulator mock(players, teams);
    mock.mock_draft(1000);
    vector<pick> p = mock.get_picks();

    // write to file
    write_picks(p, "draft.txt");
}