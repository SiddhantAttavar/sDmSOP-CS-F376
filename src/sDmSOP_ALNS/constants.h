#pragma once

#include <vector>
#define ll long long

const int NUM_THREADS = 10;
const ll inf = 0xFFFFFFFFFFFFFFFL; //very large number
const ll INF = 1e18;
const ll MAX_LIMIT_REPEAT = 100;
const ll MAX_TIME = 3600;
const ll MAX_BAD_ITERATIONS = 10000;
const ll MAX_ITERATIONS = 100000;
const ll PRINT_ITERATIONS = 10000;
const double MU = 0.11;
const double ALPHA = 0.99994;
const double PHI_1 = 0.26, PHI_2 = 0.76;
const double REMOVAL_CONSTANT = 0.3;
const double REWARD_BEST = 1.1, MIN_WEIGHT = 0.01, MAX_WEIGHT = 0.5;
const double EMA_ALPHA = 0.01;
const std::vector<double> BASE_REMOVAL_WEIGHTS = {0.2, 0.2, 0.2, 0.2, 0.2};
const std::vector<double> BASE_INSERTION_WEIGHTS = {0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.11, 0.12};
