#include <bits/stdc++.h>
#include "solution.h"
#include "instance.h"
#include "utils.h"
using namespace std;

Solution::Solution(vector<vi> _u, Instance &s) :
    u(_u), g(s.r, INF), h(s.r, INF), dp(s.n + 1, INF), rdp(s.n + 1, INF), valid(sz(u) - 1, false) {
    g[0] = 0;
    h[0] = 0;
    for (int k : s.cities_in_cluster[0]) {
        dp[k] = 0;
        rdp[k] = 0;
    }
}

Solution::Solution() : u(), g(), h(), dp(), rdp(), valid() {}
