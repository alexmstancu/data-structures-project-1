#include "vector.h"
#include "Bon.h"

#include <iostream>
#include <ctime>
#include <stdlib.h> 

using namespace std;

struct Custom {
    int A;
    std::string B;

    Custom() {}

    bool operator<(const struct Custom& other) const {
        if (A == other.A)
            return B < other.B;
        return A < other.A;
    }
};

std::string repeat(char c, int times) {
    return std::string(times, c);
}

int main() {
    const int n = 100000;
    vector<Custom> v;
    for (int i = 1; i <= n; i++) {
        Custom custom;
        custom.A = i % 2;
        custom.B = repeat('a' + (i % 26), i % 100 + 1);
        v.push_back(custom);
    }

    v.sort();

    for (int i = 0; i < n - 1; i++) {
        assert(!(v[i+1] < v[i]));
    }


// sort for tm
    vector<Bon> bonuri;
    for (int i = 1; i <= n/2; i++) {
        Bon custom;
		struct tm time;
        time.tm_sec = rand() % 60;		
        time.tm_min = rand() % 60;
		custom.set_time(time);
        bonuri.push_back(custom);
    }

    bonuri.sort();

    for (int i = 0; i < n/2 - 1; i++) {
        assert(!(bonuri[i+1] < bonuri[i]));
    }

    return 0;
}
