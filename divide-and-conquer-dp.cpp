template<class T, T (*C)(int, int)> struct dncDP {
    // i < m, j < n, for (k = 0 -> j) dp[i][j] = min(dp[i - 1][k - 1] + C(k, j))
    int m, n;
    vector<T> prvDP, curDP;

    dncDP(int _m, int _n) : m(_m), n(_n), prvDP(_n), curDP(_n) {}

    void compute(int l, int r, int optl, int optr) {
        if (l > r) return;

        int mid = (l + r) >> 1;
        pair<T, int> best = {numeric_limits<T>::max(), -1};
        for (int k = optl; k <= min(mid, optr); k++)
            best = min(best, pair<T, int>((k ? prvDP[k - 1] : 0) + C(k, mid), k));

        curDP[mid] = best.first;
        int opt = best.second;
        compute(l, mid - 1, optl, opt), compute(mid + 1, r, opt, optr);
    }

    T solve() {
        for (int i = 0; i < n; i++) prvDP[i] = C(0, i);
        for (int i = 1; i < m; i++) compute(0, n - 1, 0, n - 1), prvDP = curDP;
        return prvDP.back();
    }
};
