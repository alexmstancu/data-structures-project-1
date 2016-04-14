#include <string>
#include <iostream>
#include "hashtable.h"

void test_int_hashtable() {
    hashtable<int, int> h, w;

    const int lambda = 1023;

    for (int i = 0; i < 100; i++) {
        h[i*lambda] = i;
    }

    for (int i = 0; i < 100; i++) {
        assert(h.count(i*lambda) == 1);
        assert(h[i*lambda] == i);
    }

    for (int i = 0; i < 100; i++) {
        h[i * lambda] += lambda;
    }

    for (int i = 0; i < 100; i++) {
        assert(h[i*lambda] == i + lambda);
    }

    for (int i = 101; i < 300; i++) {
        assert(h.count(i*lambda) == 0);
        assert(h[i*lambda] == 0);
        assert(h.count(i*lambda) == 1);
    }

    for (int i = 0; i < 100; i++) {
        h.erase(i*lambda);
        assert(h.count(i*lambda) == 0);
    }

    w = h;

    for (int i = 101; i < 300; i++) {
        assert(h.count(i * lambda) == 1);
    }

    h.clear();

    for (int i = 101; i < 300; i++)
        assert(h.count(i * lambda) == 0);

    for (int i = 101; i < 300; i++) {
        assert(w.count(i * lambda) == 1);
    }

    hashtable<pair<int, int>, int> hpii;

    for (int i = 1; i <= 500; i++) {
        hpii[make_pair(i, 500 - i + 1)] += i * (500 - i + 1);
    }

    for (int i = 1; i <= 500; i++) {
        assert(hpii.count(make_pair(500 - i + 1, i)) == 1);
        assert(hpii.count(make_pair(i, i)) == 0);
        assert(hpii.count(make_pair(i, 500 - i + 1)) == 1);
        assert(hpii[make_pair(i, 500 - i + 1)] == i * (500 -i + 1));
    }

    for (hashtable<pair<int, int>, int>::iterator it = hpii.begin();
         it != hpii.end(); ++it) {
        assert(it->first.first + it->first.second == 501);
    }

    const hashtable<pair<int, int>, int>& hpiiref = hpii;

    for (hashtable<pair<int, int>, int>::const_iterator it = hpiiref.begin();
         it != hpiiref.end(); ++it) {
        assert(it->first.first + it->first.second == 501);
    }
}

int main() {
    test_int_hashtable();

    return 0;
}
