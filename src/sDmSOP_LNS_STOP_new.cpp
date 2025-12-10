// http://e-maxx.ru/algo/assignment_hungary#6 - For Hungarian Algorithm
// TODO: Can replace rand()%r with getRand(0, r-1) which might be a better random generator 

#include <bits/stdc++.h>  // to include every std. lib.
using namespace std;

#define ll  long long
#define ff first
#define ss second
#define endl "\n"
#define all(x) (x).begin(), (x).end()
#define sz(x) (ll)(x.size())
ll  powmod(ll  a,ll  l, ll  md){a%=md; ll  res=1;while(l){if(l&1)res=res*a%md;l/=2;a=a*a%md;}return res;}
ll  binpow(ll  a,ll  l){ll  res=1;while(l){if(l&1)res=res*a;l/=2;a=a*a;}return res;}
ll  invmod(ll  a, ll  md){return powmod(a,md-2,md);}
typedef unsigned long long ull; typedef long double ld;
typedef vector<ll> vi; typedef pair<ll, ll> ii; typedef vector< ii > vii;
#define pb push_back
ll  __set(ll  b, ll  i) {return b|(1LL<<i);} //set ith bit
ll  __unset(ll  b, ll  i) {return b&(~(1UL<<i));}
ll  __check(ll  b, ll  i) {return b&(1LL<<i);} //returns 0 if ith bit is 0
ll  mulmod(ll  a, ll  b, ll  md) {return (((a%md)*(b%md))%md+md)%md;}
ll  addmod(ll  a, ll  b, ll  md) {return ((a%md+b%md)%md+md)%md;}
ll  submod(ll  a, ll  b, ll  md) {return (((a%md-b%md)%md)+md)%md;}
ll  divmod(ll  a, ll  b, ll  md) {return mulmod(a, powmod(b, md-2, md), md);} //if md is prime;
const ll inf = 0xFFFFFFFFFFFFFFFL; //very large number
const ll INF = 1e18;
const ll MAX_LIMIT_REPEAT = 100;
const ll MAX_TIME = 1800;
const ll MAX_BAD_ITERATIONS = 10000;
const ll MAX_ITERATIONS = 100000;
const double MU = 0.11;
const double ALPHA = 0.99994;
const double PHI_1 = 0.24, PHI_2 = 0.76;
const double REMOVAL_CONSTANT = 0.3;
const vector<double> REMOVAL_WEIGHTS = {0.19, 0.13, 0.38, 0.2, 0.09};
const vector<double> INSERTION_WEIGHTS = {0.29, 0.36, 0.12, 0.07, 0.02, 0.04, 0.04, 0.03, 0.05};
// const vector<double> INSERTION_WEIGHTS = {0.29, 0.36, 0.12};

ofstream outfile; 


priority_queue<ll, vector<ll>, greater<ll> > pq; //for min priority_queue
#define IOS ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);


mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//GIVES A RANDOM NUMBER BETWEEN [l,r] BOTH INCLUSIVE.
ll  getRand(ll  l, ll  r){
    if (l > r) {
        return r;
    }
    uniform_int_distribution<ll> uid(l, r);
    return uid(rng);
}
/*
    n: number of vertices 
    m: number of salesman
    r: number of sets
    cost: distance between any 2 vertices
    cluster: membership map for vertices to clusters
    profit: profit of each cluster
*/
ld temperature;
ll  n, m, r;
ll total_profit = 0; 
vector<vector<ll>> cost, cluster;
vector<vector<ll>> extended_cost;
vector<ll> profit;
vector<ii> proximity;
vector<vector<ld>> relatedness;
unordered_map<ll, ll>mp;

string name, comment;
ll  Tmax;

void input(string file_name) {
    ifstream infile(file_name);

    map<string, string> data;
    string line;
    for (int i = 0; i < 10; i++) {
        getline(infile, line);
        int j = find(line.begin(), line.end(), ':') - line.begin();
        data[line.substr(0, j)] = line.substr(j + 2);
    }

    name = data["NAME"];
    n = stoi(data["DIMENSION"]);
    Tmax = stold(data["TMAX"]);
    m = stoi(data["VEHICLES"]);
    assert(data.count("SETS"));
    r = stoi(data["SETS"]);
    assert(stoi(data["START_SET"]) == 0);
    assert(stoi(data["END_SET"]) == 0);
    Tmax *= m;

    getline(infile, line);

    vector<pair<ld, ld>> pos(n + 1);
    while (getline(infile, line)) {
        istringstream s(line);
        int i;
        ld x, y;
        if (!(s >> i >> x >> y)) {
            break;
        }
        pos[i] = {x, y};
    }

    cost.resize(n+1,vector<ll>(n+1, INF));
    profit.resize(r);
    cluster.resize(r,vector<ll>(n+1,0));
    for (int u = 1; u <= n; u++) {
        for (int v = 1; v <= n; v++) {
            ld x = abs(pos[u].first - pos[v].first);
            ld y = abs(pos[u].second - pos[v].second);
            cost[u][v] = (ll) ceil(sqrt(x * x + y * y));
        }
    }

    while (getline(infile, line)) {
        istringstream s(line);
        int i, x;
        s >> i >> x;
        profit[i] = x;
        int v;
        while (s >> v) {
            cluster[i][v] = true;
        }
    }
}



// ALGORITHM
//
// u = construct_initial_solution();
// while(time constraint  not reached) {
//     l = 1;
//     while l <= l_max do {
//         u' = shake(u, l);
//         u'' = local_search(u', l);
//         if(L(u'') <= Tmax and P(u'') > P(u)) then {
//             u = u'';
//             l = 1;
//         } else {
//             l = l + 1;
//         }
//     }
// }


