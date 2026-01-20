#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include "constants.h"
using namespace std;

#define ll  long long
#define ff first
#define ss second
#define endl "\n"
#define all(x) (x).begin(), (x).end()
#define sz(x) (ll)(x.size())
inline ll  powmod(ll  a,ll  l, ll  md){a%=md; ll  res=1;while(l){if(l&1)res=res*a%md;l/=2;a=a*a%md;}return res;}
inline ll  binpow(ll  a,ll  l){ll  res=1;while(l){if(l&1)res=res*a;l/=2;a=a*a;}return res;}
inline ll  invmod(ll  a, ll  md){return powmod(a,md-2,md);}
typedef unsigned long long ull; typedef long double ld;
typedef vector<ll> vi; typedef pair<ll, ll> ii; typedef vector< ii > vii;
#define pb push_back
inline ll  __set(ll  b, ll  i) {return b|(1LL<<i);} //set ith bit
inline ll  __unset(ll  b, ll  i) {return b&(~(1UL<<i));}
inline ll  __check(ll  b, ll  i) {return b&(1LL<<i);} //returns 0 if ith bit is 0
inline ll  mulmod(ll  a, ll  b, ll  md) {return (((a%md)*(b%md))%md+md)%md;}
inline ll  addmod(ll  a, ll  b, ll  md) {return ((a%md+b%md)%md+md)%md;}
inline ll  submod(ll  a, ll  b, ll  md) {return (((a%md-b%md)%md)+md)%md;}
inline ll  divmod(ll  a, ll  b, ll  md) {return mulmod(a, powmod(b, md-2, md), md);} //if md is prime;
#define IOS ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

inline mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//GIVES A RANDOM NUMBER BETWEEN [l,r] BOTH INCLUSIVE.
inline ll  getRand(ll  l, ll  r){
    if (l > r) {
        return r;
    }
    uniform_int_distribution<ll> uid(l, r);
    return uid(rng);
}

/*
 * Random power select from an ordered list
 */
template<typename T>
inline T rp_select(vector<T> &l, int rp) {
    double y = ((double) rand() / (RAND_MAX));
    return l[(int) (pow(y, rp) * l.size())];
}

inline int priority_select(const vector<double> &weights) {
    double y = ((double) rand() / (RAND_MAX));
    for (int i = 0; i < sz(weights); i++) {
        if (y < weights[i]) {
            return i;
        }
        y -= weights[i];
    }
    return sz(weights) - 1;
}

#endif
