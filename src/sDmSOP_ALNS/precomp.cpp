#ifndef PRECOMP_H
#define PRECOMP_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
using namespace std;

/*
* Set cities that are in each cluster
*/
void set_cities_in_cluster(Solution &s) {
    // cerr<<"Setting cities in each cluster"<<endl;
    s.cities_in_cluster.resize(s.r);
    for(ll  i=0; i<s.r; i++) {
        for(ll  j=0; j<=s.n; j++) {
            if(s.cluster[i][j])
                s.cities_in_cluster[i].pb(j);
        }
    }
    
    // cerr<<"Setting cities in each cluster done"<<endl;
}


/*
 * Calculate proximity for each cluster
 */
void calculate_proximity(Solution &s) {
    s.proximity.resize(s.r, {0, 0});
    for (int i = 0; i < s.r; i++) {
        for (int u : s.cities_in_cluster[i]) {
            for (int v : s.cities_in_cluster[i]) {
                s.proximity[i].first += s.cost[u][v];
            }
        }
        s.proximity[i].second = ((ll) s.cities_in_cluster[i].size()) * s.cities_in_cluster[i].size();
    }
}


void calculate_relatedness(Solution &s) {
    s.relatedness.resize(s.r);
    for (int i = 0; i < s.r; i++) {
        s.relatedness[i].resize(s.r, 0);
        for (int j = i + 1; j < s.r; j++) {
            ll cost_max = 0;
            for (int u : s.cities_in_cluster[i]) {
                for (int v : s.cities_in_cluster[j]) {
                    s.relatedness[i][j] += s.cost[u][v];
                    cost_max = max(cost_max, s.cost[u][v]);
                }
            }
            s.relatedness[i][j] /= sz(s.cities_in_cluster[i]) * sz(s.cities_in_cluster[j]);
            s.relatedness[i][j] /= cost_max;
        }
    }
    for (int i = 0; i < s.r; i++) {
        for (int j = 0; j < i; j++) {
            s.relatedness[i][j] = s.relatedness[j][i];
        }
    }
}

void calculate_extended_cost(Solution &s) {
    s.extended_cost.resize(s.n + s.r + 1, vector<ll>(s.n + s.r + 1, INF));
    for (int i = 0; i < s.n; i++) {
        for (int j = 0; j < s.n; j++) {
            s.extended_cost[i + s.r][j + s.r] = s.cost[i][j];
        }
    }
    for (int i = 0; i < s.n; i++) {
        for (int j = 0; j < s.r; j++) {
            for (int k : s.cities_in_cluster[j]) {
                s.extended_cost[i + s.r][j] = min(s.extended_cost[i + s.r][j], s.cost[i][k]);
                s.extended_cost[j][i + s.r] = min(s.extended_cost[j][i + s.r], s.cost[k][i]);
            }
        }
    }
    for (int i = 0; i < s.r; i++) {
        for (int j = 0; j < s.r; j++) {
            for (int k : s.cities_in_cluster[j]) {
                s.extended_cost[i][j] = min(s.extended_cost[i][j], s.extended_cost[i][k + s.r]);
            }
        }
    }
}

#endif
