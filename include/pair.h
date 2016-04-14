#ifndef PAIR_H
#define PAIR_H

/* Usage:
 *
 * Create a pair from two variables:
 * int a = 3;
 * char b = 'd';
 * pair<int, char> p = make_pair(a, b);
 *
 * Retrieve the fields from p:
 *
 * p.first == 3;
 * p.second == 'd';
 *
 * You can also check for equality between pairs of the same types via
 * p == u   or   p != u.
 */

template<typename T, typename U>
struct pair {
    T first;
    U second;

    pair(T t, U u) : first(t), second(u) {
    }

    pair(const pair<T, U>& other) : first(other.first), second(other.second) {
    }

    pair<T, U>& operator=(const pair<T, U>& other) {
        first = other.first;
        second = other.second;

        return *this;
    }

    pair() {
    }

    bool operator==(const pair<T, U>& other) const {
        return is_equal(other);
    }

    bool operator!=(const pair<T, U>& other) const {
        return !is_equal(other);
    }

    private:

    bool is_equal(const pair<T, U>& other) const {
        return first == other.first && second == other.second;
    }
};

template<typename T, typename U>
pair<T, U> make_pair(T t, U u) {
    return pair<T, U>(t, u);
}

#endif /* PAIR_H */
