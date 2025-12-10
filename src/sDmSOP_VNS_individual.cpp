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
const ll INF = 1e9;
const ll MAX_LIMIT_REPEAT = 100;
// const ll MAX_TIME = 1800;
const ll MAX_TIME = 1800;
const ll MAX_BAD_ITERATIONS = 100000;

ofstream outfile; 


priority_queue<ll, vector<ll>, greater<ll> > pq; //for min priority_queue
#define IOS ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);


mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//GIVES A RANDOM NUMBER BETWEEN [l,r] BOTH INCLUSIVE.
ll  getRand(ll  l, ll  r){
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
ll  n, m, r;
ll total_profit = 0; 
vector<vector<ll>> cost, cluster;
vector<ll> profit;
unordered_map<ll, ll>mp;

void input(string file_name, ll scheme){
        ifstream infile(file_name);
        string var, node_count, set_count, line;
        infile>>var>>node_count;
        infile>>var>>set_count;
        n = stoll(node_count);
        r = stoll(set_count)+1;

        for(ll  i=0; i<4; i++){
            infile>>var;
            // cout<<var<<endl;
        }
        cost.resize(n+1,vector<ll>(n+1,0));
        cluster.resize(r,vector<ll>(n+1,0));

        /*
            city 1 is the starting point
            it belongs to a cluster number 0 with profit 0 
        */
        cluster[0][1] = 1;
        ii loneDepot = {0, 0};

        getline(infile,line);
        /*populates clusters*/
        for(ll  i=1;i<r;i++){
            getline(infile,line);
            istringstream ss(line);
            /*first value represents size of cluster*/
            ss>>var;
            // cout<<"Cluster Size:"<<line<<" units\n";
            ll  j = stoi(var);
            while(ss>>var){
                cluster[i][stoi(var)]=1;
                mp[stoi(var)] = i;
            }
            /*
                1 does not belong to any other cluster
            */
            cluster[i][1] = 0;
            if(j == 1 && stoi(var) == 1){
                loneDepot.ff = 1;
                loneDepot.ss = i;
            }
        }
    
        mp[1] = 0;
        if(loneDepot.ff){
            cluster.erase(cluster.begin() + loneDepot.ss);
            r--;
            for(ll clustno=1; clustno<r; clustno++){
                for(ll pt=1; pt<=n; pt++){
                    if(cluster[clustno][pt])
                        mp[pt] = clustno;
                }
            }
        }

        // Printing Cluster Populations
        // for(int i=0; i<r; i++){
        //     for(int j=1; j<=n; j++){
        //         cout << cluster[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        /*populate costs*/
        for(ll i=1; i<=n; i++){
            getline(infile,line);
            istringstream ss(line);
            ll  j=1;
            while(ss>>var){
                cost[i][j]=stoi(var);
                j++;
            }
        }

        // Printing Cost Populations
        // for(int i=1; i<=n; i++){
        //     for(int j=1; j<=n; j++){
        //         cout << cost[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

    
        profit.resize(r,0);
        // cout << profit[0] << " ";
        for(ll  i=1; i<r; i++){
            for(ll  j=1; j<=n; j++){
                if(scheme == 1){
                    profit[i]+=cluster[i][j];
                }
                else if(scheme == 2){
                    if(cluster[i][j])
                        profit[i]+=(1+7141*j)%100;
                }
                else{
                    cout << "Incorrect Evaluation Scheme!!" << endl;
                    exit(0);
                }
            }

            total_profit += profit[i];
            // cout << profit[i] << " ";
        }
        // cout << endl << endl;

        /*
            refer vns papers for g1 and g2 schemes
            which are used for assigning profits to profits;
        */
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
ll  Tmax;
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

    vector<pair<ll,ll>> dp_cur(sz(cities_in_cluster[cluster2]), {1e18,0});
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
    int tot = 0;
    for(int i=0; i<sz(cost); i++){
        cout<<"Cost for traveller "<< i+1 << " is " << cost[i] <<endl;
        tot += cost[i];
    }

    cout << "Cost of tour is: " << tot << endl; 
    // cerr<<"Printing Cost of tour done"<<endl;
}

ll getTourCost(vi cost){

    int tot = 0;
    for(int c: cost){
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
    
    vector<ll> weight_row(r, 1e18);
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
        ld mn_val = 1e18;
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
                id1 = j, id2 = 1e18;
            }
            // cout<<"check 5"<<endl;
        }
        if(id2 != -1) {
            seen[i]=1;
            if(id2 == 1e18) {
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
        ld mn_val = 1e18;
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
                id1 = j, id2 = 1e18;
            }
        }
        if(id2 != -1) {
            seen[i]=1;
            if(id2 == 1e18) {
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
* Path move:
*   Select a path i.e. a subarray and index and move the
*   subpart to that position. While selecting path we ensure
*   that the whole path is taken from the same salesman.
*/
vector<vector<ll>> shake_neighborhood_1(vector<vector<ll>>& u) {
    // cerr<<"Shaking the configuration for neighborhood 1....."<<endl;
    vector<vector<ll>> u1 = u;
    ll  rSwap = rand()%(m+1);
    ll  rRange = rand()%(m+1);
    ll  cnt=0;
    while(sz(u1[rSwap]) == 0 || sz(u1[rRange]) == 0) {
        rSwap = rand()%(m+1);
        rRange = rand()%(m+1);
        cnt++;
        if(cnt >= MAX_LIMIT_REPEAT)
            return u1;
    }
    ll  idSwap = rand()%sz(u1[rSwap]);
    ll  lo = rand()%sz(u1[rRange]), hi = rand()%sz(u1[rRange]);
    if(lo > hi)
        swap(lo, hi);
    ll  whichSide = rand()%2;
    // cout<<rSwap<<" "<<idSwap<<endl;
    // cout<<rRange<<" "<<lo<<" "<<hi<<endl;
    // cout<<whichSide<<endl;
    if(rSwap == rRange) {
        if(idSwap >= lo && idSwap <= hi) {
            return u1;
        }
        vector<ll> newShakeList;
        if(idSwap > hi) {
            for(ll  i = 0; i < lo; i++)
                newShakeList.pb(u1[rSwap][i]);
            for(ll  i = hi+1; i < idSwap; i++)
                newShakeList.pb(u1[rSwap][i]);
            if(whichSide == 0) {
                for(ll  i = lo; i <= hi; i++)
                    newShakeList.pb(u1[rSwap][i]);
                newShakeList.pb(u1[rSwap][idSwap]);
            } else {
                newShakeList.pb(u1[rSwap][idSwap]);
                for(ll  i = lo; i <= hi; i++)
                    newShakeList.pb(u1[rSwap][i]);
            }
            for(ll  i = idSwap+1; i < sz(u1[rSwap]); i++)
                newShakeList.pb(u1[rSwap][i]);
        } else {
            // vector<ll> newShakeList;
            for(ll  i = 0; i < idSwap; i++)
                newShakeList.pb(u1[rSwap][i]);
            if(whichSide == 0) {
                for(ll  i = lo; i <= hi; i++)
                    newShakeList.pb(u1[rSwap][i]);
                newShakeList.pb(u1[rSwap][idSwap]);
            } else {
                newShakeList.pb(u1[rSwap][idSwap]);
                for(ll  i = lo; i <= hi; i++)
                    newShakeList.pb(u1[rSwap][i]);
            }
            for(ll  i = idSwap+1; i < lo; i++)
                newShakeList.pb(u1[rSwap][i]);
            for(ll  i = hi+1; i < sz(u1[rSwap]); i++)
                newShakeList.pb(u1[rSwap][i]);
        }
        u1[rSwap] = newShakeList;
    } else {
        vector<ll> newShakeList;
        for(ll  i = 0; i < idSwap; i++)
            newShakeList.pb(u1[rSwap][i]);
        if(whichSide == 0) {
            for(ll  i = lo; i <= hi; i++)
                newShakeList.pb(u1[rRange][i]);
            newShakeList.pb(u1[rSwap][idSwap]);
        } else {
            newShakeList.pb(u1[rSwap][idSwap]);
            for(ll  i = lo; i <= hi; i++)
                newShakeList.pb(u1[rRange][i]);
        }
        for(ll  i = idSwap+1; i < sz(u1[rSwap]); i++)
            newShakeList.pb(u1[rSwap][i]);

        vector<ll> newShakeList1;
        for(ll  i = 0; i < lo; i++)
            newShakeList1.pb(u1[rRange][i]);
        for(ll  i = hi+1; i < sz(u1[rRange]); i++)
            newShakeList1.pb(u1[rRange][i]);

        u1[rSwap] = newShakeList;
        u1[rRange] = newShakeList1;
    }

    return u1;
}


vi getSameId(vector<vector<ll>>& u1, ll  rP1) {
    vector<ll> id(4);
    id[0] = rand()%sz(u1[rP1]);
    id[1] = rand()%sz(u1[rP1]);
    id[2] = rand()%sz(u1[rP1]);
    id[3] = rand()%sz(u1[rP1]);
    sort(all(id));
    return id;
    
}

/*
* Path exchange:  
*   Select two paths and exchange them. Again while 
*   paths we select each path from one salesman. 
*/
vector<vector<ll>> shake_neighborhood_2(vector<vector<ll>>& u) {
    // cerr<<"Shaking the configuration for neighborhood 2....."<<endl;

    vector<vector<ll>> u1 = u;
    ll  rP1 = rand()%(m+1), rP2 = rand()%(m+1);
    ll  cnt=0;
    while(sz(u1[rP1]) == 0 || sz(u1[rP2]) == 0) {
        rP1 = rand()%(m+1);
        rP2 = rand()%(m+1);
        cnt++;
        if(cnt >= MAX_LIMIT_REPEAT)
            return u1;
    }

    vector<ll> id(4);
    id[0] = rand()%sz(u1[rP1]);
    id[1] = rand()%sz(u1[rP1]);
    id[2] = rand()%sz(u1[rP2]);
    id[3] = rand()%sz(u1[rP2]);
    if(rP1 == rP2) {
        cnt=0;
        do {
            id = getSameId(u1, rP1);
            cnt++;
            if(cnt >= MAX_LIMIT_REPEAT)
                return u1;
        } while(id[1] == id[2]);
        vector<ll> newShakeList;
        for(ll  i = 0; i < id[0]; i++)
            newShakeList.pb(u1[rP1][i]);
        for(ll  i = id[2]; i <= id[3]; i++)
            newShakeList.pb(u1[rP1][i]);
        for(ll  i = id[1]+1; i < id[2]; i++)
            newShakeList.pb(u1[rP1][i]);
        for(ll  i = id[0]; i <= id[1]; i++)
            newShakeList.pb(u1[rP1][i]);
        for(ll  i = id[3]+1; i < sz(u1[rP1]); i++)
            newShakeList.pb(u1[rP1][i]);
        u1[rP1] = newShakeList;    
    } else {
        if(id[0] > id[1])
            swap(id[0], id[1]);
        if(id[2] > id[3])
            swap(id[2], id[3]);
        vector<ll> newShakeList1, newShakeList2;

        for(ll  i = 0; i < id[0]; i++)
            newShakeList1.pb(u1[rP1][i]);
        for(ll  i = id[2]; i <= id[3]; i++)
            newShakeList1.pb(u1[rP2][i]);
        for(ll  i = id[1]+1; i < sz(u1[rP1]); i++)
            newShakeList1.pb(u1[rP1][i]);

        for(ll  i = 0; i < id[2]; i++)
            newShakeList2.pb(u1[rP2][i]);
        for(ll  i = id[0]; i <= id[1]; i++)
            newShakeList2.pb(u1[rP1][i]);
        for(ll  i = id[3]+1; i < sz(u1[rP2]); i++)
            newShakeList2.pb(u1[rP2][i]);
        u1[rP1] = newShakeList1;
        u1[rP2] = newShakeList2;
    }

    return u1;
}



/*
* Shaking procedure. Contains two different neighborhood structures:
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
vector<vector<ll>> shake(vector<vector<ll>>& u, ll  l) {
    // cerr<<"Shaking the configuration....."<<endl;
    vector<vector<ll>> u1;
    switch(l) {
        case 1:
            // cout<<"shake_neighborhood_1"<<endl;
            u1 = shake_neighborhood_1(u);
            break;
        case 2:
            // cout<<"shake_neighborhood_2"<<endl;
            u1 = shake_neighborhood_2(u);
            break;
    }
    // cerr<<"Shaking the configuration done"<<endl;
    return u1;
}



/*
* One cluster move:
*   Select two random positions i1, i2. Now we can put:
*   1. i1 after i2 
*   or 
*   2. i2 before i1.
*   include non-path clusters or not? 
*/
vector<vector<ll>> local_search_neighborhood_1(vector<vector<ll>>& u) {
    // cerr<<"Local search for the configuration for neighborhood 1....."<<endl;
    // printTour(u);
    vector<vector<ll>> u1 = u;
    ll  i = (rand()%(r-1)), j = (rand()%(r-1));
    // cout << "i = " << i << " j = " << j << endl;
    ll tot = r-sz(u[m])-1;
    // cout << "tot = " << tot << endl;
    if(i>=tot || j>=tot || i == j)
        return u1;

    // if(i==j)
    //     return u1;

    ii pi = find_pos_in_u(u1, min(i, j));
    ii pj = find_pos_in_u(u1, max(i, j));

    
    ll  opt2 = rand()%2;
    ll  vi = u1[pi.ff][pi.ss], vj = u1[pj.ff][pj.ss];
    
    // cout<<pi.ff<<" "<<pi.ss<<" "<<pj.ff<<" "<<pj.ss<<" "<<opt2<<endl;
    if(opt2) {
            u1[pj.ff].insert(u1[pj.ff].begin()+pj.ss+1, vi);
            u1[pi.ff].erase(u1[pi.ff].begin()+pi.ss);
    } 

    else {
        u1[pi.ff].insert(u1[pi.ff].begin()+pi.ss, vj);
        if(pi.ff != pj.ff)
            u1[pj.ff].erase(u1[pj.ff].begin()+pj.ss);
        else
            u1[pj.ff].erase(u1[pj.ff].begin()+pj.ss+1);

    }
    // printTour(u1);
    // cerr<<"Local search for the configuration for neighborhood 1 done"<<endl;
    return u1;
}



/*
* One cluster exchange:  
*   Select two random clusters and exchange them.
*/
vector<vector<ll>> local_search_neighborhood_2(vector<vector<ll>>& u) {
    // cerr<<"Local search for the configuration for neighborhood 2....."<<endl;
    vector<vector<ll>> u1 = u;
    ll  i = (rand()%(r-1)), j = (rand()%(r-1));
    // cout << "i = " << i << " j = " << j << endl;
    ll tot = r-sz(u[m])-1;
    if(i>=tot || j>=tot)
        return u1;

    ii pi = find_pos_in_u(u1, i);
    ii pj = find_pos_in_u(u1, j);
    swap(u1[pi.ff][pi.ss], u1[pj.ff][pj.ss]);
    // cerr<<"Local search for the configuration for neighborhood 2 done"<<endl;
    return u1;
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
vector<vector<ll>> local_search(vector<vector<ll>>& u, ll  l) {
    // cerr<<"Local search for the configuration....."<<endl;
    vector<vector<ll>> u1;
    switch(l) {
        case 1:
            // cout << "local_search_neighborhood_1" << endl;
            u1 = local_search_neighborhood_1(u);
            break;
        case 2:
            // cout << "local_search_neighborhood_2" << endl;
            u1 = local_search_neighborhood_2(u);
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



/*
* MTSP VNS Alogorithm
*/
vector<vector<ll>> mtsp_vns(ll  stoppingTime) {
    vector<vector<ll>> u_historical; 
    vector<vector<ll>> u = construct_alternate_initial_solution();
    // vector<vector<ll>> u = construct_initial_solution();
    // if(!all_salesmen_full(u) || tourInvalid(u)) {
    //     u = construct_alternate_initial_solution();
    // }
    u = improve_initial_solution(u);
    if(tourInvalid(u)) {
        u.clear();
        return u;
    }
    cout<<"Final initial solution is: "<<endl;
    printProfit(u);
    printCost(u);
    printTour(u);

    u_historical = u; 
    ll p_current = P(u); 
    ll p_historical = P(u_historical);

    auto start = std::chrono::high_resolution_clock::now();
    int effective_iterations = 0;
    int consecutive_bad_iterations = 0;
    int iterations = 0;

    while(1) {
        // cout << "Here" << endl;
        ll  l = 1;
        while (l <= Lmax) {
            vector<vector<ll>> u1 = shake(u, l);
            vector<vector<ll>> u11 = local_search(u1, l);
            if(!tourInvalid(u11) && no_salesmen_empty(u11) && P(u11) > P(u) && u11 != u && u11 != u_historical) {
                cout<<"Improved profit from "<<P(u)<<" to "<<P(u11)<<endl;
                effective_iterations++;
                u = u11;
                l = 1;
                p_historical = p_current;
                p_current = P(u);
            } else {
                l++;
            }
        }

        if(p_historical -= p_current)
            consecutive_bad_iterations++;
        else
            consecutive_bad_iterations = 0;

        if (iterations % 10000 == 0) {
            cout << "Effective iterations: " << effective_iterations << '/' << iterations << endl;
        }
        iterations++;
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        if((diff > std::chrono::nanoseconds(stoppingTime*((ll)1e9))) || p_current == total_profit || (consecutive_bad_iterations > MAX_BAD_ITERATIONS))
            break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    outfile<<std::fixed<<chrono::duration_cast<chrono::nanoseconds>(end - start).count()/(ld)(1e9)<<endl;
    outfile<<effective_iterations<<endl;
    cerr<<"Algorithm terminated"<<endl;
    return u;
}



/*
* Driver function
* argv[1] = filename
* argc[2] = m (number of salesmen)
* argv[3] = Tmax (Hard upper limit on time)
* argv[4] = scheme(g1 or g2)
*/
signed main(int argc, char *argv[]) {

    m = stoi(argv[2]);
    Tmax = stoll(argv[3]);
    ll profitType = stoi(argv[4]);
    outfile.open(argv[5]);
    
    outfile<<m<<endl<<profitType<<endl<<Tmax<<endl;
    input(argv[1], profitType);

    ll m_lo = 1, m_hi = r;
    if(m_hi<m_lo || m < m_lo || m > m_hi){
        outfile<<0<<endl<<0<<endl<<0<<endl<<0<<endl<<"Invalid File: Not enough salesmen"<<endl;
        return 0;
    }


    srand(time(NULL)); // initialize seed
    set_cities_in_cluster();
    vector<vector<ll>> u = mtsp_vns(MAX_TIME);
    if(sz(u) == 0) {
        outfile<<0<<endl<<0<<endl<<0<<endl<<total_profit<<endl<<"No solutions possible for the given problem"<<endl;
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
