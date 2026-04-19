#pragma once

#include <bits/stdc++.h>
#include "instance.h"
#include "utils.h"
using namespace std;

struct Solution {
    vector<vi> u;
    vi g, h, dp, rdp;
    vector<bool> valid;

    Solution(vector<vi> _u, Instance &s);
    Solution();
};