vector<vector<ll>> cities_in_cluster;
// unordered_map<int,int>mp;
// Number of neighborhood structures
ll  Lmax = 3;

// Limit on time. Hard constraint
//-----------------------------------DEFINE THE PARAMETERS OF THE PROBLEM END------------------------------------------------



/*
* Set cities that are in each cluster
*/
void set_cities_in_cluster() {
    // cerr<<"Setting cities in each cluster"<<endl;
    cities_in_cluster.resize(r);
    for(ll  i=0; i<r; i++) {
        for(ll  j=0; j<=n; j++) {
            if(cluster[i][j])
                cities_in_cluster[i].pb(j);
        }
    }
    
    // cerr<<"Setting cities in each cluster done"<<endl;
}


/*
 * Calculate proximity for each cluster
 */
void calculate_proximity() {
    proximity.resize(r, {0, 0});
    for (int i = 0; i < r; i++) {
        for (int u : cities_in_cluster[i]) {
            for (int v : cities_in_cluster[i]) {
                proximity[i].first += cost[u][v];
            }
        }
        proximity[i].second = ((ll) cities_in_cluster[i].size()) * cities_in_cluster[i].size();
    }
}

void calculate_relatedness() {
    relatedness.resize(r);
    for (int i = 0; i < r; i++) {
        relatedness[i].resize(r, 0);
        for (int j = i + 1; j < r; j++) {
            ll cost_max = 0;
            for (int u : cities_in_cluster[i]) {
                for (int v : cities_in_cluster[j]) {
                    relatedness[i][j] += cost[u][v];
                    cost_max = max(cost_max, cost[u][v]);
                }
            }
            relatedness[i][j] /= sz(cities_in_cluster[i]) * sz(cities_in_cluster[j]);
            relatedness[i][j] /= cost_max;
        }
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < i; j++) {
            relatedness[i][j] = relatedness[j][i];
        }
    }
}

void calculate_extended_cost() {
    extended_cost.resize(n + r + 1, vector<ll>(n + r + 1, INF));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            extended_cost[i + r][j + r] = cost[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < r; j++) {
            for (int k : cities_in_cluster[j]) {
                extended_cost[i + r][j] = min(extended_cost[i + r][j], cost[i][k]);
                extended_cost[j][i + r] = min(extended_cost[j][i + r], cost[k][i]);
            }
        }
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < r; j++) {
            for (int k : cities_in_cluster[j]) {
                extended_cost[i][j] = min(extended_cost[i][j], extended_cost[i][k + r]);
            }
        }
    }
}


/*
* Find the profit from visited clusters
*/
ll  P(vector<vector<ll>>& u) {
    // cerr<<"Calculating profit for the current configuration....."<<endl;
    ll  p = 0;
    for(ll  i = 0; i < sz(u)-1; i++) {
        for(ll  cluster: u[i])
            p += profit[cluster];
    }
    // cerr<<"Calculation of profit for the current configuration done"<<endl;
    return p;
}



/*
* Prll  profit obtained by the final sequence
*/
void printProfit(vector<vector<ll>>& u) {
    // cerr<<"Printing max profit"<<endl;
    cout<<"Max profit is: "<<P(u)<<endl;
    // cerr<<"Printing max profit done"<<endl;
}



/*
* Prll  list of cities (in order) visited by each salesman
*/
void printTour(vector<vector<ll>>& u) {
    // cerr<<"Printing Tour"<<endl;
    cout<<"Set of visited vertices is: "<<endl;
    for(ll  i = 0; i < sz(u)-1; i++) {
        cout<<"sequence of sets for salesman "<<i+1<<": { ";
        // vector<ll> salesmanSet = ;
        for(ll  set: u[i]) {
            cout<<set<<" ";
        }
        cout<<"}"<<endl;
    }
    cout<<"sets not visisted by any salesmen are: ";
    for(ll  i: u[sz(u)-1])
        cout<<i<<" ";
    cout<<endl;
    cerr<<"Printing Tour done"<<endl;
}



//correlates an index position to a postion among the multiple set of travellers
ii find_pos_in_u(vector<vector<ll>>& u, ll  index) {
    // cerr<<"Finding position of "<<index<<" in u"<<endl;
    // printTour(u);
    // cout<<"index = "<<index<<endl;
    index++;
    ll  cur = 0;
    ii response = {0,0};
    for(ll  i = 0; i < sz(u); i++) {
        if(cur+sz(u[i]) >= index) {
            response = {i, index-cur-1};
            break;
        } else {
            cur += sz(u[i]);
        }

        // cout << i << " " << cur << endl;
    }
    // cerr<<"Finding position of "<<index-1<<" in u done"<<endl;
    // cout<<"response = "<<response.ff<<" "<<response.ss<<endl;
    return response;
}



