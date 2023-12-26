#pragma once
#include <stdlib.h>

bool isPrime(ll num) {
    if (num == 2)
        return true;
    if (num % 2 == 0)
        return false;
    ll d = 3;
    while (d * d <= num) {
        if (num % d == 0)
            return false;
        d += 2;
    }
    return true;
}

list composite(ll num) {
    ll d = 2;
    list l;
    while (d * d <= num) {
        while (num % d == 0) {
            num /= d;
            l.push_back(d);
        }
        d += 1;
    }
    if (num > 1)
        l.push_back(num);
    return l;
}

ll divizors(ll num) {
    if (num == 1)
        return 1;
    ll ans = 2;
    ll d = 2;
    while (d * d < num) {
        if (num % d == 0)
            ans += 2;
        d += 1;
    }
    if (d * d == num)
        ans++;
    return ans;
}

ll BigDivisor(ll num) {
    ll d = 2;
    while (d * d <= num) {
        if (num % d == 0)
            return num / d;
        d++;
    }
    return 1;
}

ll gcd(ll a, ll b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

ll pow(ll& x, ll n, ll& p) {
    if (n == 0)
        return 1;
    if (n % 2 == 0) {
        ll Pow = pow(x, n / 2, p);
        Pow %= p;
        return Pow * Pow;
    }
    return (pow(x, n - 1, p) % p) * x;
}