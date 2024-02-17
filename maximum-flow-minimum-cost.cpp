template<class F, class C> struct mincost_SSP {
  private:
    int n;
    struct edge { int v; F flow, capa; C cost; };
    vector<basic_string<int>> adj;
    vector<edge> edge_list;
    vector<C> dist, min_h;
    vector<int> prev;

    const F INF_F = numeric_limits<F>::max();
    const C INF_C = numeric_limits<C>::max();
  public:
    mincost_SSP() : mincost_SSP(0) {}
    mincost_SSP(int _n) : n(_n), adj(n), edge_list(0), dist(n), min_h(n), prev(n) {}

    void add_edge(int u, int v, F cap, C cost, F rcap = F(0)) {
        assert(0 <= u && u < n), assert(0 <= v && v < n);
        assert(cap >= F(0));
        adj[u] += sz(edge_list), edge_list.push_back({v, F(0), cap, cost});
        adj[v] += sz(edge_list), edge_list.push_back({u, F(0), rcap, -cost});
    }

    bool find_path(int s, int t) {
        dist.assign(n, INF_C);

        using T = pair<C, int>;
        priority_queue<T, vector<T>, greater<T>> pq;
        pq.emplace(dist[s] = C(0), s);

        while (!pq.empty()) {
            T u = pq.top(); pq.pop();
            if (u.first > dist[u.second]) continue;
            for (int i : adj[u.second]) {
                const edge &e = edge_list[i];
                if (e.capa && ckmin(dist[e.v], u.first + e.cost + min_h[u.second] - min_h[e.v])) 
                    prev[e.v] = i, pq.emplace(dist[e.v], e.v);
            }
        }
        return dist[t] != INF_C;
    }

    pair<F, C> calc(int s, int t) {
        assert(0 <= s && s < n), assert(0 <= t && t < n), assert(s != t);
        for (int i = 0; i < n; i++) for (int j = 0; j < sz(edge_list); j++) {
            const edge &e = edge_list[j], &r = edge_list[j ^ 1];
            if (e.capa) ckmin(min_h[e.v], min_h[r.v] + e.cost);
        }
        
        F totFlow = F(0);
        C totCost = C(0);
        while (find_path(s, t)) {
            for (int i = 0; i < n; i++) 
                min_h[i] = (dist[i] == INF_C ? INF_C : min_h[i] + dist[i]);
            
            F delta = INF_F;
            for (int x = t; x != s; x = edge_list[prev[x] ^ 1].v)
                ckmin(delta, edge_list[prev[x]].capa);
            
            totFlow += delta, totCost += (min_h[t] - min_h[s]) * delta;
            for (int x = t; x != s; x = edge_list[prev[x] ^ 1].v) {
                int i = prev[x], j = i ^ 1;
                edge_list[i].flow += delta, edge_list[i].capa -= delta;
                edge_list[j].flow -= delta, edge_list[j].capa += delta;
            }
        }
        return pair<F, C>(totFlow, totCost);
    }
};
