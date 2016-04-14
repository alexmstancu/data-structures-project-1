#include <iostream>
#include <cassert>
#include "vector.h"

using namespace std;

int main() {
    vector<int> v, w, u;

    for (int i = 1; i <= 15; i++) {
        v.push_back(10);
    }

    for (int i = 1; i <= 15; i++) {
        v.push_back(11);
    }

    v.remove_element(10);
    assert(v[14] == 11);

    /* for (int i = 0; i < v.size(); i++)
        cout << v[i] << ' ';

    cout << endl; */

    w = v;
    assert(w[14] == 11);

    v.remove_all(10);

    int ok = 1;
    for (size_t i = 0; i < v.size(); i++) 
        if(v[i] == 10)
            ok = 0;
    assert(ok == 1);

    v[0] = 1;

    /* for (int i = 0; i < v.size(); i++)
        cout << v[i] << ' ';

    cout << endl;

    for (int i = 0; i < w.size(); i++)
        cout << w[i] << ' ';

    cout << endl; */

    for (size_t i = 0; i < 4; i++)
        v.push_front(i+1);

    assert(v[0] == 4);

    /* for (int i = 0; i < v.size(); i++) {
        cout << v[i] << ' ';
    }

    cout << '\n'; */

    int &x = v.front();
    int &y = v.back();
    x = -1;
    y = -1;
    assert(v.front() == -1);
    assert(v.back() == -1);

    vector<int> vv(v);
    v.pop_back();

    assert(v.size() == vv.size() - 1);

    for (size_t i = 0; i < v.size(); i++)
        assert(v[i] == vv[i]);

    v = vv;
    v.pop_front();

    assert(v.size() == vv.size() - 1);

    for (size_t i = 0; i < v.size(); i++)
        assert(v[i] == vv[i + 1]);

    v.clear();

    assert(v.size() == 0);

    return 0;
}
