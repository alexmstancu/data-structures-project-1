#include "hashtable.h"
#include <string>

// default_hash_policy<int> implementation
template<>
int default_hash_policy<int>::operator()(const int& key) {
    return key % NUMBER_OF_BUCKETS;
}

// default_hash_policy<string> implementation
// Note; will use base (26 + 26 + 10) to hash the strings. (26 lowercase, 26 uppercase, 10 digits)
template<>
int default_hash_policy<std::string>::operator()(const std::string& key) {
    int hash = 0, base = 62;
    for (size_t i = 0; i < key.size(); i++) {
        // the value of char key[i] in base 62.
        // will map the values of '0' - '9' to 0 - 9
        //                        'a' - 'z' to 10 - 35
        //                        'A' - 'Z' to 36 - 61
        int char_in_base = 0;
        if (key[i] >= '0' && key[i] <= '9') char_in_base = key[i] - '0';
        if (key[i] >= 'a' && key[i] <= 'z') char_in_base = key[i] - 'a' + 10;
        if (key[i] >= 'A' && key[i] <= 'Z') char_in_base = key[i] - 'A' + 36;

        hash = (hash * base + char_in_base) % NUMBER_OF_BUCKETS;
    }

    return hash;
}

template<>
int default_hash_policy<pair<std::string, std::string> >::operator()(const pair<std::string, std::string>& key) {
    std::string _key = key.first + " " + key.second;
    int hash = 0, base = 62;
    for (size_t i = 0; i < _key.size(); i++) {
        // the value of char key[i] in base 62.
        // will map the values of '0' - '9' to 0 - 9
        //                        'a' - 'z' to 10 - 35
        //                        'A' - 'Z' to 36 - 61
        int char_in_base = 0;
        if (_key[i] >= '0' && _key[i] <= '9') char_in_base = _key[i] - '0';
        if (_key[i] >= 'a' && _key[i] <= 'z') char_in_base = _key[i] - 'a' + 10;
        if (_key[i] >= 'A' && _key[i] <= 'Z') char_in_base = _key[i] - 'A' + 36;

        hash = (hash * base + char_in_base) % NUMBER_OF_BUCKETS;
    }

    return hash;
}

// default hash_policy pair<int, int> implementation
template<>
int default_hash_policy<pair<int, int> >::operator()(const pair<int, int>& key) {
    // multiply key.first by a constant prime lower than the number of buckets.
    long long tmp = 1LL * key.first * 829 + key.second;

    tmp %= NUMBER_OF_BUCKETS; // in C++, modulo of negative numbers returns negative number, so we have to account for that.
    if (tmp < 0) tmp += NUMBER_OF_BUCKETS;

    return (int)tmp;
}
