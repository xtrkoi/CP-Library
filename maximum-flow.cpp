template<class F> struct push_relabel {
  private:
    int n;
    struct edge { int v, rev; F flow, capa; };
    vector<vector<edge>> adj;
    vector<F> excess;
    vector<stack<int>> ovfl_list;
    vector<int> h, cnt_h;
    vector<edge*> last_pushed;
  public:
    push_relabel() : push_relabel(0) {}
    push_relabel(int _n) : n(_n), adj(n), 
        excess(n), ovfl_list(n << 1), h(n), cnt_h(n << 1), last_pushed(n) {}
    
    void add_edge(int u, int v, F cap, F rcap = F(0)) {
        assert(0 <= u && u < n), assert(0 <= v && v < n), assert(cap >= F(0));
        if (u == v) return;
        adj[u].push_back({v, sz(adj[v]), F(0), cap});
        adj[v].push_back({u, sz(adj[u]) - 1, F(0), rcap});
    }

    void push(edge &e, F delta) {
        edge &r = adj[e.v][e.rev];
        if (!excess[e.v] && delta) ovfl_list[h[e.v]].push(e.v);
        e.flow += delta, e.capa -= delta, excess[e.v] += delta;
        r.flow -= delta, r.capa += delta, excess[r.v] -= delta;
    }

    F calc(int s, int t) {
        assert(0 <= s && s < n), assert(0 <= t && t < n), assert(s != t);
        h[s] = n, excess[t] = 1, cnt_h[0] = n - 1;
        for (int i = 0; i < n; i++) last_pushed[i] = adj[i].data();
        for (edge &e : adj[s]) push(e, e.capa);

        for (int cur_h = 0;;) {
            while (ovfl_list[cur_h].empty()) if (!cur_h--) return -excess[s];
            int u = ovfl_list[cur_h].top(); ovfl_list[cur_h].pop();
            while (excess[u] > 0) {
                if (last_pushed[u] == adj[u].data() + sz(adj[u])) {
                    h[u] = n << 1;
                    for (edge &e : adj[u]) if (e.capa && h[u] > h[e.v] + 1)
                        h[u] = h[e.v] + 1, last_pushed[u] = &e;

                    cnt_h[h[u]]++, cnt_h[cur_h]--;
                    if (!cnt_h[cur_h] && cur_h) for (int i = 0; i < n; i++) 
                        if (cur_h < h[i] && h[i] < n) cnt_h[h[i]]--, h[i] = n + 1;

                    cur_h = h[u];
                } else if (last_pushed[u]->capa && h[u] == h[last_pushed[u]->v] + 1)
                    push(*last_pushed[u], min(excess[u], last_pushed[u]->capa));
                else ++last_pushed[u];
            }
        }
    }

    bool left_of_min_cut(int u) { return h[u] >= n; }
};
