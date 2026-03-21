#pragma once

#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

struct Solution {
    vector<vi> u;
    ll total_profit = 0;
    vi g, h, dp, rdp;

    Solution(vector<vi> _u, ll n, ll r);
};
