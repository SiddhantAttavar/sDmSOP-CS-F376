#include <bits/stdc++.h>
#include "utils.h"
#include "constants.h"
#include "solution.h"
#include "precomp.h"
#include "calc.h"
#include "initial.h"
#include "remove.h"
#include "insert.h"
using namespace std;

ofstream outfile; 

// http://e-maxx.ru/algo/assignment_hungary#6 - For Hungarian Algorithm
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

bool accept(ll p_new, ll p_prev, ld temperature) {
    if (p_new > p_prev) {
        return true;
    }
    double y = ((double) rand() / (RAND_MAX));
    double p = exp((p_new - p_prev) / temperature);
    return y < p;
}

void update_weights(vector<double> &weights, int op, double reward, ll time, double &avg_time) {
     if (op == -1) {
         return;
     }

     reward *= avg_time / time;
     avg_time = (1 - EMA_ALPHA) * avg_time + EMA_ALPHA * time;
     weights[op] = min(MAX_WEIGHT, max(MIN_WEIGHT, weights[op] * reward));
     double sum = accumulate(weights.begin(), weights.end(), 0.0);
     for (int i = 0; i < sz(weights); i++) {
         weights[i] /= sum;
     }
}


/*
* MTSP ALNS Alogorithm
*/
void mtsp_alns(Solution &s, ll  stoppingTime) {
    // vector<vector<ll>> u = construct_alternate_initial_solution();
    construct_initial_solution(s);
    // if(tourInvalid(u)) {
    //     u = construct_alternate_initial_solution();
    // }
    improve_initial_solution(s);
    // vector<vector<ll>> u(m + 1);
    // for (int i = 1; i < r; i++) {
    //     u[m].push_back(i);
    // }
    // u = get<0>(insert(u, 1));
    if(tourInvalid(s.u, s)) {
        s.u.clear();
        return;
    }
    cout<<"Final initial solution is: "<<endl;
    printProfit(s);
    printCost(s);
    printTour(s);
    cout << "Max Cost (Tmax): " << s.Tmax << endl;

    vector<vi> u_historical = s.u, u_best = s.u;
    ll p_current = P(s.u, s.profit); 
    ll p_historical = P(u_historical, s.profit);
    ll p_max = p_current;
    s.temperature = -MU * p_current / log(0.5);

    auto start = std::chrono::high_resolution_clock::now();
    int effective_iterations = 0;
    int consecutive_bad_iterations = 0;
    int iterations = 0;

    while (true) {
        // cout << "Here" << endl;
        auto [u1, removal_operator, removal_time] = remove(s);
        auto [u11, insertion_operator, insertion_time] = insert(u1, s);
        ll p_new = P(u11, s.profit);
        double adaptive_reward = p_new / (1.0 * p_current);
        if(!tourInvalid(u11, s) && no_salesmen_empty(u11) && s.u != u11 && accept(p_new, p_current, s.temperature)) {
            if (p_new > p_max) {
                cout << "Improved profit from " << p_max << " to " << p_new << endl;
                p_max = p_new;
                u_best = u11;
                effective_iterations++;
                consecutive_bad_iterations = 0;
                adaptive_reward *= REWARD_BEST;
            }
            else {
                consecutive_bad_iterations++;
            }
            s.u = u11;
            p_historical = p_current;
            p_current = p_new;
        }
        else {
            consecutive_bad_iterations++;
        }

        update_weights(s.removal_weights, removal_operator, adaptive_reward, removal_time, s.avg_removal_time);
        update_weights(s.insertion_weights, insertion_operator, adaptive_reward, insertion_time, s.avg_insertion_time);

        s.temperature *= ALPHA;
        iterations++;
        if (iterations % 1000 == 0) {
            // cout << removal_operator << ' ' << avg_removal_time / removal_time << ' ' << avg_removal_time << endl;
            // cout << insertion_operator << ' ' << avg_insertion_time / insertion_time << ' ' << avg_insertion_time << endl;
            cout << "Effective iterations: " << effective_iterations << '/' << iterations << endl;
            cout.flush();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        if((diff > std::chrono::nanoseconds(stoppingTime*((ll)1e9))) || p_current == s.total_profit || (consecutive_bad_iterations > MAX_BAD_ITERATIONS) || (iterations > MAX_ITERATIONS))
            break;
    }

    cout << "Removal weights: ";
    for (double i : s.removal_weights) {
        cout << std::fixed << setprecision(2) << i << ' ';
    }
    cout << endl;
    cout << "Insertion weights: ";
    for (double i : s.insertion_weights) {
        cout << std::fixed << setprecision(2) << i << ' ';
    }
    cout << endl;

    auto end = std::chrono::high_resolution_clock::now();
    outfile<<std::fixed<<chrono::duration_cast<chrono::nanoseconds>(end - start).count()/(ld)(1e9)<<endl;
    outfile<<effective_iterations<<endl;
    cout << "Iterations: " << iterations << endl;
    cerr<<"Algorithm terminated"<<endl;
    s.u = u_best;
}



/*
* Driver function
* argv[1] = filename
* argc[2] = m (number of salesmen)
* argv[3] = Tmax (Hard upper limit on time)
* argv[4] = scheme(g1 or g2)
*/
signed main(int argc, char *argv[]) {
    IOS
    Solution s = input(argv[1]);
    outfile.open(argv[2]);
    outfile<<s.m<<endl;
    outfile<<s.Tmax<<endl;

    ll m_lo = 1, m_hi = s.r;
    if(m_hi<m_lo || s.m < m_lo || s.m > m_hi){
        outfile<<0<<endl<<0<<endl<<"Invalid File: Not enough salesmen"<<endl;
        return 0;
    }


    srand(time(NULL)); // initialize seed
    set_cities_in_cluster(s);
    calculate_proximity(s);
    calculate_relatedness(s);
    calculate_extended_cost(s);
    mtsp_alns(s, MAX_TIME);
    if(sz(s.u) == 0) {
        outfile<<0<<endl<<s.total_profit<<endl<<"No solutions possible for the given problem"<<endl;
        return 0;
    }

    outfile << P(s.u, s.profit) << endl; 
    outfile << s.total_profit << endl; 
    outfile << "Valid solution exists for the given problem" << endl;

    cout<<"Printing best solutions: "<<endl;
    printProfit(s);
    printCost(s);
    printTour(s);
    // cout<< "Done!" << endl;
}

// berlin optilmal -> 4040
// eil optimal -> 174
// formula -> Tmax = opt*w*m
