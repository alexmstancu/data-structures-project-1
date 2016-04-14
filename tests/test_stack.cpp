#include "stack.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    const int N = 10;

    stack<int> st;

    // Test stack pushing

    for (int i = 1; i <= N; i++) {
        st.push(i);
    }

    assert(st.top() == N);
    assert(st.size() == N);

    // Test stack popping
    
    for (int i = 1; i <= N - N / 2; i++) {
        assert(st.top() == N - i + 1);

        st.pop();
    }

    // Test stack copy assignment / constructor

    stack<int> st2, st3(st);

    st2 = st;

    assert(st.size() == st2.size() && st.size() == st3.size());

    while (st.size()) {
        assert(st.top() == st2.top() && st.top() == st3.top());

        st.pop();
        st2.pop();
        st3.pop();
    }

    assert(st.size() == 0 && st2.size() == 0 && st3.size() == 0);

    return 0;
}
