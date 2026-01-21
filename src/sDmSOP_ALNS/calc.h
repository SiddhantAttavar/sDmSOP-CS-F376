#ifndef CALC_H
#define CALC_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
using namespace std;

ll  P(vector<vi> &u, vi &profit);
void printProfit(Solution &s);
void printTour(Solution &s);
ii find_pos_in_u(vector<vector<ll>> &u, ll  index);
vector<pair<ll,ll>> find_next_state_of_dp(Solution &s, ll  cluster1, ll  cluster2, vector<pair<ll,ll>> dp_row);
vi  L(vector<vi> &u, Solution &s, bool printCity=false);
void printCost(Solution &s, bool printCity=false);
ll getTourCost(const vi &cost);
bool tourInvalid(vector<vector<ll>> &u, Solution &s);
array<vi, 4> preprocess_costs(vector<vector<ll>> &u, Solution &s, int modified = -1);
bool no_salesmen_empty(vector<vector<ll>>& u);

#endif
