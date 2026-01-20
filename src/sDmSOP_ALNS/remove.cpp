#ifndef REMOVE_H
#define REMOVE_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
#include "calc.h"
using namespace std;

/*
 * Random remove
 */
vector<vector<ll>> remove_1(vector<vector<ll>> u, Solution &s) {
    int i = rand() % (s.r - sz(u.back()) - 1);
    ii p = find_pos_in_u(u, i);
    u.back().push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
 * Worst cost removal
 */
vector<vector<ll>> remove_2(vector<vector<ll>> u, Solution &s) {
    vector<pair<ll, ii>> cost;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll x = cost_of_path(u[i], s);
        for (int j = 0; j < sz(u[i]); j++) {
            vector<ll> l = u[i];
            l.erase(l.begin() + j);
            cost.push_back({cost_of_path(l, s) - x, {i, j}});
        }
    }

    sort(cost.begin(), cost.end());
    ii p = rp_select(cost, 5).second;

    u.back().push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
 * Worst profit removal
 */
vector<vector<ll>> remove_3(vector<vector<ll>> u, Solution &s) {
    vector<pair<ll, ii>> cost;
    for (ll i = 0; i < sz(u) - 1; i++) {
        for (ll j = 0; j < sz(u[i]); j++) {
            cost.push_back({s.profit[u[i][j]], make_pair(i, j)});
        }
    }

    sort(cost.begin(), cost.end());
    ii p = rp_select(cost, 10).second;

    u.back().push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
 * Worst profit over cost removal
 */
vector<vector<ll>> remove_4(vector<vector<ll>> u, Solution &s) {
    vector<pair<ii, ii>> cost;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll x = cost_of_path(u[i], s);
        for (int j = 0; j < sz(u[i]); j++) {
            vector<ll> l = u[i];
            l.erase(l.begin() + j);
            cost.push_back({
                make_pair(s.profit[u[i][j]], cost_of_path(l, s) - x),
                make_pair(i, j)
            });
        }
    }

    sort(cost.begin(), cost.end(), [](pair<ii, ii> a, pair<ii, ii> b) {
        return a.first.first * b.first.second < b.first.first * b.first.second;
    });
    ii p = rp_select(cost, 4).second;

    u.back().push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

vector<vector<ll>> remove_5(vector<vector<ll>> u, Solution &s) {
    vector<pair<ll, ii>> l;
    for (int i = 0; i < sz(u) - 1; i++) {
        for (int j = 0; j < sz(u[i]); j++) {
            l.push_back({u[i][j], {i, j}});
        }
    }
    vector<pair<ld, pair<ii, ii>>> cost;
    for (int i = 0; i < sz(l); i++) {
        for (int j = i + 1; j < sz(l); j++) {
            ld x = 1;
            if (l[i].second.first == l[j].second.second) {
                x = ((ld) abs(l[i].second.second - l[j].second.second)) / sz(u[l[i].second.first]);
            }
            cost.push_back({
                PHI_1 * s.relatedness[i][j] + PHI_2 * x,
                {l[i].second, l[j].second}
            });
        }
    }
    sort(cost.begin(), cost.end());
    if (cost.empty()) {
        return u;
    }

    auto [p, q] = rp_select(cost, 10).second;
    tie(p, q) = minmax(p, q);
    u.back().push_back(u[q.first][q.second]);
    u[q.first].erase(u[q.first].begin() + q.second);
    u.back().push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
* Remove procedure. Contains two different neighborhood structures:
* 1. Neighborhood 1 (Path move):
*                       Select a path i.e. a subarray and index and move the
*                       subpart to that position. While selecting path we ensure
*                       that the whole path is taken from the same salesman.
* 2. Neighborhood 2 (Path exchange):  
*                       Select two paths and exchange them. Again while 
*                       paths we select each path from one salesman. 
*
* In all cases we keep the new values only if they satisfy all the hard constraints.
* We can add more neighborhoods structures later, if needed.
*/
tuple<vector<vi>, int, ll> remove(Solution &s) {
    // cerr<<"Shaking the configuration....."<<endl;
    int k = s.r - sz(s.u[s.m]) - 1;
    if (k == 0) {
        return {s.u, -1, 0};
    }
    auto start = std::chrono::high_resolution_clock::now();
    int x = getRand(4, min(40, (int) (REMOVAL_CONSTANT * k)));
    // int x = getRand(4, REMOVAL_CONSTANT * k);
	vector<vi> u = s.u;
    int l = priority_select(s.removal_weights);
    while (x--) {
        vector<vector<ll>> u1 = u;
        switch(l) {
            case 0:
                u1 = remove_1(u, s);
                break;
            case 1:
                u1 = remove_2(u, s);
                break;
            case 2:
                u1 = remove_3(u, s);
                break;
            case 3:
                u1 = remove_4(u, s);
                break;
            case 4:
                u1 = remove_5(u, s);
                break;
        }
        // cerr<<"Shaking the configuration done"<<endl;
        u = u1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    ll time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    return {u, l, time};
}


#endif
