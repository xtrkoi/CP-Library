template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*compose)(F, F), F (*id)()> struct lazy_segtree {
  private:
    int _n, _size, _log;
    vector<S> d;
    vector<F> lz;
    void updateS(int k) { d[k] = op(d[k << 1], d[k << 1 | 1]); }
    void updateL(int k) { for (int i = 1; i <= _log; ++i) updateS(k >> i); }
    void applyS(int k, F a) { d[k] = mapping(a, d[k]); if (k < _size) lz[k] = compose(a, lz[k]); }
    void propagateS(int k) { applyS(k << 1, lz[k]), applyS(k << 1 | 1, lz[k]), lz[k] = id(); }
    void propagateL(int k) { for (int i = _log; i > 0; --i) propagateS(k >> i); }
    constexpr bool ineq(int x, int y, int z) const { return x <= y && y < z; }
  public:
    lazy_segtree() : lazy_segtree(0) {}
    explicit lazy_segtree(int __n) : lazy_segtree(vector<S>(__n, e())) {}
    explicit lazy_segtree(const vector<S> &v) : _n((int)v.size()) {
        _log = 31 - __builtin_clz(_n) + ((_n & -_n) != _n), _size = 1 << _log;
        d = vector<S>(_size << 1, e());
        lz = vector<F>(_size, id());
        for (int i = 0; i < _n; ++i) d[_size + i] = v[i];
        for (int i = _size - 1; i > 0; --i) updateS(i);
    }

    void assign(int p, S x) {
        assert(ineq(0, p, _n));
        propagateL(p += _size), d[p] = x, updateL(p);
    }
    void apply(int p, F a) {
        assert(ineq(0, p, _n));
        propagateL(p += _size), d[p] = mapping(a, d[p]), updateL(p);
    }
    S query(int p) {
        assert(ineq(0, p, _n));
        propagateL(p += _size);
        return d[p];
    }
    S query_all() const { assert(_n > 0); return d[1]; }

    void apply(int l, int r, F a) { // [l, r)
        assert(ineq(0, l, r + 1) && ineq(l, r, _n + 1));
        if (l == r) return;
        l += _size, r += _size;

        int l_ctz = __builtin_ctz(l), r_ctz = __builtin_ctz(r);
        for (int i = _log; i > l_ctz; --i) propagateS(l >> i);
        for (int i = _log; i > r_ctz; --i) propagateS((r - 1) >> i);

        for (int l2 = l, r2 = r; l2 < r2; l2 >>= 1, r2 >>= 1) {
            if (l2 & 1) applyS(l2++, a);
            if (r2 & 1) applyS(--r2, a);
        }

        for (int i = l_ctz + 1; i <= _log; ++i) updateS(l >> i);
        for (int i = r_ctz + 1; i <= _log; ++i) updateS((r - 1) >> i);
    }

    S query(int l, int r) { // [l, r)
        assert(ineq(0, l, r + 1) && ineq(l, r, _n + 1));
        if (l == r) return e();
        l += _size, r += _size;

        int l_ctz = __builtin_ctz(l), r_ctz = __builtin_ctz(r);
        for (int i = _log; i > l_ctz; --i) propagateS(l >> i);
        for (int i = _log; i > r_ctz; --i) propagateS((r - 1) >> i);

        S sml = e(), smr = e();
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
        }
        return op(sml, smr);
    }

    template<bool (*g)(S)> int max_right(int l) { return max_right(l, [](S x) { return g(x); }); }
    template<class G> int max_right(int l, G g) {
        assert(ineq(0, l, _n + 1) && g(e()));
        if (l == _n) return _n;
        propagateL(l += _size);
        S sm = e();
        do {
            while (~l & 1) l >>= 1;
            if (!g(op(sm, d[l]))) {
                while (l < _size) {
                    propagateS(l);
                    if (g(op(sm, d[l <<= 1]))) sm = op(sm, d[l++]);
                }
                return l - _size;
            }
            sm = op(sm, d[l++]);
        } while ((l & -l) != l);
        return _n;
    }

    template<bool (*g)(S)> int min_left(int r) { return min_left(r, [](S x) { return g(x); }); }
    template<class G> int min_left(int r, G g) {
        assert(ineq(0, r, _n + 1) && g(e()));
        if (r == 0) return 0;
        propagateL((r += _size) - 1);
        S sm = e();
        do {
            r--;
            while (r > 1 && (r & 1)) r >>= 1;
            if (!g(op(d[r], sm))) {
                while (r < _size) {
                    propagateS(r);
                    if (g(op(d[r = r << 1 | 1], sm))) sm = op(d[r--], sm);
                }
                return r + 1 - _size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};
