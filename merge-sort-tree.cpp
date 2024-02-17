template<class T> struct merge_sort_tree {
    int _l, _r, _m;
    vector<T> v;
    merge_sort_tree *lt, *rt;
    merge_sort_tree(int l, int r, const vector<T> &e) : _l(l), _r(r), _m((l + r) >> 1), v(r - l + 1) {
        if (l == r) v[0] = e[l], lt = rt = nullptr;
        else {
            lt = new merge_sort_tree(_l, _m, e);
            rt = new merge_sort_tree(_m + 1, _r, e);
            merge(all(lt->v), all(rt->v), v.begin());
        }
    }
    int count(int qs, int qe, T x, T y) {
        if (x > y || _r < qs || qe < _l) return 0;
        if (qs <= _l && _r <= qe) return upper_bound(all(v), y) - lower_bound(all(v), x);
        return lt->count(qs, qe, x, y) + rt->count(qs, qe, x, y);
    }
};
