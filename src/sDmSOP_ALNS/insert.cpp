#ifndef INSERT_H
#define INSERT_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
#include "calc.h"
using namespace std;

pair<vector<ll>, vector<ll>> preprocess_costs(vector<vector<ll>> &u, Solution &s) {
    vector<ll> dp(s.n + 1, INF), rdp(s.n + 1, INF);
    for (int k : s.cities_in_cluster[0]) {
        dp[k] = 0;
        rdp[k] = 0;
    }
    for (int i = 0; i < sz(u); i++) {
        if (u[i].empty()) {
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
    return {g, h};
}

vector<pair<ll, ii>> insertion_costs(vector<vector<ll>> &u, Solution &s, int v) {
    auto [g, h] = preprocess_costs(u, s);
    vector<pair<ll, ii>> res;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll x = cost_of_path(u[i], s);
        for (int j = 0; j <= sz(u[i]); j++) {
            int prev = j > 0 ? u[i][j - 1] : 0, next = j < u[i].size() ? u[i][j] : 0;
            ll bound1 = g[prev] + s.extended_cost[prev][v] + s.extended_cost[v][next] + h[next];
            if (bound1 > s.Tmax / s.m) {
                continue;
            }
            ll z = g[prev] + h[next];
            ll bound2 = INF;
            for (int k : s.cities_in_cluster[v]) {
                bound2 = min(bound2, z + s.extended_cost[prev][k + s.r] + s.extended_cost[k + s.r][next]);
            }
            if (bound2 > s.Tmax / s.m) {
                continue;
            }

            vector<ll> l = u[i];
            l.insert(l.begin() + j, v);
            ll y = cost_of_path(l, s);
            if (y <= s.Tmax / s.m) {
                res.push_back({y - x, {i, j}});
            }
        }
    }
    return res;
}

ii find_least_cost_insertion_position(vector<vector<ll>> &u, Solution &s, int v) {
    vector<pair<ll, ii>> l = insertion_costs(u, s, v);
    if (l.empty()) {
        return {-1, -1};
    }
    return min_element(l.begin(), l.end())->second;
}

vector<vector<ll>> predetermined_insert(vector<vector<ll>> u, Solution &s, int rp) {
    vector<ll> v;
    while (!u.back().empty()) {
        int i = rp_select(u.back(), rp);
        u.back().erase(find(u.back().begin(), u.back().end(), i));
        ii p = find_least_cost_insertion_position(u, s, i);
        if (p.first == -1) {
            v.push_back(i);
            continue;
        }
        assert(i < s.r);
        u[p.first].insert(u[p.first].begin() + p.second, i);
    }
    u.back() = v;
    return u;
}

/*
 * Random insertion
 */
vector<vector<ll>> insert_1(vector<vector<ll>> &u, Solution &s) {
    shuffle(u.back().begin(), u.back().end(), rng);
    return predetermined_insert(u, s, 5);
}

/*
 * Profit based insertion
 */
vector<vector<ll>> insert_2(vector<vector<ll>> &u, Solution &s) {
    sort(u.back().begin(), u.back().end(), [&](int a, int b) {
        return s.profit[a] > s.profit[b];
    });
    return predetermined_insert(u, s, 4);
}

/*
 * Proximity based insertion
 */
vector<vector<ll>> insert_3(vector<vector<ll>> &u, Solution &s) {
    sort(u.back().begin(), u.back().end(), [&](int a, int b) {
        return s.proximity[a].first * s.proximity[b].second > s.proximity[b].first * s.proximity[a].second;
    });
    return predetermined_insert(u, s, 6);
}

vector<vector<ll>> parallel_insert(vector<vector<ll>> u, Solution &s, function<int(vector<vector<pair<ll, ii>>>&)> select) {
    vector<ll> left;
    while (!u.back().empty()) {
        vector<vector<pair<ll, ii>>> costs;
        vector<ll> v;
        vector<ll> temp;
        for (int i : u.back()) {
            vector<pair<ll, ii>> cost = insertion_costs(u, s, i);
            if (!cost.empty()) {
                costs.push_back(cost);
                temp.push_back(i);
            }
            else {
                v.push_back(i);
                left.push_back(i);
            }
        }
        u.back() = temp;
        int j = select(costs);
        if (j != -1) {
            ii p = min_element(costs[j].begin(), costs[j].end())->second;
            u[p.first].insert(u[p.first].begin() + p.second, u.back()[j]);
            u.back().erase(u.back().begin() + j);
        }
    }
    u.back() = left;
    return u;
}

/*
 * Cheapest insertion
 */
vector<vector<ll>> insert_4(vector<vector<ll>> &u, Solution &s) {
    return parallel_insert(u, s, [](vector<vector<pair<ll, ii>>> &v) {
        ii res = {INF, -1};
        for (int i = 0; i < v.size(); i++) {
            if (!v[i].empty()) {
                res = min(res, {min_element(v[i].begin(), v[i].end())->first, i});
            }
        }
        return res.second;
    });
}

/*
 * Dynamic profit over cost insertion
 */
vector<vector<ll>> insert_5(vector<vector<ll>> &u, Solution &s) {
    return parallel_insert(u, s, [&](vector<vector<pair<ll, ii>>> &v) {
        pair<ld, ll> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (!v[i].empty()) {
                res = max(res, {((ld) s.profit[i]) / min_element(v[i].begin(), v[i].end())->first, i});
            }
        }
        return res.second;
    });
}

