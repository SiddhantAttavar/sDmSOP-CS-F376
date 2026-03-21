#pragma once

#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
#include "instance.h"
#include "calc.h"
using namespace std;

Solution construct_alternate_initial_solution(Instance &s);
Solution construct_initial_solution(Instance &s);
void improve_initial_solution(Instance &s, Solution &t);
