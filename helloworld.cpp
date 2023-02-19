#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <map>
#include <string>
#include <regex>

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
    // string of csv positional team needs (per PFF)
    // i.e G,C,WR. Process into arr in class to make things nicer
    string needs;
    // what percentage of the time will the team 
    // reach for a play who plays the position of need
    float reach_prob; 
};

// mock draft class
class mockDraft {
    int sims;
    vector<player> big_board;
    vector<team> teams;

    public: 
        // constructor
        mockDraft(int sims, vector<player> big_board, vector<team> teams) {
            // number of monte carlo simulations
            sims=sims;
            // player rankings and positions
            big_board = big_board;
            // teams with needs and reach probabilities
            teams = teams;
        }
        // board getter
        void get_board() {
            for (player p: big_board) {
                cout << "player name: " << p.name << "\n";
                cout << "player position: " << p.pos << "\n";
                cout << "PFF big board ranking" << p.num << "\n";
            }
        }
        // team getter
        void get_teams() {
            for (team t: teams) {
                cout << "team name: " << t.name << "\n";
            }
        }
};

int main(){

    int count = 0;

    // temp vars for player readin
    string name;
    string pos;
    int num;

    // temp vars for team readin
    string needs;
    // have to read needs string into tmp var before splitting into arr
    string needs_tmp;
    float reach_prob;

    // big board and team vectors
    vector <player> big_board;
    vector <team> teams;

    // load players
    ifstream inputPlayers("players.txt");
    while (inputPlayers >> name >> pos >> num) {
        player p;
        p.name = regex_replace(name, regex("_"), " ");
        p.pos = pos;
        p.num = num;
        big_board.push_back(p);
    }

    for (player p: big_board) {
        cout << "player name: " << p.name << "\n";
        cout << "player position: " << p.pos << "\n";
        cout << "PFF big board ranking: " << p.num << "\n";
    }

    // load teams
    ifstream inputTeam("teams.txt");
    while (inputTeam >> name >> needs >> reach_prob) {
        team t;
        t.name = regex_replace(name, regex("_"), " ");
        t.needs = needs;
        t.reach_prob = reach_prob;
        teams.push_back(t);
    }

    for (team t: teams) {
        cout << "team name: " << t.name << "\n";
        cout << "team needs: " << t.needs << "\n";
        cout << "team reach prob: " << t.reach_prob << "\n";
    }
}