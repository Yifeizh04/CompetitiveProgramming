#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

constexpr i64 inf = 1E18;

#pragma GCC optimize("O3")
#pragma GCC optimization("Ofast,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

/*
Sort the people by a_i decreasingly, and process them in this order
Since we can choose k members to be in the audience, is always convenient
to choose from the first k+p (+p because some of them we can assign it for the positions)
After k+p, let dp[mask] be the maximum strength with k people assigned and `mask` configuration:
- we can decide to choose it as audience (possible if we have less k people)
- we can decide to choose it into the mask

After the k+p, we can only change the mask (positions) and from there is just standard dp bitmask
Complexity: O(p * 2^p * n) 
*/

void solve() {
  int n, p, k;
  cin >> n >> p >> k;
  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }

  vector s(n, vector<int>(p));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < p; ++j) {
      cin >> s[i][j];
    }
  }

  vector<int> ord(n);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int i, int j) {
    return a[i] > a[j];
  });

  const int N = 1 << p;
  vector<i64> dp(N), ndp(N);
  dp[0] = 0;

  auto bitCount = [&](int mask) -> int {
    return __builtin_popcount(mask);
  };

  auto vmax = [&](i64 &x, i64 y) {
    x = max(x, y);
  };

  // process first p + k elements
  for (int i = 0; i < n; ++i) {
    ndp = dp;
    int idx = ord[i];
    for (int mask = 0; mask < N; ++mask) {
      int cnt = bitCount(mask);
      // choose it into k, can do it if 
      if (i < p + k && cnt <= i) {
        int alreadyK = i - cnt;
        if (alreadyK + 1 <= k) {
          vmax(ndp[mask], dp[mask] + a[idx]);
        }
      }
      // choose it into p
      for (int bit = 0; bit < p; ++bit) {
        if (mask >> bit & 1) {
          vmax(ndp[mask], dp[mask ^ (1 << bit)] + s[idx][bit]);
        }
      }
    }

    swap(dp, ndp);
  }

  cout << dp.back() << "\n";
}


int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int tt = 1;
  // cin >> tt;

  while (tt--) {
    solve();
  }

  return 0;
}
