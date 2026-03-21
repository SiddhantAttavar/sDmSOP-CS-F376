#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
using namespace std;

Solution::Solution(vector<vi> _u, ll n, ll r) :
    u(_u), g(r, INF), h(r, INF), dp(n + 1, INF), rdp(n + 1, INF) {}