/*
* Finds the next state of dp given the start and end clusters and the previous dp values
*/
vector<pair<ll,ll>> find_next_state_of_dp(ll  cluster1, ll  cluster2, vector<pair<ll,ll>> dp_row) {
    // cerr<<"Finding the next dp state for movement from "<<cluster1<<" to "<<cluster2<<"....."<<endl;

    vector<pair<ll,ll>> dp_cur(sz(cities_in_cluster[cluster2]), {INF,0});
    for(ll  id1 = 0; id1 < sz(cities_in_cluster[cluster1]); id1++) {
        for(ll  id2 = 0; id2 < sz(cities_in_cluster[cluster2]); id2++) {
            // dp_cur[id2] = min(dp_cur[id2], dp_row[id1]+d[cities_in_cluster[cluster1][id1]][cities_in_cluster[cluster2][id2]]);
            if(dp_cur[id2].ff > dp_row[id1].ff + cost[cities_in_cluster[cluster1][id1]][cities_in_cluster[cluster2][id2]]){
                dp_cur[id2].ff = dp_row[id1].ff + cost[cities_in_cluster[cluster1][id1]][cities_in_cluster[cluster2][id2]];
                dp_cur[id2].ss = id1;
            }

            // cout << id1 << " " << id2 << " " << dp_cur[id2].ff << endl;
        }
    }
    // cout <<"Solutions found for next dp state for movement from "<<cluster1<<" to "<<cluster2<<endl;
    return dp_cur;
}



/*
* Finds the minimum cost for the given sequence of clusters over all salesmen
*/

// Add depot cost here
vi  L(vector<vector<ll>>& u, bool printCity=false) {
    // cerr<<"Calculating cost for the current configuration....."<<endl;
    // printTour(u);
    vi  cost(m,0);
    
    for(ll  i = 0; i < sz(u)-1; i++) {
        // cout<<"i = "<<i<<endl;
        vector<vector<pair<ll,ll>>> dp(sz(u[i])+2); // Minimum cost of reaching each vertex of each cluster
        //first stores min cost, second stores prev city
        dp[0].pb({0,1});
        ll  cluster1 = 0;
        for(ll  j = 0; j < sz(u[i]); j++) {
            ll  cluster2 = u[i][j];
            // cout << "prev cluster: " << cluster1 <<" next cluster: " << cluster2 << endl;
            dp[j+1] = find_next_state_of_dp(cluster1, cluster2, dp[j]);
            cluster1 = u[i][j];
        }
        ll  cluster2 = 0;

        dp[sz(u[i])+1] =  find_next_state_of_dp(cluster1, cluster2, dp[sz(u[i])]);
        cost[i] += dp[sz(u[i])+1][0].ff;
        // cout << ", Move Cost: " << dp[sz(u[i])+1][0].ff << ", New Cost: " << cost << endl;

        if(printCity){
            string temp = "";
            ll  k = sz(u[i]), j;

            if(k != 0){
                j = dp[k+1][0].ss;
                cout<<"last city visited by salesman "<<i+1<<": "<<cities_in_cluster[u[i][k-1]][j]<<"\n";
            }

            while(k>0){
                temp = to_string(cities_in_cluster[u[i][k-1]][j])+" "+temp;
                k--;
                j = dp[k+1][j].ss;
            }
            cout<<"Path of cities for salesman "<<i+1<<": "<<temp<<"\n";
        }
    }
    // cout <<"Calculation of cost for the current configuration done: " << cost << endl;
    return cost;
}



/*
* Prints the cost of given configuration
*/
void printCost(vector<vector<ll>>& u, bool printCity=false) {
    // cerr<<"Printing Cost of tour"<<endl;
    vi cost = L(u, printCity);
    ll tot = 0;
    for(int i=0; i<sz(cost); i++){
        cout<<"Cost for traveller "<< i+1 << " is " << cost[i] <<endl;
        tot += cost[i];
    }

    cout << "Cost of tour is: " << tot << endl; 
    // cerr<<"Printing Cost of tour done"<<endl;
}

ll getTourCost(vi cost){

    ll tot = 0;
    for(ll c: cost){
        tot += c;
    }

    return tot;
}


bool tourInvalid(vector<vector<ll>>& u){
    vi tourCosts = L(u); 
    int flag; 

    if(getTourCost(tourCosts) > Tmax) return true;
    for(int c: tourCosts){
        if(c > Tmax/m) return true;
    }

    return false;
}



/*
* Objective function for algorithm 1 of constructing solutions
*/
ld objective_function_algorithm_1(vector<vector<ll>>& u1, vector<vector<ll>>& u2, ll  p_extra) {
    // printTour(u1);
    return (getTourCost(L(u1))-getTourCost(L(u2)))/(ld)p_extra;
}


