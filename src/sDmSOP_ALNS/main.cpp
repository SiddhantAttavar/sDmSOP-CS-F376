#include <bits/stdc++.h>
#include "solution.h"
#include "utils.h"
#include "constants.h"
#include "instance.h"
#include "solution.h"
#include "precomp.h"
#include "calc.h"
#include "initial.h"
#include "remove.h"
#include "insert.h"
using namespace std;

struct thread_state {
    atomic<ll> p_current;
    atomic<int> update_iter;
    atomic<bool> alive;
};

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

    reward *= sqrt(avg_time / time);
    avg_time = (1 - EMA_ALPHA) * avg_time + EMA_ALPHA * time;
    weights[op] *= reward;
    double sum = accumulate(weights.begin(), weights.end(), 0.0);
    for (int i = 0; i < sz(weights); i++) {
        weights[i] /= sum;
        weights[i] = min(MAX_WEIGHT, max(MIN_WEIGHT, weights[i]));
    }
}

/**
 * Reheat instance solution to encourage more exploratory behvaiou
 * Called if the current solution is stale (no recent improvements)
 */
void reheat_instance(Instance &s, ll p_current) {
    s.temperature = -MU * p_current / log(0.5);
    s.removal_weights = BASE_REMOVAL_WEIGHTS;
    s.insertion_weights = BASE_INSERTION_WEIGHTS;
}

/*
* MTSP ALNS Alogorithm
*/
Solution mtsp_alns(Instance s, ll  stoppingTime, vector<thread_state> &thread_states, int thread = -1) {
    thread_states[thread].p_current = -1;
    thread_states[thread].update_iter = -1;
    thread_states[thread].alive = -1;

    Solution t = construct_alternate_initial_solution(s);
    // construct_initial_solution(s);
    // if(tourInvalid(u)) {
    //     u = construct_alternate_initial_solution();
    // }
    improve_initial_solution(s, t);
    // vector<vector<ll>> u(m + 1);
    // for (int i = 1; i < r; i++) {
    //     u[m].push_back(i);
    // }
    // u = get<0>(insert(u, 1));
    if(tourInvalid(t.u, s)) {
        t.u.clear();
        return t;
    }
    cout<<"Final initial solution is: "<<endl;
    printProfit(t, s);
    printCost(t, s);
    printTour(t);
    printTour(t);
    cout << "Max Cost (Tmax): " << s.Tmax << endl;

    Solution t_best = t;
    ll p_current = P(t.u, s.profit); 
    ll p_max = p_current;
    s.temperature = -MU * p_current / log(0.5);

    int effective_iterations = 0;
    int consecutive_bad_iterations = 0;
    int iterations = 0;

    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        // cout << "Here" << endl;
        auto [t1, removal_operator, removal_time] = remove(s, t);
        auto [t11, insertion_operator, insertion_time] = insert(s, t1);
        ll p_new = P(t11.u, s.profit);
        double adaptive_reward = p_new / (1.0 * p_current);
        if(!tourInvalid(t11.u, s) && no_salesmen_empty(t11.u) && t.u != t11.u && accept(p_new, p_current, s.temperature)) {
            if (p_new > p_max) {
                cout << "Thread " << thread << ": Improved profit from " << p_max << " to " << p_new << endl;
                p_max = p_new;
                t_best = t11;
                effective_iterations++;
                consecutive_bad_iterations = 0;
                adaptive_reward *= REWARD_BEST;
            }
            else {
                consecutive_bad_iterations++;
            }
            t = t11;
            p_current = p_new;
        }
        else {
            consecutive_bad_iterations++;
        }

        update_weights(s.removal_weights, removal_operator, adaptive_reward, removal_time, s.avg_removal_time);
        update_weights(s.insertion_weights, insertion_operator, adaptive_reward, insertion_time, s.avg_insertion_time);

        // if (consecutive_bad_iterations > MAX_BAD_ITERATIONS) {
        //     cout << "Thread " << thread << ": Reheating instance" << endl;
        //     t = t_best;
        //     p_current = P(t.u, s.profit);
        //     reheat_instance(s, p_current);
        //     consecutive_bad_iterations = 0;
        // }

        s.temperature *= ALPHA;
        iterations++;
        if (iterations % PRINT_ITERATIONS == 0) {
            cout << "Thread " << thread << ": Effective iterations: " << effective_iterations << '/' << iterations << endl;
            cout.flush();
        }

        if (iterations % (MAX_ITERATIONS / NUM_THREADS) == 0) {
            // Kill bad performing threads
            thread_states[thread].p_current = p_current;
            thread_states[thread].update_iter = iterations;
            bool flag = true;
            for (int i = 0; i < NUM_THREADS; i++) {
                if (thread_states[i].alive and thread_states[i].update_iter < iterations) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                int j = -1, c = 0;
                ll p_worst = inf;
                for (int i = 0; i < NUM_THREADS; i++) {
                    if (thread_states[i].alive) {
                        c++;
                        if (j == -1 or thread_states[i].p_current < p_worst) {
                            j = i;
                            p_worst = thread_states[j].p_current;
                        }
                    }
                }
                if (c > 1) {
                    cout << "Killed thread " << j << endl;
                    thread_states[j].alive = false;
                }
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::nanoseconds>(end - start);
        if((diff > std::chrono::nanoseconds(stoppingTime*((ll)1e9))) || p_current == s.total_profit || (iterations > MAX_ITERATIONS) || !thread_states[thread].alive)
            break;
    }

    cout << "Removal weights: ";
    for (double i : s.removal_weights) {
        cout << std::fixed << setprecision(4) << i << ' ';
    }
    cout << endl;
    cout << "Insertion weights: ";
    for (double i : s.insertion_weights) {
        cout << std::fixed << setprecision(4) << i << ' ';
    }
    cout << endl;

    cout << "Iterations: " << iterations << endl;
    cerr<<"Algorithm terminated"<<endl;
    return t_best;
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
    if (argc != 3) {
        cout << "Invalid arguments" << endl;
        cout << "Usage: ./main.out <instance_file> <output_file>" << endl;
        return 0;
    }

    Instance s = input(argv[1]);
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

    auto start = std::chrono::high_resolution_clock::now();
    vector<future<Solution>> solutions(NUM_THREADS);
    vector<thread_state> thread_states(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        solutions[i] = async(mtsp_alns, s, MAX_TIME, std::ref(thread_states), i);
    }

    Solution t;
    ll p_best = -1;
    for (int i = 0; i < NUM_THREADS; i++) {
        Solution t1 = solutions[i].get();
        ll p_current = P(t1.u, s.profit);
        if (p_current > p_best) {
            t = t1;
            p_best = p_current;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    outfile<<std::fixed<<chrono::duration_cast<chrono::nanoseconds>(end - start).count()/(ld)(1e9)<<endl;

    if(sz(t.u) == 0) {
        outfile<<0<<endl<<s.total_profit<<endl<<"No solutions possible for the given problem"<<endl;
        return 0;
    }

    outfile << P(t.u, s.profit) << endl; 
    outfile << s.total_profit << endl; 
    outfile << "Valid solution exists for the given problem" << endl;

    cout<<"Printing best solutions: "<<endl;
    printProfit(t, s);
    printCost(t, s);
    printTour(t);
    printTourFile(t, outfile);
    // cout<< "Done!" << endl;
}

// berlin optilmal -> 4040
// eil optimal -> 174
// formula -> Tmax = opt*w*mint
