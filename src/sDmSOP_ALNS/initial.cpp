#ifndef INITIAL_H
#define INITIAL_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
#include "calc.h"
using namespace std;

/*
* Objective function for algorithm 1 of constructing solutions
*/
ld objective_function_algorithm_1(vector<vector<ll>>& u1, vector<vector<ll>>& u2, Solution &s, ll  p_extra) {
    // printTour(u1);
    return (getTourCost(L(u1, s))-getTourCost(L(u2, s)))/(ld)p_extra;
}

/*
* Hungarian algorithm based method to find alternate initial solutions. This is used in case when
* the other approach doesn't give a solution in which all the visit atleast 1 set.
* Hungarian algortihm: a is n x m adjacency matrix. in our case it would be m x r. 
*/
void construct_alternate_initial_solution(Solution &s) {
    // cerr<<"Constructing alternate initial solutions....."<<endl;
    
    vector<ll> weight_row(s.r, INF);
    for(ll  j = 1; j < s.r; j++) {
        // cout << "Cluster: " << j <<endl;
        for(ll  city: s.cities_in_cluster[j]){
            weight_row[j] = min(weight_row[j], 2*s.cost[1][city]);
            // cout << "City: " << city << " Weights: " << weight_row[j] << endl;
        }
        // cout << endl;
    }

    // for(ll  j = 1; j < r; j++)
    //     cout<<weight_row[j]<<" ";
    // cout<<endl;

    vector<ll> u (s.m+1), v (s.r), matching (s.r), way (s.r);
    for (ll  i=1; i<=s.m; i++) {
        matching[0] = i;
        ll  j0 = 0;
        vector<ll> minv (s.r, INF);
        vector<char> used (s.r, false);
        do {
            used[j0] = true;
            ll  i0 = matching[j0],  delta = INF,  j1;
            for (ll  j=1; j < s.r; j++){
                // cout << "i0 = " << i0 << " j = " << j << endl;
                if (!used[j]) {
                    ll  cur = weight_row[j]-u[i0]-v[j];
                    // cout << "Cur: " << cur << " minv = " << minv[j] << endl;
                    if (cur < minv[j])
                        minv[j] = cur,  way[j] = j0;
                    if (minv[j] < delta)
                        delta = minv[j],  j1 = j;
                }
            }
            for (ll  j = 1; j < s.r; j++)
                if (used[j])
                    u[matching[j]] += delta,  v[j] -= delta;
                else
                    minv[j] -= delta;
            j0 = j1;
            // cout << "j1 = " << j1 << " matching[j0] = " << matching[j0] << endl;
        } while (matching[j0] != 0);
        do {
            ll  j1 = way[j0];
            matching[j0] = matching[j1];
            j0 = j1;
        } while (j0);
    }
    // cerr<<"Initial solutions constructed"<<endl;
    vector<vector<ll>> ans (s.m+1);
    vector<ll> seen(s.r, 0);
    for (ll  j=1; j <s.r; j++) {
        // cout << matching[j] << endl;
        if(matching[j] != 0) {
            // cout<<"pairing "<<matching[j]<<" with "<<j<<endl;
            ans[matching[j]-1].pb(j);
            seen[j]=1;
        }
    }

    // cout << "Seen: ";
    for(ll  i = 1; i < s.r; i++) {
        // cout << " " << seen[i] << " ";
        if(!seen[i])
            ans[s.m].pb(i);
    }
    // cout << endl;
    // printProfit(ans);
    // printCost(ans);
    // printTour(ans); 

    // cout << "Alternate Initial Solutions Constructed" << endl;
    s.u = ans;
}