/*
* Hungarian algorithm based method to find alternate initial solutions. This is used in case when
* the other approach doesn't give a solution in which all the visit atleast 1 set.
* Hungarian algortihm: a is n x m adjacency matrix. in our case it would be m x r. 
*/
vector<vector<ll>> construct_alternate_initial_solution() {
    // cerr<<"Constructing alternate initial solutions....."<<endl;
    
    vector<ll> weight_row(r, INF);
    for(ll  j = 1; j < r; j++) {
        // cout << "Cluster: " << j <<endl;
        for(ll  city: cities_in_cluster[j]){
            weight_row[j] = min(weight_row[j], 2*cost[1][city]);
            // cout << "City: " << city << " Weights: " << weight_row[j] << endl;
        }
        // cout << endl;
    }

    // for(ll  j = 1; j < r; j++)
    //     cout<<weight_row[j]<<" ";
    // cout<<endl;

    vector<ll> u (m+1), v (r), matching (r), way (r);
    for (ll  i=1; i<=m; i++) {
        matching[0] = i;
        ll  j0 = 0;
        vector<ll> minv (r, INF);
        vector<char> used (r, false);
        do {
            used[j0] = true;
            ll  i0 = matching[j0],  delta = INF,  j1;
            for (ll  j=1; j < r; j++){
                // cout << "i0 = " << i0 << " j = " << j << endl;
                if (!used[j]) {
                    ll  cur = weight_row[j]-u[i0]-v[j];
                    // cout << "Cur: " << cur << " minv = " << minv[j] << endl;
                    if (cur < minv[j])
                        minv[j] = cur,  way[j] = j0;
                    if (minv[j] < delta)
                        delta = minv[j],  j1 = j;
                }
            }
            for (ll  j = 1; j < r; j++)
                if (used[j])
                    u[matching[j]] += delta,  v[j] -= delta;
                else
                    minv[j] -= delta;
            j0 = j1;
            // cout << "j1 = " << j1 << " matching[j0] = " << matching[j0] << endl;
        } while (matching[j0] != 0);
        do {
            ll  j1 = way[j0];
            matching[j0] = matching[j1];
            j0 = j1;
        } while (j0);
    }
    // cerr<<"Initial solutions constructed"<<endl;
    vector<vector<ll>> ans (m+1);
    vector<ll> seen(r, 0);
    for (ll  j=1; j <r; j++) {
        // cout << matching[j] << endl;
        if(matching[j] != 0) {
            // cout<<"pairing "<<matching[j]<<" with "<<j<<endl;
            ans[matching[j]-1].pb(j);
            seen[j]=1;
        }
    }

    // cout << "Seen: ";
    for(ll  i = 1; i < r; i++) {
        // cout << " " << seen[i] << " ";
        if(!seen[i])
            ans[m].pb(i);
    }
    // cout << endl;
    // printProfit(ans);
    // printCost(ans);
    // printTour(ans); 

    // cout << "Alternate Initial Solutions Constructed" << endl;
    return ans;
}



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

