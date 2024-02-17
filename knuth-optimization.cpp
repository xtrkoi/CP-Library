template<class T, T (*C)(int, int)> struct knuthDP {
    // for (k = i -> j - 1) dp[i][j] = min(dp[i][k] + dp[k + 1][j] + C(i, j))
    int n;
    vector<vector<T>> dp, opt;

    knuthDP(int _n) : n(_n), 
        dp(n, vector<T>(n)), opt(n, vector<T>(n)) {}
    
    T solve() {
        for (int i = 0; i < n; i++) opt[i][i] = i, dp[i][i] = C(i, i);
        for (int i = n - 2; i >= 0; i--) for (int j = i + 1; j < n; j++) {
            T cost = C(i, j), mn = numeric_limits<T>::max();
            for (int k = opt[i][j - 1]; k <= min(j - 1, opt[i + 1][j]); k++)
                if (mn >= dp[i][k] + dp[k + 1][j] + cost)
                    mn = dp[i][k] + dp[k + 1][j] + cost, opt[i][j] = k;
            dp[i][j] = mn;
        }
        return dp[0][n - 1];
    }
};
