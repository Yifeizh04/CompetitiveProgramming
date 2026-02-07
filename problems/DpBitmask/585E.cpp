#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

// https://codeforces.com/contest/1215/problem/E
// In the answer we would have a specific order 
// we can build it by fixing the order of the prefix
// e.g: place all x as first block, all y as second etc...
// For each color, calculate the moves required to move all elements of a color
// let maxA = 20
// into the prefix: for each element, store how many element is before with specific color (precompute in O(n * maxA))
// when we calculate the costs by fixing a color as last one, we need count how many is already fixed (they don't need to be counted)
// and how many I have have to move
// After this just do DP bitmask
// then precompute the matrix [bit][mask] to find contribution in O(1)
// precomputation: O(maxA * n)
// complexity: O(n * 2^n)

i64 contMask[20][1 << 20];
void solve() {
  int n; 
  cin >> n;  
  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
    --a[i];
  }

  auto b = a;
  sort(b.begin(), b.end());
  b.erase(unique(b.begin(), b.end()), b.end());
  auto comp = [&b](int x) -> int {
    return lower_bound(b.begin(), b.end(), x) - b.begin();
  };

  int m = b.size();
  vector<vector<int>> occ(m);
  for (int i = 0; i < n; ++i) {
    a[i] = comp(a[i]);
    occ[a[i]].push_back(i);
  }

  int N = 1 << m;

  vector delta(m, vector<i64>(m));
  vector<int> cnt(m);
  for (int col = 0; col < m; ++col) {
    cnt.assign(n, 0);
    for (int i = 0; i < n; ++i) {
      if (a[i] != col) {
        cnt[a[i]] += 1;
      } else { // is runned once for all elements in total 
        for (int bit = 0; bit < m; ++bit) {
          delta[col][bit] += cnt[bit];
        }
      }
    }
  }

  auto MSB = [&](int x) -> int {
    return 31 - __builtin_clz(x);
  };

  for (int bit = 0; bit < m; ++bit) {
    for (int mask = 1; mask < N; ++mask) {
      int pos = MSB(mask);
      contMask[bit][mask] = contMask[bit][mask ^ (1 << pos)];
      contMask[bit][mask] += delta[bit][pos];
    }
  }

  constexpr i64 inf = 1E18;
  vector<i64> dp(N, inf);
  dp[0] = 0;
  for (int mask = 1; mask < N; ++mask) {
    for (int bit = 0; bit < m; ++bit) {
      if (mask >> bit & 1) {
        int prvMask = mask ^ (1 << bit);
        // Given mask, I need to subtract all contribution it gives the bits already
        i64 costs = contMask[bit][N - 1] - contMask[bit][prvMask];
        dp[mask] = min(dp[mask], dp[prvMask] + costs);
      }
    }
    // cerr << "Mask " << mask << " --> " << dp[mask] << '\n';
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
