#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
#include "instance.h"
using namespace std;

/*
* Find the profit from visited clusters
*/
ll P(vector<vi> &u, vi &profit) {
    // cerr<<"Calculating profit for the current configuration....."<<endl;
    ll  p = 0;
    for(ll  i = 0; i < sz(u)-1; i++) {
        for(ll  cluster: u[i])
            p += profit[cluster];
    }
    // cerr<<"Calculation of profit for the current configuration done"<<endl;
    return p;
}

/*
* Print profit obtained by the final sequence
*/
void printProfit(Solution &t, Instance &s) {
    // cerr<<"Printing max profit"<<endl;
    cout<<"Max profit is: "<<P(t.u, s.profit)<<endl;
    // cerr<<"Printing max profit done"<<endl;
}

/*
* Print list of cities (in order) visited by each salesman
*/
void printTour(Solution &t) {
    // cerr<<"Printing Tour"<<endl;
    cout<<"Set of visited vertices is: "<<endl;
    for(ll  i = 0; i < sz(t.u)-1; i++) {
        cout<<"sequence of sets for salesman "<<i+1<<": { ";
        // vector<ll> salesmanSet = ;
        for(ll  set: t.u[i]) {
            cout<<set<<" ";
        }
        cout<<"}"<<endl;
    }
    cout<<"sets not visited by any salesmen are: ";
    for(ll  i: t.u[sz(t.u)-1])
        cout<<i<<" ";
    cout<<endl;
    cerr<<"Printing Tour done"<<endl;
}

/*
* Print list of cities (in order) visited by each salesman
*/
void printTourFile(Solution &t, ofstream& outfile) {
    // cerr<<"Printing Tour"<<endl;
    // outfile<<"Set of visited vertices is: "<<endl;
    for(ll  i = 0; i < sz(t.u)-1; i++) {
        // outfile<<"sequence of sets for salesman "<<i+1<<": { ";
        // vector<ll> salesmanSet = ;
        for(ll  set: t.u[i]) {
            outfile<<set<<" ";
        }
        outfile << endl;
        // outfile<<"}"<<endl;
    }
    // outfile<<"sets not visited by any salesmen are: ";
    for(ll  i: t.u[sz(t.u)-1])
        outfile<<i<<" ";
    outfile<<endl;
    // cerr<<"Printing Tour done"<<endl;
}

//correlates an index position to a postion among the multiple set of travellers
ii find_pos_in_u(vector<vi> &u, ll  index) {
    // cerr<<"Finding position of "<<index<<" in u"<<endl;
    // printTour(u);
    // cout<<"index = "<<index<<endl;
    index++;
    ll  cur = 0;
    ii response = {0,0};
    for(ll  i = 0; i < sz(u); i++) {
        if(cur+sz(u[i]) >= index) {
            response = {i, index-cur-1};
            break;
        } else {
            cur += sz(u[i]);
        }

        // cout << i << " " << cur << endl;
    }
    // cerr<<"Finding position of "<<index-1<<" in u done"<<endl;
    // cout<<"response = "<<response.ff<<" "<<response.ss<<endl;
    return response;
}

/*
* Finds the next state of dp given the start and end clusters and the previous dp values
*/
vector<pair<ll,ll>> find_next_state_of_dp(Instance &s, ll  cluster1, ll  cluster2, vector<pair<ll,ll>> dp_row) {
    // cerr<<"Finding the next dp state for movement from "<<cluster1<<" to "<<cluster2<<"....."<<endl;

    vector<pair<ll,ll>> dp_cur(sz(s.cities_in_cluster[cluster2]), {INF,0});
    for(ll  id1 = 0; id1 < sz(s.cities_in_cluster[cluster1]); id1++) {
        for(ll  id2 = 0; id2 < sz(s.cities_in_cluster[cluster2]); id2++) {
            // dp_cur[id2] = min(dp_cur[id2], dp_row[id1]+d[cities_in_cluster[cluster1][id1]][cities_in_cluster[cluster2][id2]]);
            if(dp_cur[id2].ff > dp_row[id1].ff + s.cost[s.cities_in_cluster[cluster1][id1]][s.cities_in_cluster[cluster2][id2]]){
                dp_cur[id2].ff = dp_row[id1].ff + s.cost[s.cities_in_cluster[cluster1][id1]][s.cities_in_cluster[cluster2][id2]];
                dp_cur[id2].ss = id1;
            }

            // cout << id1 << " " << id2 << " " << dp_cur[id2].ff << endl;
        }
    }
    // cout <<"Solutions found for next dp state for movement from "<<cluster1<<" to "<<cluster2<<endl;
    return dp_cur;
}

/*
* Finds the minimum cost for the given sequence of clusters over all salesmen
*/

