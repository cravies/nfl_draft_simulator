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
#include "utils.h"
#include "data_structures.h"

using namespace std;

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

        void erase_need(team &t, string need) {
            // erase a need from a team
            t.needs.erase(remove(t.needs.begin(), t.needs.end(), need), t.needs.end());
            cout << "erased " << need << " from " << t.name << "\n";
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
                    for (int j=0; j<myteams.size(); j++) {
                        if (myteams[j].name==myteams[t].name) {
                            erase_need(myteams[j],pos);
                            // add position to "picked" list
                            myteams[j].picks.push_back(pos);
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

        // just take the best player available (usually first on the board)
        // unless the team doesn't need a QB, in which case they'll skip over them
        // I.e the bears won't pick Bryce Young at 1 over Jalen Carter at 2.
        void BPA(vector<team> &myteams, vector<player> &big_board, pick &mypick, int t) {
            string pos;
            vector <string> tneeds = myteams[t].needs;
            // otherwise take BPA (best player available)
            // might have to iterate until we get a non QB if we aren't taking a QB
            for (int i=0; i<big_board.size(); i++) {
                // if they match a need, erase from needs array
                pos = big_board[i].pos;
                // check - pass if they are a QB and this team isn't taking one
                // or if its a position this team has already taken
                // or just randomly skip 10% of the time.
                if ((myteams[t].taking_QB==false && pos=="QB") or contains(myteams[t].picks,pos) or (get_random()<=0.05)) {
                    cout << "Team " << myteams[t].name << " avoided taking " << big_board[i].name << "\n";
                } else {
                    for (int j=0; j<myteams.size(); j++) {
                        if (myteams[j].name==myteams[t].name) {
                            if (contains(tneeds,pos)) {
                                // erase from needs every version of team (i.e texans 2, texans 12)
                                erase_need(myteams[j],pos);
                            }
                            //add to "picked" list to avoid duplicate position picks
                            myteams[j].picks.push_back(pos);
                        }
                    }
                    cout << "~~~~~~~~~~~~~\n" << myteams[t].name << " BPA. " << big_board[i].name << "\n";
                    // set player as pick
                    mypick.player = big_board[i];
                    // erase player (BPA) from big board
                    big_board.erase(big_board.begin() + i);
                    // pick made, exit
                    return;
                }
            }
        }

        void make_pick(vector<team> &myteams, int t, vector<player> &big_board, pick &mypick) {
            cout << "Making pick \n";
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
            BPA(myteams, big_board, mypick, t);
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

int main(){

    // temp vars for player readin
    string name;
    string pos;
    int num;

    // temp vars for team readin
    string needs;
    float reach_prob;
    string QB;

    // big board and team vectors
    vector <player> players;
    vector <team> teams;

    // load players
    ifstream inputPlayers("./settings/players.txt");
    int count = 1;
    while (inputPlayers >> name >> pos) {
        player p;
        p.name = regex_replace(name, regex("_"), " ");
        p.pos = pos;
        p.num = count;
        players.push_back(p);
        count += 1;
    }

    // load teams
    ifstream inputTeam("./settings/teams.txt");
    while (inputTeam >> name >> needs >> reach_prob >> QB) {
        team t;
        t.name = regex_replace(name, regex("_"), " ");
        t.needs = split(needs,',');
        t.reach_prob = reach_prob;
        t.taking_QB = (QB=="Y") ? true : false;
        cout << name << needs[0] << reach_prob << QB << "\n";
        teams.push_back(t);
    }

    // simulate draft 1000 times
    cout << "simulating\n";
    draftSimulator mock(players, teams);
    mock.print_teams();
    mock.mock_draft(10);
    vector<pick> p = mock.get_picks();

    // write to file
    write_picks(p, "./output/draft.txt");
}