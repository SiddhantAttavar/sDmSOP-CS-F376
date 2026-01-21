#ifndef CALC_H
#define CALC_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
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
* Prll  profit obtained by the final sequence
*/
void printProfit(Solution &s) {
    // cerr<<"Printing max profit"<<endl;
    cout<<"Max profit is: "<<P(s.u, s.profit)<<endl;
    // cerr<<"Printing max profit done"<<endl;
}

/*
* Prll  list of cities (in order) visited by each salesman
*/
void printTour(Solution &s) {
    // cerr<<"Printing Tour"<<endl;
    cout<<"Set of visited vertices is: "<<endl;
    for(ll  i = 0; i < sz(s.u)-1; i++) {
        cout<<"sequence of sets for salesman "<<i+1<<": { ";
        // vector<ll> salesmanSet = ;
        for(ll  set: s.u[i]) {
            cout<<set<<" ";
        }
        cout<<"}"<<endl;
    }
    cout<<"sets not visisted by any salesmen are: ";
    for(ll  i: s.u[sz(s.u)-1])
        cout<<i<<" ";
    cout<<endl;
    cerr<<"Printing Tour done"<<endl;
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
vector<pair<ll,ll>> find_next_state_of_dp(Solution &s, ll  cluster1, ll  cluster2, vector<pair<ll,ll>> dp_row) {
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
vi  L(vector<vi> &u, Solution &s, bool printCity=false) {
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
void printCost(Solution &s, bool printCity=false) {
    // cerr<<"Printing Cost of tour"<<endl;
    vi cost = L(s.u, s, printCity);
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


bool tourInvalid(vector<vi> &u, Solution &s){
    vi tourCosts = L(u, s); 
    return *max_element(all(tourCosts)) > s.T;
}

array<vi, 4> preprocess_costs(vector<vector<ll>> &u, Solution &s, int modified = -1) {
    vector<ll> dp(s.n + 1, INF), rdp(s.n + 1, INF);
    for (int k : s.cities_in_cluster[0]) {
        dp[k] = 0;
        rdp[k] = 0;
    }
    for (int i = 0; i < sz(u) - 1; i++) {
        if (u[i].empty() or (modified != -1 and modified != i)) {
            continue;
        }
        for (int k : s.cities_in_cluster[u[i][0]]) {
            for (int v : s.cities_in_cluster[0]) {
                dp[k] = min(dp[k], dp[v] + s.cost[v][k]);
            }
        }
        for (int j = 1; j < sz(u[i]); j++) {
            for (int k : s.cities_in_cluster[u[i][j]]) {
                for (int v : s.cities_in_cluster[u[i][j - 1]]) {
                    dp[k] = min(dp[k], dp[v] + s.cost[v][k]);
                }
            }
        }
        for (int k : s.cities_in_cluster[u[i].back()]) {
            for (int v : s.cities_in_cluster[0]) {
                rdp[k] = min(rdp[k], rdp[v] + s.cost[k][v]);
            }
        }
        for (int j = sz(u[i]) - 2; j >= 0; j--) {
            for (int k : s.cities_in_cluster[u[i][j]]) {
                for (int v : s.cities_in_cluster[u[i][j + 1]]) {
                    rdp[k] = min(rdp[k], rdp[v] + s.cost[k][v]);
                }
            }
        }
    }
    vector<ll> g(s.r, INF), h(s.r, INF);
    for (int i = 0; i < s.r; i++) {
        for (int j : s.cities_in_cluster[i]) {
            g[i] = min(g[i], dp[j]);
            h[i] = min(h[i], rdp[j]);
        }
    }
    return {g, h, dp, rdp};
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

#endif
