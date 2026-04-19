#pragma once

#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

struct Instance {
	ld temperature;
	ll  n, m, r, Tmax, T;
    ll total_profit = 0;
	vector<vi> cost, cluster;
    vector<vi> cities_in_cluster;
	vector<vi> extended_cost;
	vi profit;
	vector<ii> proximity;
	vector<vector<ld>> relatedness;
	unordered_map<ll, ll>mp;
	vector<double> removal_weights = BASE_REMOVAL_WEIGHTS;
	vector<double> insertion_weights = BASE_INSERTION_WEIGHTS;
	double avg_removal_time = 1000, avg_insertion_time = 1000;
	string name, comment;
};

Instance input(string file_name);