// Add depot cost here
vi  L(vector<vi> &u, Instance &s, bool printCity=false) {
    // cerr<<"Calculating cost for the current configuration....."<<endl;
    // printTour(u);
    vi  cost(sz(u) - 1,0);
    
    for(ll  i = 0; i < sz(u)-1; i++) {
        // cout<<"i = "<<i<<endl;
        vector<vector<pair<ll,ll>>> dp(sz(u[i])+2); // Minimum cost of reaching each vertex of each cluster
        //first stores min cost, second stores prev city
        dp[0].pb({0,1});
        ll  cluster1 = 0;
        for(ll  j = 0; j < sz(u[i]); j++) {
            ll  cluster2 = u[i][j];
            // cout << "prev cluster: " << cluster1 <<" next cluster: " << cluster2 << endl;
            dp[j+1] = find_next_state_of_dp(s, cluster1, cluster2, dp[j]);
            cluster1 = u[i][j];
        }
        ll  cluster2 = 0;

        dp[sz(u[i])+1] =  find_next_state_of_dp(s, cluster1, cluster2, dp[sz(u[i])]);
        cost[i] += dp[sz(u[i])+1][0].ff;
        // cout << ", Move Cost: " << dp[sz(u[i])+1][0].ff << ", New Cost: " << cost << endl;

        if(printCity){
            string temp = "";
            ll  k = sz(u[i]), j;

            if(k != 0){
                j = dp[k+1][0].ss;
                cout<<"last city visited by salesman "<<i+1<<": "<<s.cities_in_cluster[u[i][k-1]][j]<<"\n";
            }

            while(k>0){
                temp = to_string(s.cities_in_cluster[u[i][k-1]][j])+" "+temp;
                k--;
                j = dp[k+1][j].ss;
            }
            cout<<"Path of cities for salesman "<<i+1<<": "<<temp<<"\n";
        }
    }
    // cout <<"Calculation of cost for the current configuration done: " << cost << endl;
    return cost;
}

/*
* Prints the cost of given configuration
*/
void printCost(Solution &t, Instance &s, bool printCity=false) {
    // cerr<<"Printing Cost of tour"<<endl;
    vi cost = L(t.u, s, printCity);
    ll tot = 0;
    for(int i=0; i<sz(cost); i++){
        cout<<"Cost for traveller "<< i+1 << " is " << cost[i] <<endl;
        tot += cost[i];
    }

    cout << "Cost of tour is: " << tot << endl; 
    // cerr<<"Printing Cost of tour done"<<endl;
}

ll getTourCost(const vi &cost){
    return accumulate(all(cost), 0ll);
}


bool tourInvalid(vector<vi> &u, Instance &s){
    vi tourCosts = L(u, s); 
    return *max_element(all(tourCosts)) > s.T;
}

void preprocess_costs(Solution &t, Instance &s, int modified = -1) {
    if (modified == -1) {
        for (int i = 0; i < sz(t.u) - 1; i++) {
            preprocess_costs(t, s, i);
        }
        return;
    }

    int i = modified;
    for (int j : t.u[i]) {
        t.g[j] = INF;
        t.h[j] = INF;
        for (int k : s.cities_in_cluster[j]) {
            t.dp[k] = INF;
            t.rdp[k] = INF;
        }
    }

    if (t.u[i].empty()) {
        return;
    }

    for (int k : s.cities_in_cluster[t.u[i][0]]) {
        for (int v : s.cities_in_cluster[0]) {
            t.dp[k] = min(t.dp[k], t.dp[v] + s.cost[v][k]);
        }
    }
    for (int j = 1; j < sz(t.u[i]); j++) {
        for (int k : s.cities_in_cluster[t.u[i][j]]) {
            for (int v : s.cities_in_cluster[t.u[i][j - 1]]) {
                t.dp[k] = min(t.dp[k], t.dp[v] + s.cost[v][k]);
            }
        }
    }
    for (int k : s.cities_in_cluster[t.u[i].back()]) {
        for (int v : s.cities_in_cluster[0]) {
            t.rdp[k] = min(t.rdp[k], t.rdp[v] + s.cost[k][v]);
        }
    }
    for (int j = sz(t.u[i]) - 2; j >= 0; j--) {
        for (int k : s.cities_in_cluster[t.u[i][j]]) {
            for (int v : s.cities_in_cluster[t.u[i][j + 1]]) {
                t.rdp[k] = min(t.rdp[k], t.rdp[v] + s.cost[k][v]);
            }
        }
    }
    for (int j : t.u[i]) {
        for (int k : s.cities_in_cluster[j]) {
            t.g[j] = min(t.g[j], t.dp[k]);
            t.h[j] = min(t.h[j], t.rdp[k]);
        }
    }
}


/*
* Returns false if any salesman doesn't visit any city. True otherwise
*/
bool no_salesmen_empty(vector<vector<ll>>& u) {
    for(ll  i = 0; i < sz(u)-1; i++) {
        if(sz(u[i]) == 0)
            return false;
    }
    return true;
}

/*
* Objective function for algorithm 1 of constructing solutions
*/
ld objective_function_algorithm_1(vector<vector<ll>>& u1, vector<vector<ll>>& u2, Instance &s, ll  p_extra) {
    // printTour(u1);
    return (getTourCost(L(u1, s))-getTourCost(L(u2, s)))/(ld)p_extra;
}
