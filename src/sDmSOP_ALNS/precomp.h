#pragma once

#include <bits/stdc++.h>
#include "utils.h"
#include "instance.h"
using namespace std;

void set_cities_in_cluster(Instance &s);
void calculate_proximity(Instance &s);
void calculate_relatedness(Instance &s);
void calculate_extended_cost(Instance &s);
