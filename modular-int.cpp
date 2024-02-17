template<int MOD> struct mint {
    int v;
    explicit operator int() { return v; }
    mint() : v(0) {}
    mint(long long _v) : v((int)(_v % MOD)) { v += MOD * (v < 0); }
    mint& operator+=(mint o) { v += o.v, v -= MOD * (v >= MOD); return *this; }
    mint& operator-=(mint o) { v -= o.v, v += MOD * (v < 0); return *this; }
    mint& operator*=(mint o) { v = (int)(1LL * v * o.v % MOD); return *this; }
    static mint power(mint a, long long b, mint res = 1) { assert(b >= 0);
        for (; b; a *= a, b >>= 1) res *= (b & 1 ? a : 1);
        return res;
    }
    static mint inverse(mint a) { return power(a, MOD - 2); }
    mint& operator/=(mint o) { *this *= inverse(o); return *this; }
    friend mint operator+(mint a, mint b) { return a += b; }
    friend mint operator-(mint a, mint b) { return a -= b; }
    friend mint operator*(mint a, mint b) { return a *= b; }
    friend mint operator/(mint a, mint b) { return a /= b; }
    friend mint operator-(const mint a) { return 0 - a; }
    friend ostream& operator<<(ostream &os, mint a) { return os << a.v; }
    friend bool operator==(const mint &o) { return v == o.v; }
    friend bool operator!=(const mint &o) { return v != o.v; }
};