/*
* Construct the initial solution.
* Algorithm 1: The algorithm discussed in paper by Robert Penicka, Et. al.
* Iterate over clusters. For each cluster find a position for it in the sequence for each salesman.
* It basically means iterating over all n possibilities for the position. Then choose the position that
* gives the min value of (L(Sigma_{k+1}) - L(Sigma_{k}))/p_i where p_i is profit from the newly added
* cluster, without violating the Tmax condition.
*
* Algorithm 2:
* Iterate over clusters. For each cluster find a position for it in the sequence for each salesman.
* It basically means iterating over all n possibilities for the position. Then choose the position that
* gives the max value of total profit without violating the Tmax condition.
*/
vector<vector<ll>> construct_initial_solution() {
    // cerr<<"Constructing initial solutions....."<<endl;
    vector<vector<ll>> u(m+1);
    vector<ll> seen(r, 0);
    for(ll  i=1; i<r; i++) {
        // cout<<"i = "<<i<<endl;
        ll  id1=m, id2=-1;
        ld mn_val = INF;
        for(ll  j=0; j<m; j++) {
            // cout<<"j = "<<j<< ", r = "<<r<<", m = "<<m<<endl;

            ll  zero_count = 0; // Number of travellers with no path assigned
            for(ll  i = 0; i < m; i++) {
                if(sz(u[i])==0){
                    zero_count++;
                    // cout << zero_count << " ";
                }
            }
            // cout << endl;
            // cout<<"check 1 "<<endl;

            //what does this mean?, may not need for this problem, check introduced for mDmSOP
            if((sz(u[j]) != 0 && zero_count >= r) || zero_count > r) {
                // cout<<i<<" "<<j<<endl;
                continue;
            }
            // cout<<"check 2: " << sz(u[j]) <<endl;

            for(ll  pos = 0; pos < sz(u[j]); pos++) {
                // cout<<"pos = "<<pos<<endl;
                vector<vector<ll>> u1 = u;
                u1[j].insert(u1[j].begin()+pos, i);
                ld contest_mn_val = objective_function_algorithm_1(u1, u, profit[i]);
                
                // cout<<contest_mn_val<<" "<<mn_val<<" "<<L(u1)<<endl;
    
                if(tourInvalid(u1)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
                if(contest_mn_val < mn_val) {
                    mn_val = contest_mn_val;
                    id1 = j, id2 = pos;
                }
            }

            vector<vector<ll>> u1 = u;
            u1[j].pb(i);
            // cout<<"check 3: " << profit[i] << endl;
            ld contest_mn_val = objective_function_algorithm_1(u1, u, profit[i]);
            // cout<<"check 4"<<endl;

            if(tourInvalid(u1)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
            if(contest_mn_val < mn_val) {
                mn_val = contest_mn_val;
                id1 = j, id2 = INF;
            }
            // cout<<"check 5"<<endl;
        }
        if(id2 != -1) {
            seen[i]=1;
            if(id2 == INF) {
                u[id1].pb(i);
            } 
            
            else {
                // ll  tp = u[id1][id2];
                // u[id1][id2] = i;
                // u[id1].insert(u[id1].begin()+id2, tp);
                u[id1].insert(u[id1].begin()+id2, i);
            }
        }
    }

    for(ll  i = 0; i < r; i++) {
        if(!seen[i])
            u[m].pb(i);
    }
    // cerr<<"Initial solutions constructed"<<endl;
    // printProfit(u);
    // printCost(u);
    // printTour(u);
    return u;
}



vector<vector<ll>> improve_initial_solution(vector<vector<ll>>& u) {
    // cerr<<"Improving initial solutions....."<<endl;
    vector<ll> seen(r, 0);
    for(ll  i = 0; i < m; i++)
        for(ll  cluster: u[i])
            seen[cluster] = 1;
    u[m].clear();
    for(ll  i = 1; i < r; i++) {
        if(seen[i])
            continue;
        ll  id1=m, id2=-1;
        ld mn_val = INF;
        for(ll  j = 0; j < m; j++) {
            for(ll  pos = 0; pos < sz(u[j]); pos++) {
                vector<vector<ll>> u1 = u;
                u1[j].insert(u1[j].begin()+pos, i);
                ld contest_mn_val = objective_function_algorithm_1(u1, u, profit[i]);
                if(tourInvalid(u1)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
                //CHECK THIS PART...............................................................................................................................................
                if(contest_mn_val < mn_val) {
                    mn_val = contest_mn_val;
                    id1 = j, id2 = pos;
                }
            }
            vector<vector<ll>> u1 = u;
            u1[j].pb(i);
            ld contest_mn_val = objective_function_algorithm_1(u1, u, profit[i]);
            if(tourInvalid(u1)) continue; //CAN COMMENT THIS LINE AND CHECK HOW THE ALGORITHM WORKS
            //CHECK THIS PART...............................................................................................................................................
            if(contest_mn_val < mn_val) {
                mn_val = contest_mn_val;
                id1 = j, id2 = INF;
            }
        }
        if(id2 != -1) {
            seen[i]=1;
            if(id2 == INF) {
                u[id1].pb(i);
            } else {
                u[id1].insert(u[id1].begin()+id2, i);
            }
        }

    }
    for(ll  i = 1; i < r; i++) {
        if(!seen[i])
            u[m].pb(i);
    }
    // cerr<<"Initial solutions Improved"<<endl;
    // printProfit(u);
    // printCost(u);
    // printTour(u);
    return u;
}


/*
 * Calculate cost of single salesman
 */
ll cost_of_path(vector<ll> &l) {
    vector<vector<pair<ll,ll>>> dp(sz(l)+2); // Minimum cost of reaching each vertex of each cluster
    //first stores min cost, second stores prev city
    dp[0].pb({0,1});
    ll  cluster1 = 0;
    for(ll  j = 0; j < sz(l); j++) {
        ll  cluster2 = l[j];
        // cout << "prev cluster: " << cluster1 <<" next cluster: " << cluster2 << endl;
        dp[j+1] = find_next_state_of_dp(cluster1, cluster2, dp[j]);
        cluster1 = l[j];
    }
    ll  cluster2 = 0;

    dp[sz(l)+1] =  find_next_state_of_dp(cluster1, cluster2, dp[sz(l)]);
    return dp[sz(l)+1][0].ff;
}

/*
 * Random power select from an ordered list
 */
template<typename T>
T rp_select(vector<T> &l, int rp) {
    double y = ((double) rand() / (RAND_MAX));
    return l[(int) (pow(y, rp) * l.size())];
}

int priority_select(const vector<double> &weights) {
    double y = ((double) rand() / (RAND_MAX));
    for (int i = 0; i < sz(weights); i++) {
        if (y < weights[i]) {
            return i;
        }
        y -= weights[i];
    }
    return sz(weights) - 1;
}

/*
 * Random remove
 */
vector<vector<ll>> remove_1(vector<vector<ll>> u) {
    int i = rand() % (r - sz(u[m]) - 1);
    ii p = find_pos_in_u(u, i);
    u[m].push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
 * Worst cost removal
 */
vector<vector<ll>> remove_2(vector<vector<ll>> u) {
    vector<pair<ll, ii>> cost;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll x = cost_of_path(u[i]);
        for (int j = 0; j < sz(u[i]); j++) {
            vector<ll> l = u[i];
            l.erase(l.begin() + j);
            cost.push_back({cost_of_path(l) - x, {i, j}});
        }
    }

    sort(cost.begin(), cost.end());
    ii p = rp_select(cost, 5).second;

    u[m].push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
 * Worst profit removal
 */
vector<vector<ll>> remove_3(vector<vector<ll>> u) {
    vector<pair<ll, ii>> cost;
    for (ll i = 0; i < sz(u) - 1; i++) {
        for (ll j = 0; j < sz(u[i]); j++) {
            cost.push_back({profit[u[i][j]], make_pair(i, j)});
        }
    }

    sort(cost.begin(), cost.end());
    ii p = rp_select(cost, 10).second;

    u[m].push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
 * Worst profit over cost removal
 */
vector<vector<ll>> remove_4(vector<vector<ll>> u) {
    vector<pair<ii, ii>> cost;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll x = cost_of_path(u[i]);
        for (int j = 0; j < sz(u[i]); j++) {
            vector<ll> l = u[i];
            l.erase(l.begin() + j);
            cost.push_back({
                make_pair(profit[u[i][j]], cost_of_path(l) - x),
                make_pair(i, j)
            });
        }
    }

    sort(cost.begin(), cost.end(), [](pair<ii, ii> a, pair<ii, ii> b) {
        return a.first.first * b.first.second < b.first.first * b.first.second;
    });
    ii p = rp_select(cost, 4).second;

    u[m].push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

vector<vector<ll>> remove_5(vector<vector<ll>> u) {
    vector<pair<ll, ii>> l;
    for (int i = 0; i < sz(u) - 1; i++) {
        for (int j = 0; j < sz(u[i]); j++) {
            l.push_back({u[i][j], {i, j}});
        }
    }
    vector<pair<ld, pair<ii, ii>>> cost;
    for (int i = 0; i < sz(l); i++) {
        for (int j = i + 1; j < sz(l); j++) {
            ld x = 1;
            if (l[i].second.first == l[j].second.second) {
                x = ((ld) abs(l[i].second.second - l[j].second.second)) / sz(u[l[i].second.first]);
            }
            cost.push_back({
                PHI_1 * relatedness[i][j] + PHI_2 * x,
                {l[i].second, l[j].second}
            });
        }
    }
    sort(cost.begin(), cost.end());
    if (cost.empty()) {
        return u;
    }

    auto [p, q] = rp_select(cost, 10).second;
    tie(p, q) = minmax(p, q);
    u[m].push_back(u[q.first][q.second]);
    u[q.first].erase(u[q.first].begin() + q.second);
    u[m].push_back(u[p.first][p.second]);
    u[p.first].erase(u[p.first].begin() + p.second);
    return u;
}

/*
* Remove procedure. Contains two different neighborhood structures:
* 1. Neighborhood 1 (Path move):
*                       Select a path i.e. a subarray and index and move the
*                       subpart to that position. While selecting path we ensure
*                       that the whole path is taken from the same salesman.
* 2. Neighborhood 2 (Path exchange):  
*                       Select two paths and exchange them. Again while 
*                       paths we select each path from one salesman. 
*
* In all cases we keep the new values only if they satisfy all the hard constraints.
* We can add more neighborhoods structures later, if needed.
*/
vector<vector<ll>> remove(vector<vector<ll>> u) {
    // cerr<<"Shaking the configuration....."<<endl;
    int k = r - sz(u[m]) - 1;
    if (k == 0) {
        return u;
    }
    int x = getRand(4, min(40, (int) (REMOVAL_CONSTANT * k)));
    while (x--) {
        int l = priority_select(REMOVAL_WEIGHTS);
        vector<vector<ll>> u1 = u;
        switch(l) {
            case 0:
                u1 = remove_1(u);
                break;
            case 1:
                u1 = remove_2(u);
                break;
            case 2:
                u1 = remove_3(u);
                break;
            case 3:
                u1 = remove_4(u);
                break;
            case 4:
                u1 = remove_5(u);
                break;
        }
        // cerr<<"Shaking the configuration done"<<endl;
        u = u1;
    }
    return u;
}

pair<vector<ll>, vector<ll>> preprocess_costs(vector<vector<ll>> &u) {
    vector<ll> dp(n + 1, INF), rdp(n + 1, INF);
    for (int k : cities_in_cluster[0]) {
        dp[k] = 0;
        rdp[k] = 0;
    }
    for (int i = 0; i < sz(u); i++) {
        if (u[i].empty()) {
            continue;
        }
        for (int k : cities_in_cluster[u[i][0]]) {
            for (int v : cities_in_cluster[0]) {
                dp[k] = min(dp[k], dp[v] + cost[v][k]);
            }
        }
        for (int j = 1; j < sz(u[i]); j++) {
            for (int k : cities_in_cluster[u[i][j]]) {
                for (int v : cities_in_cluster[u[i][j - 1]]) {
                    dp[k] = min(dp[k], dp[v] + cost[v][k]);
                }
            }
        }
        for (int k : cities_in_cluster[u[i].back()]) {
            for (int v : cities_in_cluster[0]) {
                rdp[k] = min(rdp[k], rdp[v] + cost[k][v]);
            }
        }
        for (int j = sz(u[i]) - 2; j >= 0; j--) {
            for (int k : cities_in_cluster[u[i][j]]) {
                for (int v : cities_in_cluster[u[i][j + 1]]) {
                    rdp[k] = min(rdp[k], rdp[v] + cost[k][v]);
                }
            }
        }
    }
    vector<ll> g(r, INF), h(r, INF);
    for (int i = 0; i < r; i++) {
        for (int j : cities_in_cluster[i]) {
            g[i] = min(g[i], dp[j]);
            h[i] = min(h[i], rdp[j]);
        }
    }
    return {g, h};
}

vector<pair<ll, ii>> insertion_costs(vector<vector<ll>> &u, int v) {
    auto [g, h] = preprocess_costs(u);
    vector<pair<ll, ii>> res;
    for (int i = 0; i < sz(u) - 1; i++) {
        ll x = cost_of_path(u[i]);
        for (int j = 0; j <= sz(u[i]); j++) {
            int prev = j > 0 ? u[i][j - 1] : 0, next = j < u[i].size() ? u[i][j] : 0;
            ll bound1 = g[prev] + extended_cost[prev][v] + extended_cost[v][next] + h[next];
            if (bound1 > Tmax / m) {
                continue;
            }
            ll z = g[prev] + h[next];
            ll bound2 = INF;
            for (int k : cities_in_cluster[v]) {
                bound2 = min(bound2, z + extended_cost[prev][k + r] + extended_cost[k + r][next]);
            }
            if (bound2 > Tmax / m) {
                continue;
            }

            vector<ll> l = u[i];
            l.insert(l.begin() + j, v);
            ll y = cost_of_path(l);
            if (y <= Tmax / m) {
                res.push_back({y - x, {i, j}});
            }
        }
    }
    return res;
}

ii find_least_cost_insertion_position(vector<vector<ll>> &u, int v) {
    vector<pair<ll, ii>> l = insertion_costs(u, v);
    if (l.empty()) {
        return {-1, -1};
    }
    return min_element(l.begin(), l.end())->second;
}

vector<vector<ll>> predetermined_insert(vector<vector<ll>> u, int rp) {
    vector<ll> v;
    while (!u[m].empty()) {
        int i = rp_select(u[m], rp);
        u[m].erase(find(u[m].begin(), u[m].end(), i));
        ii p = find_least_cost_insertion_position(u, i);
        if (p.first == -1) {
            v.push_back(i);
            continue;
        }
        assert(i < r);
        u[p.first].insert(u[p.first].begin() + p.second, i);
    }
    u[m] = v;
    return u;
}

/*
 * Random insertion
 */
vector<vector<ll>> insert_1(vector<vector<ll>> u) {
    shuffle(u[m].begin(), u[m].end(), rng);
    return predetermined_insert(u, 5);
}

/*
 * Profit based insertion
 */
vector<vector<ll>> insert_2(vector<vector<ll>> u) {
    sort(u[m].begin(), u[m].end(), [&](int a, int b) {
        return profit[a] > profit[b];
    });
    return predetermined_insert(u, 4);
}

/*
 * Proximity based insertion
 */
vector<vector<ll>> insert_3(vector<vector<ll>> u) {
    sort(u[m].begin(), u[m].end(), [&](int a, int b) {
        return proximity[a].first * proximity[b].second > proximity[b].first * proximity[a].second;
    });
    return predetermined_insert(u, 6);
}

vector<vector<ll>> parallel_insert(vector<vector<ll>> u, function<int(vector<vector<pair<ll, ii>>>)> select) {
    vector<ll> left;
    while (!u[m].empty()) {
        vector<vector<pair<ll, ii>>> costs;
        vector<ll> v;
        vector<ll> temp;
        for (int i : u[m]) {
            vector<pair<ll, ii>> cost = insertion_costs(u, i);
            if (!cost.empty()) {
                costs.push_back(cost);
                temp.push_back(i);
            }
            else {
                v.push_back(i);
                left.push_back(i);
            }
        }
        u[m] = temp;
        int j = select(costs);
        if (j != -1) {
            ii p = min_element(costs[j].begin(), costs[j].end())->second;
            u[p.first].insert(u[p.first].begin() + p.second, u[m][j]);
            u[m].erase(u[m].begin() + j);
        }
    }
    u[m] = left;
    return u;
}

/*
 * Cheapest insertion
 */
vector<vector<ll>> insert_4(vector<vector<ll>> u) {
    return parallel_insert(u, [](vector<vector<pair<ll, ii>>> v) {
        ii res = {INF, -1};
        for (int i = 0; i < v.size(); i++) {
            if (!v[i].empty()) {
                res = min(res, {min_element(v[i].begin(), v[i].end())->first, i});
            }
        }
        return res.second;
    });
}

/*
 * Dynamic profit over cost insertion
 */
vector<vector<ll>> insert_5(vector<vector<ll>> u) {
    return parallel_insert(u, [](vector<vector<pair<ll, ii>>> v) {
        pair<ld, ll> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (!v[i].empty()) {
                res = max(res, {((ld) profit[i]) / min_element(v[i].begin(), v[i].end())->first, i});
            }
        }
        return res.second;
    });
}

/*
 * Regret-k insertion
 */
vector<vector<ll>> regret_k_insert(vector<vector<ll>> u, int k) {
    return parallel_insert(u, [&](vector<vector<pair<ll, ii>>> v) {
        pair<ll, int> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) {
                continue;
            }
            if (v[i].size() < k) {
                return i;
            }
            sort(v[i].begin(), v[i].end());
            ll c = 0;
            for (int j = 1; j < k; j++) {
                c += v[i][j].first - v[i][0].first;
            }
            res = max(res, {c, i});
        }
        return res.second;
    });
}

/*
 * Regret-2 insertion
 */
vector<vector<ll>> insert_6(vector<vector<ll>> u) {
    return regret_k_insert(u, 2);
}

/*
 * Regret-3 insertion
 */
vector<vector<ll>> insert_7(vector<vector<ll>> u) {
    return regret_k_insert(u, 3);
}

/*
 * Dynamic profit regret-k insertion
 */
vector<vector<ll>> dynamic_profit_regret_k_insert(vector<vector<ll>> u, int k) {
    return parallel_insert(u, [&](vector<vector<pair<ll, ii>>> v) {
        pair<ld, int> res = {0, -1};
        for (int i = 0; i < v.size(); i++) {
            if (v[i].empty()) {
                continue;
            }
            if (v[i].size() < k) {
                return i;
            }
            sort(v[i].begin(), v[i].end());
            ll c = 0;
            for (int j = 1; j < k; j++) {
                c += v[i][j].first - v[i][0].first;
            }
            res = max(res, {((ld) profit[i]) / c, i});
        }
        return res.second;
    });
}

/*
 * Dynamic profit regret-2 insertion
 */
vector<vector<ll>> insert_8(vector<vector<ll>> u) {
    return dynamic_profit_regret_k_insert(u, 2);
}

/*
 * Dynamic profit regret-3 insertion
 */
vector<vector<ll>> insert_9(vector<vector<ll>> u) {
    return dynamic_profit_regret_k_insert(u, 3);
}

/*
* Local Search. Contains two different neighborhood structures:
* 1. Neighborhood 1 (One cluster move):
*                       Select two random positions i1, i2. Now we can put:
*                       1. i1 after i2 
*                       or 
*                       2. i2 before i1.
* 2. Neighborhood 2 (One cluster exchange):  
*                       Select two random clusters and exchange them.
*
* In both the cases we only keep the solution if it increases the value of P.
* Another option can be to check all n^2 pairs for both neighborhoods.
* We can add more neighborhoods structures later, if needed.
*/
vector<vector<ll>> insert(vector<vector<ll>>& u, int l = -1) {
    // cerr<<"Local search for the configuration....."<<endl;
    if (u[m].empty()) {
        vector<vector<ll>> u1 = u;
        return u1;
    }
    vector<vector<ll>> u1;
    if (l == -1) {
        l = priority_select(INSERTION_WEIGHTS);
    }
    switch(l) {
        case 0:
            u1 = insert_1(u);
            break;
        case 1:
            u1 = insert_2(u);
            break;
        case 2:
            u1 = insert_3(u);
            break;
        case 3:
            u1 = insert_4(u);
            break;
        case 4:
            u1 = insert_5(u);
            break;
        case 5:
            u1 = insert_6(u);
            break;
        case 6:
            u1 = insert_7(u);
            break;
        case 7:
            u1 = insert_8(u);
            break;
        case 8:
            u1 = insert_9(u);
            break;
    }
    // cerr<<"Local search for the configuration done"<<endl;
    return u1;
}


/*
* Returns false if any salesman doesn't visit any city. True otherwise
*/
bool no_salesmen_empty(vector<vector<ll>>& u) {
    for(ll  i = 0; i < sz(u)-1; i++) {
        if(sz(u[i]) == 0)
            return false;
    }
    return true;
}


bool accept(ll p_new, ll p_prev) {
    if (p_new > p_prev) {
        return true;
    }
    double y = ((double) rand() / (RAND_MAX));
    double p = exp((p_new - p_prev) / temperature);
    return y < p;
}


/*
* MTSP VNS Alogorithm
*/
vector<vector<ll>> mtsp_lns(ll  stoppingTime) {
    vector<vector<ll>> u_historical; 
    // vector<vector<ll>> u = construct_alternate_initial_solution();
    // vector<vector<ll>> u = construct_initial_solution();
    // if(tourInvalid(u)) {
    //     u = construct_alternate_initial_solution();
    // }
    // u = improve_initial_solution(u);
    vector<vector<ll>> u(m + 1);
    for (int i = 1; i < r; i++) {
        u[m].push_back(i);
    }
    u = insert(u, 1);
    if(tourInvalid(u)) {
        u.clear();
        return u;
    }
    cout<<"Final initial solution is: "<<endl;
    printProfit(u);
    printCost(u);
    printTour(u);
    cout << "Max Cost (Tmax): " << Tmax << endl;

    u_historical = u; 
    vector<vector<ll>> u_best = u;
    ll p_current = P(u); 
    ll p_historical = P(u_historical);
    ll p_max = p_current;
    temperature = -MU * p_current / log(0.5);

    auto start = std::chrono::high_resolution_clock::now();
    int effective_iterations = 0;
    int consecutive_bad_iterations = 0;
    int iterations = 0;

    while (true) {
        // cout << "Here" << endl;
        vector<vector<ll>> u1 = remove(u);
        vector<vector<ll>> u11 = insert(u1);
        ll p_new = P(u11);
        if(!tourInvalid(u11) && no_salesmen_empty(u11) && u != u11 && accept(p_new, p_current)) {
            if (p_new > p_max) {
                cout << "Improved profit from " << p_max << " to " << p_new << endl;
                p_max = p_new;
                u_best = u11;
                effective_iterations++;
                consecutive_bad_iterations = 0;
            }
            else {
                consecutive_bad_iterations++;
            }
            u = u11;
            p_historical = p_current;
            p_current = p_new;
        }
        else {
            consecutive_bad_iterations++;
        }

        temperature *= ALPHA;
        iterations++;
        if (iterations % 1000 == 0) {
            cout << "Effective iterations: " << effective_iterations << '/' << iterations << endl;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        if((diff > std::chrono::nanoseconds(stoppingTime*((ll)1e9))) || p_current == total_profit || (consecutive_bad_iterations > MAX_BAD_ITERATIONS) || (iterations > MAX_ITERATIONS))
            break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    outfile<<std::fixed<<chrono::duration_cast<chrono::nanoseconds>(end - start).count()/(ld)(1e9)<<endl;
    outfile<<effective_iterations<<endl;
    cout << "Iterations: " << iterations << endl;
    cerr<<"Algorithm terminated"<<endl;
    return u_best;
}



/*
* Driver function
* argv[1] = filename
* argc[2] = m (number of salesmen)
* argv[3] = Tmax (Hard upper limit on time)
* argv[4] = scheme(g1 or g2)
*/
signed main(int argc, char *argv[]) {
    input(argv[1]);
    outfile.open(argv[2]);
    outfile<<m<<endl;
    outfile<<Tmax<<endl;

    ll m_lo = 1, m_hi = r;
    if(m_hi<m_lo || m < m_lo || m > m_hi){
        outfile<<0<<endl<<0<<endl<<"Invalid File: Not enough salesmen"<<endl;
        return 0;
    }


    srand(time(NULL)); // initialize seed
    set_cities_in_cluster();
    calculate_proximity();
    calculate_relatedness();
    calculate_extended_cost();
    vector<vector<ll>> u = mtsp_lns(MAX_TIME);
    if(sz(u) == 0) {
        outfile<<0<<endl<<total_profit<<endl<<"No solutions possible for the given problem"<<endl;
        return 0;
    }

    outfile << P(u) << endl; 
    outfile << total_profit << endl; 
    outfile << "Valid solution exists for the given problem" << endl;

    cout<<"Printing best solutions: "<<endl;
    printProfit(u);
    printCost(u);
    printTour(u);
    // cout<< "Done!" << endl;
}

// berlin optilmal -> 4040
// eil optimal -> 174
// formula -> Tmax = opt*w*m
