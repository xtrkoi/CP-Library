using graph = vector<basic_string<int>>;

graph bi_conn(graph &g, vector<bool> &is_cutpoint, vector<int> &id) {
    int n = (int)g.size();
    graph comps;
    vector<int> num(n), low(n), stk;
    is_cutpoint.resize(n), id.resize(n);

    function<void(int, int, int&)> dfs = [&](int u, int p, int &timer) {
        num[u] = low[u] = ++timer, stk.push_back(u);
        for (int v : g[u]) if (v != p) {
            if (!num[v]) {
                dfs(v, u, timer);
                low[u] = min(low[u], low[v]);
                if (low[v] >= num[u]) {
                    is_cutpoint[u] = (num[u] > 1 || num[v] > 2);
                    comps.push_back({u});
                    while (comps.back().back() != v) 
                        comps.back().push_back(stk.back()), stk.pop_back();
                }
            } else low[u] = min(low[u], num[v]);
        }
    };
    for (int u = 0, t; u < n; u++) if (!num[u]) dfs(u, -1, t = 0);
    
    function<graph()> build_tree = [&]() {
        graph t(1);
        int root = 0;
        for (int u = 0; u < n; u++) if (is_cutpoint[u]) id[u] = root++, t.push_back({});
        for (auto &comp : comps) {
            int node = root++;
            t.push_back({});
            for (int u : comp) {
                if (!is_cutpoint[u]) id[u] = node;
                else t[node].push_back(id[u]), t[id[u]].push_back(node);
            }
        }
        return t;
    };
    return build_tree();
}
