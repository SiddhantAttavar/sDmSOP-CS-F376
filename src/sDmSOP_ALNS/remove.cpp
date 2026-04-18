#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
#include "instance.h"
#include "calc.h"
using namespace std;

/*
 * Random remove
 */
vector<ii> remove_1(Solution &t, Instance &s) {
    int i = rand() % (s.r - sz(t.u.back()) - 1);
    return {find_pos_in_u(t.u, i)};
}

/*
 * Worst cost removal
 */
vector<ii> remove_2(Solution &t, Instance &s) {
    preprocess_costs(t, s, -1);
    vector<pair<ll, ii>> cost;
    for (int i = 0; i < sz(t.u) - 1; i++) {
        ll base_cost = 0;
        if (!t.u[i].empty()) {
            base_cost = INF;
            for (int k : s.cities_in_cluster[t.u[i][0]]) {
                base_cost = min(base_cost, t.dp[k] + t.rdp[k]);
            }
        }
        for (int j = 0; j < sz(t.u[i]); j++) {
            int prev = j > 0 ? t.u[i][j - 1] : 0, next = j < sz(t.u[i]) - 1 ? t.u[i][j + 1] : 0;
            ll removal_cost = INF;
            for (int p : s.cities_in_cluster[prev]) {
                for (int q : s.cities_in_cluster[next]) {
                    removal_cost = min(removal_cost, t.dp[p] + s.cost[p][q] + t.rdp[q]);
                }
            }
            cost.push_back({removal_cost - base_cost, {i, j}});
        }
    }

    sort(cost.begin(), cost.end());
    if (cost.empty()) {
        return {};
    }
    return {rp_select(cost, 5).second};
}

/*
 * Worst profit removal
 */
vector<ii> remove_3(Solution &t, Instance &s) {
    vector<pair<ll, ii>> cost;
    for (ll i = 0; i < sz(t.u) - 1; i++) {
        for (ll j = 0; j < sz(t.u[i]); j++) {
            cost.push_back({s.profit[t.u[i][j]], make_pair(i, j)});
        }
    }

    sort(cost.begin(), cost.end());
    if (cost.empty()) {
        return {};
    }
    return {rp_select(cost, 10).second};
}

/*
 * Worst profit over cost removal
 */
vector<ii> remove_4(Solution &t, Instance &s) {
    preprocess_costs(t, s, -1);
    vector<pair<ii, ii>> cost;
    for (int i = 0; i < sz(t.u) - 1; i++) {
        ll base_cost = 0;
        if (!t.u[i].empty()) {
            base_cost = INF;
            for (int k : s.cities_in_cluster[t.u[i][0]]) {
                base_cost = min(base_cost, t.dp[k] + t.rdp[k]);
            }
        }
        for (int j = 0; j < sz(t.u[i]); j++) {
            int prev = j > 0 ? t.u[i][j - 1] : 0, next = j < sz(t.u[i]) - 1 ? t.u[i][j + 1] : 0;
            ll removal_cost = INF;
            for (int p : s.cities_in_cluster[prev]) {
                for (int q : s.cities_in_cluster[next]) {
                    removal_cost = min(removal_cost, t.dp[p] + s.cost[p][q] + t.rdp[q]);
                }
            }
            cost.push_back({
                make_pair(s.profit[t.u[i][j]], removal_cost - base_cost),
                make_pair(i, j)
            });
        }
    }

    sort(cost.begin(), cost.end(), [](pair<ii, ii> a, pair<ii, ii> b) {
        return a.first.first * b.first.second < b.first.first * a.first.second;
    });
    if (cost.empty()) {
        return {};
    }
    return {rp_select(cost, 4).second};
}

/**
 * Related removal
 */
vector<ii> remove_5(Solution &t, Instance &s) {
    vector<pair<ll, ii>> l;
    for (int i = 0; i < sz(t.u) - 1; i++) {
        for (int j = 0; j < sz(t.u[i]); j++) {
            l.push_back({t.u[i][j], {i, j}});
        }
    }
    vector<pair<ld, pair<ii, ii>>> cost;
    for (int i = 0; i < sz(l); i++) {
        for (int j = i + 1; j < sz(l); j++) {
            ld x = 1;
            if (l[i].second.first == l[j].second.second) {
                x = ((ld) abs(l[i].second.second - l[j].second.second)) / sz(t.u[l[i].second.first]);
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
tuple<Solution, int, ll> remove(Instance &s, Solution &t) {
    // cerr<<"Shaking the configuration....."<<endl;
    int k = s.r - sz(t.u[s.m]) - 1;
    if (k == 0) {
        return {t, -1, 0};
    }
    auto start = std::chrono::high_resolution_clock::now();
    int x = getRand(4, min(40, (int) (REMOVAL_CONSTANT * k)));
    // int x = getRand(4, REMOVAL_CONSTANT * k);
    Solution z = t;
    int l = priority_select(s.removal_weights);
    while (x > 0) {
        int k = s.r - sz(z.u[s.m]) - 1;
        if (k == 0) {
            break;
        }
        vector<ii> p;
        switch(l) {
            case 0:
                p = remove_1(z, s);
                break;
            case 1:
                p = remove_2(z, s);
                break;
            case 2:
                p = remove_3(z, s);
                break;
            case 3:
                p = remove_4(z, s);
                break;
            case 4:
                p = remove_5(z, s);
                break;
        }
        // cerr<<"Shaking the configuration done"<<endl;
        sort(all(p));
        reverse(all(p));
        for (auto [i, j] : p) {
            // TODO implement remove
            int v = z.u[i][j];
            z.u.back().push_back(v);
            z.u[i].erase(z.u[i].begin() + j);
        }
        x -= max(1ll, sz(p));
    }
    auto end = std::chrono::high_resolution_clock::now();
    ll time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    return {z, l, time};
}
