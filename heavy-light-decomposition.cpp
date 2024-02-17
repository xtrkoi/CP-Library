template<bool EDGE_VAL> struct HLD {
  private:
    int n, root, dfs_timer;
    vector<basic_string<int>> adj;
    vector<int> par, dep, sub_sz, tin, top;
  public:
    HLD() : HLD(0) {}
    HLD(int _n, int _root) : n(_n), root(_root), dfs_timer(0), adj(n),
        par(n), dep(n), sub_sz(n), tin(n), top(n) {}
    
    void add_edge(int u, int v) {
        assert(0 <= u && u < n), assert(0 <= v && v < n);
        if (u == v) return;
        adj[u] += v, adj[v] += u;
    }

    void dfs_size(int u) {
        sub_sz[u] = 1;
        for (int &v : adj[u]) {
            par[v] = u, dep[v] = dep[u] + 1;
            adj[v].erase(find(all(adj[v]), u));
            dfs_size(v);
            sub_sz[u] += sub_sz[v];
            if (sub_sz[v] > sub_sz[adj[u][0]]) swap(v, adj[u][0]);
        }
    }

    void dfs_hld(int u) {
        if (!dep[u]) top[u] = u, dfs_timer = 0;
        tin[u] = dfs_timer++;
        for (int v : adj[u]) {
            top[v] = (v == adj[u][0] ? top[u] : v);
            dfs_hld(v);
        }
    }

    void init() { dfs_size(root), dfs_hld(root); }

    int lca(int u, int v) {
        for (; top[u] != top[v]; v = par[top[v]])
            if (dep[top[u]] > dep[top[v]]) swap(u, v);
        return (dep[u] < dep[v] ? u : v);
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }

    template<class F> void process_path(int u, int v, F op) {
        for (; top[u] != top[v]; v = par[top[v]]) {
            if (dep[top[u]] > dep[top[v]]) swap(u, v);
            op(tin[top[v]], tin[v]);
        }
        if (dep[u] > dep[v]) swap(u, v);
        op(tin[u] + EDGE_VAL, tin[v]);
    }

    template<class F> void process_subtree(int u, F op) {
        op(tin[u] + EDGE_VAL, tin[u] + sub_sz[u] - 1);
    }
};
