# nfl_draft_simulator
Monte carlo simulation of 2023 NFL draft first round based on PFF big board rankings and team needs.

Each player is defined by a struct
```C++
// Player struct
struct player {
    string name; // player name
    string pos; // position
    int num; // PFF big board ranking
};
```

And each team is defined by a struct
```C++
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
```

Teams will sometimes (based on probabilities you set with reach_prob) randomly reach for a player at a position of need.
Quarterback needy teams will do this way more often.
Python file outputs probability distribution of each player landing at each team.
