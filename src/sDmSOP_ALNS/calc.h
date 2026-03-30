#pragma once

#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
#include "instance.h"
using namespace std;

ll  P(vector<vi> &u, vi &profit);
void printProfit(Solution &t, Instance &s);
void printTour(Solution &t);
void printTourFile(Solution &t, ofstream& outfile);
ii find_pos_in_u(vector<vector<ll>> &u, ll  index);
vector<pair<ll,ll>> find_next_state_of_dp(Instance &s, ll  cluster1, ll  cluster2, vector<pair<ll,ll>> dp_row);
vi  L(vector<vi> &u, Instance &s, bool printCity=false);
void printCost(Solution &t, Instance &s, bool printCity=false);
bool tourInvalid(vector<vector<ll>> &u, Instance &s);
void preprocess_costs(Solution &t, Instance &s, int modified = -1);
bool no_salesmen_empty(vector<vector<ll>>& u);
ld objective_function_algorithm_1(vector<vector<ll>>& u1, vector<vector<ll>>& u2, Instance &s, ll  p_extra);
