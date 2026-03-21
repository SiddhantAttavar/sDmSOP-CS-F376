#include <bits/stdc++.h>
#include "instance.h"
#include "utils.h"
using namespace std;

/*
    n: number of vertices 
    m: number of salesman
    r: number of sets
    cost: distance between any 2 vertices
    cluster: membership map for vertices to clusters
    profit: profit of each cluster
*/

/*
* Solution vector u is of the form:
* salesman 1: [u11, u12, ..., u1i] -> sequence of clusters to visit in that order for salesman 1 
* salesman 2: [u21, ...] -> sequence of clusters to visit in that order for salesman 2 
* .....
* .....
* salesman m: [um1, ...] -> sequence of clusters to visit in that order for salesman m
* extra set: [...] -> Contains all the remaining clusters
* Size of u = (m+1)
*/

Instance input(string file_name) {
	Instance s;
    ifstream infile(file_name);

    map<string, string> data;
    string line;
    for (int i = 0; i < 10; i++) {
        getline(infile, line);
        int j = find(line.begin(), line.end(), ':') - line.begin();
        data[line.substr(0, j)] = line.substr(j + 2);
    }

    s.name = data["NAME"];
    s.n = stoi(data["DIMENSION"]);
    s.Tmax = stold(data["TMAX"]);
    s.m = stoi(data["VEHICLES"]);
    assert(data.count("SETS"));
    s.r = stoi(data["SETS"]);
    assert(stoi(data["START_SET"]) == 0);
    assert(stoi(data["END_SET"]) == 0);
    s.T = s.Tmax;
    s.Tmax *= s.m;

    getline(infile, line);

    vector<pair<ld, ld>> pos(s.n + 1);
    while (getline(infile, line)) {
        istringstream iss(line);
        int i;
        ld x, y;
        if (!(iss >> i >> x >> y)) {
            break;
        }
        pos[i] = {x, y};
    }

    s.cost.resize(s.n+1,vi(s.n+1, INF));
    s.profit.resize(s.r);
    s.cluster.resize(s.r,vi(s.n+1,0));
    for (int u = 1; u <= s.n; u++) {
        for (int v = 1; v <= s.n; v++) {
            ld x = abs(pos[u].first - pos[v].first);
            ld y = abs(pos[u].second - pos[v].second);
            s.cost[u][v] = (ll) ceil(sqrt(x * x + y * y));
        }
    }

    while (getline(infile, line)) {
        istringstream iss(line);
        int i, x;
        iss >> i >> x;
        s.profit[i] = x;
        int v;
        while (iss >> v) {
            s.cluster[i][v] = true;
        }
    }

	return s;
}
