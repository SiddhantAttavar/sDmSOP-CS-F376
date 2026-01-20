#ifndef SOLUTION_H
#define SOLUTION_H

#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

struct Solution {
	ld temperature;
	vector<vector<ll>> u;
	ll  n, m, r, Tmax, T;
	ll total_profit = 0; 
	vector<vector<ll>> cost, cluster;
    vector<vector<ll>> cities_in_cluster;
	vector<vector<ll>> extended_cost;
	vector<ll> profit;
	vector<ii> proximity;
	vector<vector<ld>> relatedness;
	unordered_map<ll, ll>mp;
	vector<double> removal_weights = {0.19, 0.13, 0.38, 0.2, 0.09};
	vector<double> insertion_weights = {0.29, 0.36, 0.12, 0.07, 0.02, 0.04, 0.04, 0.03, 0.05};
	double avg_removal_time = 1000, avg_insertion_time = 1000;
	string name, comment;
};

Solution input(string file_name);
#endif
