struct bi_matching {
    vector<basic_string<int>> G;
    vector<int> L, R, seen;
    
    bi_matching(int n, int m) : G(n), L(n, -1), R(m, -1), seen(n) {}
    
    void add(int a, int b) { G[a] += b; }
    bool match(int u) {
        if (seen[u]) return false;
        seen[u] = true;
        for (auto v : G[u]) if (R[v] == -1) return L[u] = v, R[v] = u, true;
        for (auto v : G[u]) if (match(R[v])) return L[u] = v, R[v] = u, true;
        return false;
    }
    
    int solve() {
        int matched = true;
        while (matched--) {
            fill(seen.begin(), seen.end(), 0);
            for (int i = 0; i < (int)L.size(); i++)
                if (L[i] == -1) matched |= match(i);
        }
        int res = 0;
        for (int i = 0; i < (int)L.size(); i++) res += (L[i] != -1);
        return res;
    }
};
