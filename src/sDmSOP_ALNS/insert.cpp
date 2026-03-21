#include <algorithm>
#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
#include "instance.h"
#include "calc.h"
using namespace std;

void insertion_costs(vector<pair<ll, ii>> &res, Solution &t, Instance &s, int v, int modified = -1) {
    preprocess_costs(t, s, modified);
    vector<pair<ll, ii>> cost;
    if (modified != -1) {
        for (auto [c, p] : res) {
            if (p.first != modified) {
                cost.push_back({c, p});
            }
        }
    }
    res = cost;

    for (int i = 0; i < sz(t.u) - 1; i++) {
        if (modified != -1 and modified != i) {
            continue;
        }

        ll base_cost = 0;
        if (!t.u[i].empty()) {
            base_cost = INF;
            for (int k : s.cities_in_cluster[t.u[i][0]]) {
                base_cost = min(base_cost, t.dp[k] + t.rdp[k]);
            }
        }
        for (int j = 0; j <= sz(t.u[i]); j++) {
            int prev = j > 0 ? t.u[i][j - 1] : 0, next = j < t.u[i].size() ? t.u[i][j] : 0;
            ll z = t.g[prev] + t.h[next], insert_cost = INF;
            if (z + s.extended_cost[prev][v] + s.extended_cost[v][next] > s.T) {
                continue;
            }
            for (int k : s.cities_in_cluster[v]) {
                if (z + s.extended_cost[prev][k + s.r] + s.extended_cost[k + s.r][next] > s.T) {
                    continue;
                }
                for (int p : s.cities_in_cluster[prev]) {
                    if (t.dp[p] + s.cost[p][k] + s.extended_cost[k + s.r][next] > s.T) {
                        continue;
                    }
                    for (int q : s.cities_in_cluster[next]) {
                        insert_cost = min(insert_cost, t.dp[p] + s.cost[p][k] + s.cost[k][q] + t.rdp[q]);
                    }
                }
            }

            if (insert_cost <= s.T) {
                res.push_back({insert_cost - base_cost, {i, j}});
            }
        }
    }
}

ii find_least_cost_insertion_position(Solution &t, Instance &s, int v) {
    vector<pair<ll, ii>> l;
    insertion_costs(l, t, s, v, -1);
    if (l.empty()) {
        return {-1, -1};
    }
    return min_element(l.begin(), l.end())->second;
}

Solution predetermined_insert(Solution t, Instance &s, int rp) {
    vector<ll> v;
    while (!t.u.back().empty()) {
        int i = rp_select(t.u.back(), rp);
        t.u.back().erase(find(t.u.back().begin(), t.u.back().end(), i));
        ii p = find_least_cost_insertion_position(t, s, i);
        if (p.first == -1) {
            v.push_back(i);
            continue;
        }
        assert(i < s.r);
        t.u[p.first].insert(t.u[p.first].begin() + p.second, i);
    }
    t.u.back() = v;
    return t;
}

/*
 * Random insertion
 */
Solution insert_1(Solution &t, Instance &s) {
    shuffle(t.u.back().begin(), t.u.back().end(), rng);
    return predetermined_insert(t, s, 5);
}

/*
 * Profit based insertion
 */
Solution insert_2(Solution &t, Instance &s) {
    sort(t.u.back().begin(), t.u.back().end(), [&](int a, int b) {
        return s.profit[a] > s.profit[b];
    });
    return predetermined_insert(t, s, 4);
}

/*
 * Proximity based insertion
 */
Solution insert_3(Solution &t, Instance &s) {
    sort(t.u.back().begin(), t.u.back().end(), [&](int a, int b) {
        return s.proximity[a].first * s.proximity[b].second > s.proximity[b].first * s.proximity[a].second;
    });
    return predetermined_insert(t, s, 6);
}

Solution parallel_insert(Solution t, Instance &s, function<int(vector<vector<pair<ll, ii>>>&)> select) {
    vector<ll> left;
    int modified = -1;
    while (!t.u.back().empty()) {
        vector<vector<pair<ll, ii>>> costs;
        vector<ll> temp;
        for (int i : t.u.back()) {
            vector<pair<ll, ii>> cost;
            insertion_costs(cost, t, s, i, modified);
            if (!cost.empty()) {
                costs.push_back(cost);
                temp.push_back(i);
            }
            else {
                left.push_back(i);
            }
        }
        t.u.back() = temp;
        int j = select(costs);
        if (j == -1) {
            break;
        }

        ii p = min_element(costs[j].begin(), costs[j].end())->second;
        t.u[p.first].insert(t.u[p.first].begin() + p.second, t.u.back()[j]);
        t.u.back().erase(t.u.back().begin() + j);
        modified = p.first;
    }
    t.u.back() = left;
    return t;
}

/*
 * Cheapest insertion
 */
Solution insert_4(Solution &t, Instance &s) {
    return parallel_insert(t, s, [](vector<vector<pair<ll, ii>>> &v) {
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
Solution insert_5(Solution &t, Instance &s) {
    return parallel_insert(t, s, [&](vector<vector<pair<ll, ii>>> &v) {
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
Solution regret_k_insert(Solution &t, Instance &s, int k) {
    return parallel_insert(t, s, [&](vector<vector<pair<ll, ii>>> &v) {
        pair<ll, int> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) {
                continue;
            }
            if (v[i].size() < k) {
                return i;
            }

            partial_sort(v[i].begin(), v[i].begin() + k, v[i].end());
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
Solution insert_6(Solution &t, Instance &s) {
    return regret_k_insert(t, s, 2);
}

/*
 * Regret-3 insertion
 */
Solution insert_7(Solution &t, Instance &s) {
    return regret_k_insert(t, s, 3);
}

/*
 * Dynamic profit regret-k insertion
 */
Solution dynamic_profit_regret_k_insert(Solution &t, Instance &s, int k) {
    return parallel_insert(t, s, [&](vector<vector<pair<ll, ii>>> &v) {
        pair<ld, int> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) {
                continue;
            }
            if (v[i].size() < k) {
                return i;
            }

            partial_sort(v[i].begin(), v[i].begin() + k, v[i].end());
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
Solution insert_8(Solution &t, Instance &s) {
    return dynamic_profit_regret_k_insert(t, s, 2);
}

/*
 * Dynamic profit regret-3 insertion
 */
Solution insert_9(Solution &t, Instance &s) {
    return dynamic_profit_regret_k_insert(t, s, 3);
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
tuple<Solution, int, ll> insert(Instance &s, Solution &t, int l = -1) {
    // cerr<<"Local search for the configuration....."<<endl;
    if (t.u.back().empty()) {
        return {t, -1, 0};
    }
    auto start = std::chrono::high_resolution_clock::now();
    Solution z = t;
    if (l == -1) {
        l = priority_select(s.insertion_weights);
    }
    switch(l) {
        case 0:
            z = insert_1(z, s);
            break;
        case 1:
            z = insert_2(z, s);
            break;
        case 2:
            z = insert_3(z, s);
            break;
        case 3:
            z = insert_4(z, s);
            break;
        case 4:
            z = insert_5(z, s);
            break;
        case 5:
            z = insert_6(z, s);
            break;
        case 6:
            z = insert_7(z, s);
            break;
        case 7:
            z = insert_8(z, s);
            break;
        case 8:
            z = insert_9(z, s);
            break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    ll time = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    // cerr<<"Local search for the configuration done"<<endl;
    return {z, l, time};
}
