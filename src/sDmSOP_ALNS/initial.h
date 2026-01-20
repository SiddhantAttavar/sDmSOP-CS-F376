#ifndef INITIAL_H
#define INITIAL_H

#include <bits/stdc++.h>
#include "utils.h"
#include "solution.h"
#include "calc.h"
using namespace std;

ld objective_function_algorithm_1(vector<vector<ll>>& u1, vector<vector<ll>>& u2, Solution &s, ll  p_extra);
void construct_alternate_initial_solution(Solution &s);
void construct_initial_solution(Solution &s);
void improve_initial_solution(Solution &s);

#endif
