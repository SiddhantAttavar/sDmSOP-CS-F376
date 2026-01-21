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
vector<ii> remove_1(vector<vector<ll>> u, Solution &s) {
    int i = rand() % (s.r - sz(u.back()) - 1);
    return {find_pos_in_u(u, i)};
}

/*
 * Worst cost removal
 */
vector<ii> remove_2(vector<vector<ll>> u, Solution &s) {
    auto [g, h, dp, rdp] = preprocess_costs(u, s, -1);
    vector<pair<ll, ii>> cost;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll base_cost = 0;
        if (!u[i].empty()) {
            base_cost = INF;
            for (int k : s.cities_in_cluster[u[i][0]]) {
                base_cost = min(base_cost, dp[k] + rdp[k]);
            }
        }
        for (int j = 0; j < sz(u[i]); j++) {
            int prev = j > 0 ? u[i][j - 1] : 0, next = j < sz(u[i]) - 1 ? u[i][j + 1] : 0;
            ll removal_cost = INF;
            for (int p : s.cities_in_cluster[prev]) {
                for (int q : s.cities_in_cluster[next]) {
                    removal_cost = min(removal_cost, dp[p] + s.cost[p][q] + rdp[q]);
                }
            }
            cost.push_back({removal_cost - base_cost, {i, j}});
        }
    }

    sort(cost.begin(), cost.end());
    return {rp_select(cost, 5).second};
}

/*
 * Worst profit removal
 */
vector<ii> remove_3(vector<vector<ll>> &u, Solution &s) {
    vector<pair<ll, ii>> cost;
    for (ll i = 0; i < sz(u) - 1; i++) {
        for (ll j = 0; j < sz(u[i]); j++) {
            cost.push_back({s.profit[u[i][j]], make_pair(i, j)});
        }
    }

    sort(cost.begin(), cost.end());
    return {rp_select(cost, 10).second};
}

/*
 * Worst profit over cost removal
 */
vector<ii> remove_4(vector<vector<ll>> &u, Solution &s) {
    auto [g, h, dp, rdp] = preprocess_costs(u, s, -1);
    vector<pair<ii, ii>> cost;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll base_cost = 0;
        if (!u[i].empty()) {
            base_cost = INF;
            for (int k : s.cities_in_cluster[u[i][0]]) {
                base_cost = min(base_cost, dp[k] + rdp[k]);
            }
        }
        for (int j = 0; j < sz(u[i]); j++) {
            int prev = j > 0 ? u[i][j - 1] : 0, next = j < sz(u[i]) - 1 ? u[i][j + 1] : 0;
            ll removal_cost = INF;
            for (int p : s.cities_in_cluster[prev]) {
                for (int q : s.cities_in_cluster[next]) {
                    removal_cost = min(removal_cost, dp[p] + s.cost[p][q] + rdp[q]);
                }
            }
            cost.push_back({
                make_pair(s.profit[u[i][j]], removal_cost - base_cost),
                make_pair(i, j)
            });
        }
    }

    sort(cost.begin(), cost.end(), [](pair<ii, ii> a, pair<ii, ii> b) {
        return a.first.first * b.first.second < b.first.first * a.first.second;
    });
    return {rp_select(cost, 4).second};
}

vector<ii> remove_5(vector<vector<ll>> &u, Solution &s) {
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
        return {};
    }

    auto [p, q] = rp_select(cost, 10).second;
    return {p, q};
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
    while (x > 0) {
        vector<ii> p;
        switch(l) {
            case 0:
                p = remove_1(u, s);
                break;
            case 1:
                p = remove_2(u, s);
                break;
            case 2:
                p = remove_3(u, s);
                break;
            case 3:
                p = remove_4(u, s);
                break;
            case 4:
                p = remove_5(u, s);
                break;
        }
        // cerr<<"Shaking the configuration done"<<endl;
        sort(all(p));
        reverse(all(p));
        for (auto [i, j] : p) {
            int v = u[i][j];
            u.back().push_back(v);
            u[i].erase(u[i].begin() + j);
        }
        x -= max(1ll, sz(p));
    }
    auto end = std::chrono::high_resolution_clock::now();
    ll time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    return {u, l, time};
}

#endif
