#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <vector>
#include <string>

using namespace std;

// Player struct
struct player {
    string name; // player name
    string pos; // position
    double num; // value ranking
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
    // will this team take a quarterback if it is the BPA?
    bool taking_QB;
    // what position has this team picked at (for multi round drafts)
    vector<string> picks;
};

struct pick {
    player player;
    team team;
};

#endif