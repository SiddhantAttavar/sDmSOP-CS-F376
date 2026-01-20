#ifndef INSERT_H
#define INSERT_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
#include "calc.h"
using namespace std;

pair<vector<ll>, vector<ll>> preprocess_costs(vector<vector<ll>> &u, Solution &s);
vector<pair<ll, ii>> insertion_costs(vector<vector<ll>> &u, Solution &s, int v);
ii find_least_cost_insertion_position(vector<vector<ll>> &u, Solution &s, int v);
vector<vector<ll>> predetermined_insert(vector<vector<ll>> u, Solution &s, int rp);
vector<vector<ll>> insert_1(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> insert_2(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> insert_3(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> parallel_insert(vector<vector<ll>> u, Solution &s, function<int(vector<vector<pair<ll, ii>>>&)> select);
vector<vector<ll>> insert_4(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> insert_5(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> regret_k_insert(vector<vector<ll>> u, Solution &s, int k);
vector<vector<ll>> insert_6(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> insert_7(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> dynamic_profit_regret_k_insert(vector<vector<ll>> &u, Solution &s, int k);
vector<vector<ll>> insert_8(vector<vector<ll>> &u, Solution &s);
vector<vector<ll>> insert_9(vector<vector<ll>> &u, Solution &s);
tuple<vector<vi>, int, ll> insert(vector<vector<ll>>& u, Solution &s, int l = -1);

#endif