/*
* Construct the initial solution.
* Algorithm 1: The algorithm discussed in paper by Robert Penicka, Et. al.
* Iterate over clusters. For each cluster find a position for it in the sequence for each salesman.
* It basically means iterating over all n possibilities for the position. Then choose the position that
* gives the min value of (L(Sigma_{k+1}) - L(Sigma_{k}))/p_i where p_i is profit from the newly added
* cluster, without violating the Tmax condition.
*
* Algorithm 2:
* Iterate over clusters. For each cluster find a position for it in the sequence for each salesman.
* It basically means iterating over all n possibilities for the position. Then choose the position that
* gives the max value of total profit without violating the Tmax condition.
*/
void construct_initial_solution(Solution &s) {
    // cerr<<"Constructing initial solutions....."<<endl;
    s.u = vector<vi>(s.m + 1);
    vector<ll> seen(s.r, 0);
    for(ll  i=1; i<s.r; i++) {
        // cout<<"i = "<<i<<endl;
        ll  id1=s.m, id2=-1;
        ld mn_val = INF;
        for(ll  j=0; j<s.m; j++) {
            // cout<<"j = "<<j<< ", r = "<<r<<", m = "<<m<<endl;

            ll  zero_count = 0; // Number of travellers with no path assigned
            for(ll  i = 0; i < s.m; i++) {
                if(sz(s.u[i])==0){
                    zero_count++;
                    // cout << zero_count << " ";
                }
            }
            // cout << endl;
            // cout<<"check 1 "<<endl;

            //what does this mean?, may not need for this problem, check introduced for mDmSOP
            if((sz(s.u[j]) != 0 && zero_count >= s.r) || zero_count > s.r) {
                // cout<<i<<" "<<j<<endl;
                continue;
            }
            // cout<<"check 2: " << sz(u[j]) <<endl;

            for(ll  pos = 0; pos <= sz(s.u[j]); pos++) {
                // cout<<"pos = "<<pos<<endl;
                vector<vector<ll>> u1 = s.u;
                u1[j].insert(u1[j].begin()+pos, i);
                ld contest_mn_val = objective_function_algorithm_1(u1, s.u, s, s.profit[i]);
                
                // cout<<contest_mn_val<<" "<<mn_val<<" "<<L(u1)<<endl;
    
                if(tourInvalid(u1, s)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
                if(contest_mn_val < mn_val) {
                    mn_val = contest_mn_val;
                    id1 = j, id2 = pos;
                }
            }
        }
        if(id2 != -1) {
            seen[i]=1;
            s.u[id1].insert(s.u[id1].begin()+id2, i);
        }
    }

    for(ll  i = 0; i < s.r; i++) {
        if(!seen[i])
            s.u[s.m].pb(i);
    }
    // cerr<<"Initial solutions constructed"<<endl;
    // printProfit(s);
    // printCost(s);
    // printTour(s);
}

void improve_initial_solution(Solution &s) {
    // cerr<<"Improving initial solutions....."<<endl;
    vector<ll> seen(s.r, 0);
    for(ll  i = 0; i < s.m; i++)
        for(ll  cluster: s.u[i])
            seen[cluster] = 1;
    s.u[s.m].clear();
    for(ll  i = 1; i < s.r; i++) {
        if(seen[i])
            continue;
        ll  id1=s.m, id2=-1;
        ld mn_val = INF;
        for(ll  j = 0; j < s.m; j++) {
            for(ll  pos = 0; pos < sz(s.u[j]); pos++) {
                vector<vector<ll>> u1 = s.u;
                u1[j].insert(u1[j].begin()+pos, i);
                ld contest_mn_val = objective_function_algorithm_1(u1, s.u, s, s.profit[i]);
                if(tourInvalid(u1, s)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
                //CHECK THIS PART...............................................................................................................................................
                if(contest_mn_val < mn_val) {
                    mn_val = contest_mn_val;
                    id1 = j, id2 = pos;
                }
            }
            vector<vector<ll>> u1 = s.u;
            u1[j].pb(i);
            ld contest_mn_val = objective_function_algorithm_1(u1, s.u, s, s.profit[i]);
            if(tourInvalid(u1, s)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
            //CHECK THIS PART...............................................................................................................................................
            if(contest_mn_val < mn_val) {
                mn_val = contest_mn_val;
                id1 = j, id2 = INF;
            }
        }
        if(id2 != -1) {
            seen[i]=1;
            if(id2 == INF) {
                s.u[id1].pb(i);
            } else {
                s.u[id1].insert(s.u[id1].begin()+id2, i);
            }
        }

    }
    for(ll  i = 1; i < s.r; i++) {
        if(!seen[i])
            s.u[s.m].pb(i);
    }
    // cerr<<"Initial solutions Improved"<<endl;
    // printProfit(s);
    // printCost(s);
    // printTour(s);
}

#endif
