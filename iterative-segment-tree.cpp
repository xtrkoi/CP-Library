template<class S, S (*op)(S, S), S (*e)()> struct segtree {
  private:
    int _n, _size, _log;
    vector<S> d;
    void updateS(int k) { d[k] = op(d[k << 1], d[k << 1 | 1]); }
    void updateL(int k) { for (int i = 1; i <= _log; ++i) updateS(k >> i); }
    constexpr bool ineq(int x, int y, int z) const { return x <= y && y < z; }
  public:
    segtree() : segtree(0) {}
    explicit segtree(int __n) : segtree(vector<S>(__n, e())) {}
    explicit segtree(const vector<S> &v) : _n((int)v.size()) {
        _log = 31 - __builtin_clz(_n) + ((_n & -_n) != _n), _size = 1 << _log;
        d = vector<S>(_size << 1, e());
        for (int i = 0; i < _n; ++i) d[_size + i] = v[i];
        for (int i = _size - 1; i > 0; --i) updateS(i);
    }

    void assign(int p, S x) {
        assert(ineq(0, p, _n));
        p += _size, d[p] = x, updateL(p);
    }
    S query(int p) {
        assert(ineq(0, p, _n));
        p += _size;
        return d[p];
    }
    S query_all() const { assert(_n > 0); return d[1]; }

    S query(int l, int r) { // [l, r)
        assert(ineq(0, l, r + 1) && ineq(l, r, _n + 1));
        if (l == r) return e();
        l += _size, r += _size;

        S sml = e(), smr = e();
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
        }
        return op(sml, smr);
    }

    template<bool (*f)(S)> int max_right(int l) const { return max_right(l, [](S x) { return f(x); }); }
    template<class V> int max_right(int l, V f) const {
        assert(ineq(0, l, _n + 1) && f(e()));
        if (l == _n) return _n;
        l += _size;
        S sm = e();
        do {
            while (~l & 1) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < _size) if (f(op(sm, d[l <<= 1]))) sm = op(sm, d[l++]);
                return l - _size;
            }
            sm = op(sm, d[l++]);
        } while ((l & -l) != l);
        return _n;
    }

    template<bool (*f)(S)> int min_left(int r) const { return min_left(r, [](S x) { return f(x); }); }
    template<class V> int min_left(int r, V f) const {
        assert(ineq(0, r, _n + 1) && f(e()));
        if (r == 0) return 0;
        r += _size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r & 1)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < _size) if (f(op(d[r = r << 1 | 1], sm))) sm = op(d[r--], sm);
                return r + 1 - _size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }
};