/*
 * Regret-k insertion
 */
vector<vector<ll>> regret_k_insert(vector<vector<ll>> u, Solution &s, int k) {
    return parallel_insert(u, s, [&](vector<vector<pair<ll, ii>>> &v) {
        pair<ll, int> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) {
                continue;
            }
            if (v[i].size() < k) {
                return i;
            }
            sort(v[i].begin(), v[i].end());
            ll c = 0;
            for (int j = 1; j < k; j++) {
                c += v[i][j].first - v[i][0].first;
            }
            res = max(res, {c, i});
        }
        return res.second;
    });
}

/*
 * Regret-2 insertion
 */
vector<vector<ll>> insert_6(vector<vector<ll>> &u, Solution &s) {
    return regret_k_insert(u, s, 2);
}

/*
 * Regret-3 insertion
 */
vector<vector<ll>> insert_7(vector<vector<ll>> &u, Solution &s) {
    return regret_k_insert(u, s, 3);
}

/*
 * Dynamic profit regret-k insertion
 */
vector<vector<ll>> dynamic_profit_regret_k_insert(vector<vector<ll>> &u, Solution &s, int k) {
    return parallel_insert(u, s, [&](vector<vector<pair<ll, ii>>> &v) {
        pair<ld, int> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) {
                continue;
            }
            if (v[i].size() < k) {
                return i;
            }
            sort(v[i].begin(), v[i].end());
            ll c = 0;
            for (int j = 1; j < k; j++) {
                c += v[i][j].first - v[i][0].first;
            }
            res = max(res, {((ld) s.profit[i]) / c, i});
        }
        return res.second;
    });
}

/*
 * Dynamic profit regret-2 insertion
 */
vector<vector<ll>> insert_8(vector<vector<ll>> &u, Solution &s) {
    return dynamic_profit_regret_k_insert(u, s, 2);
}

/*
 * Dynamic profit regret-3 insertion
 */
vector<vector<ll>> insert_9(vector<vector<ll>> &u, Solution &s) {
    return dynamic_profit_regret_k_insert(u, s, 3);
}

/*
* Local Search. Contains two different neighborhood structures:
* 1. Neighborhood 1 (One cluster move):
*                       Select two random positions i1, i2. Now we can put:
*                       1. i1 after i2 
*                       or 
*                       2. i2 before i1.
* 2. Neighborhood 2 (One cluster exchange):  
*                       Select two random clusters and exchange them.
*
* In both the cases we only keep the solution if it increases the value of P.
* Another option can be to check all n^2 pairs for both neighborhoods.
* We can add more neighborhoods structures later, if needed.
*/
tuple<vector<vi>, int, ll> insert(vector<vector<ll>>& u, Solution &s, int l = -1) {
    // cerr<<"Local search for the configuration....."<<endl;
    if (u.back().empty()) {
        return {u, -1, 0};
    }
    auto start = std::chrono::high_resolution_clock::now();
    vector<vi> u1;
    if (l == -1) {
        l = priority_select(s.insertion_weights);
    }
    switch(l) {
        case 0:
            u1 = insert_1(u, s);
            break;
        case 1:
            u1 = insert_2(u, s);
            break;
        case 2:
            u1 = insert_3(u, s);
            break;
        case 3:
            u1 = insert_4(u, s);
            break;
        case 4:
            u1 = insert_5(u, s);
            break;
        case 5:
            u1 = insert_6(u, s);
            break;
        case 6:
            u1 = insert_7(u, s);
            break;
        case 7:
            u1 = insert_8(u, s);
            break;
        case 8:
            u1 = insert_9(u, s);
            break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    ll time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    // cerr<<"Local search for the configuration done"<<endl;
    return {u1, l, time};
}

#endif
