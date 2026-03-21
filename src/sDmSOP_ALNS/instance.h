#pragma once

#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

struct Instance {
	ld temperature;
	ll  n, m, r, Tmax, T;
	vector<vi> cost, cluster;
    vector<vi> cities_in_cluster;
	vector<vi> extended_cost;
	vi profit;
	vector<ii> proximity;
	vector<vector<ld>> relatedness;
	unordered_map<ll, ll>mp;
	vector<double> removal_weights = {0.19, 0.13, 0.38, 0.2, 0.09};
	vector<double> insertion_weights = {0.29, 0.36, 0.12, 0.07, 0.02, 0.04, 0.04, 0.03, 0.05};
	double avg_removal_time = 1000, avg_insertion_time = 1000;
	string name, comment;
};

Instance input(string file_name);
