#ifndef HASHTABLE_H
#define HASHTABLE_H

/* Usage:
 *
 * Let's assume we want to map integers to strings.
 *
 * Declare the hashtable:
 *      hashtable<int, std::string> h;
 * Check if an element is present in the hashtable:
 *      h.count(125); // will return 0 if not present, 1 if present.
 * Delete an element from the hashtable:
 *      h.erase(125); // will return true on success, false on failure.
 * Map integer 100 to string "asdf":
 *      h[100] = "asdf";
 * Retrieve the value of h[100]:
 *      std::string value = h[100];
 *      if integer 100 is not present in the hashtable, it will get inserted via std::string's default constructor.
 *      Beware if you want to use hashtable with custom types, they need a default constructor defined.
 * Clear the entire hashtable:
 *      h.clear();
 * Iterating over all elements in hashtable:
 *      If the hashtable is const&, use const_iterator.
 *      const hashtable<int, std::string>& hr = h;
 *      for (hashtable<int, std::string>::const_iterator it = hr.begin(); it != hr.end(); ++it) {
 *          it is const pair<int, std::string>*
 *          it->first is int
 *          it->second is std::string
 *      }
 *      Otherwise, use iterator.
 *      hashtable<pair<int, int>, int> hpii;
 *      for (hashtable<int, std::string>::iterator it = h.begin(); it != h.end(); ++it) {
 *          it is pair<int, std::string>*
 *          it->first is int
 *          it->second is std::string
 *      }
 */

#include <string>
#include <cassert>

#include "vector.h"
#include "pair.h"

#define NUMBER_OF_BUCKETS 997

template<typename T>
struct default_hash_policy {
    int operator()(const T& key);
};

template<typename Key, typename Value>
class hashtable {
    private:
        typedef pair<Key, Value> node;

        template<typename _Key, typename _Value, typename _Hashtable, typename _Pair>
        class Iterator {
            private:
                _Hashtable *ht_;
                size_t bucket_;
                size_t position_;

                friend class hashtable<_Key, _Value>;

                Iterator(_Hashtable *ht, int bucket, int position)
                    : ht_(ht), bucket_(bucket), position_(position)  {
                }

                bool equals(const Iterator& other) {
                    return position_ == other.position_ && bucket_ == other.bucket_;
                }

            public:

                Iterator(const Iterator& other)
                    : ht_(other.ht_), bucket_(other.bucket_), position_(other.position_) {
                }

                ~Iterator() {
                }

                void operator++() {
                    position_++;
                    while (position_ == ht_->h[bucket_].size() && bucket_ < ht_->buckets()) {
                        bucket_++;
                        position_ = 0;
                    }
                }

                bool operator==(const Iterator& other) {
                    return equals(other);
                }

                bool operator!=(const Iterator& other) {
                    return !equals(other);
                }

                _Pair* operator->() const {
                    return &ht_->h[bucket_][position_];
                }
        };

        vector<node> h[NUMBER_OF_BUCKETS];

        size_t buckets() const {
            return NUMBER_OF_BUCKETS;
        }

        Value& value_find(const Key& key) {
            int hash = default_hash_policy<Key>()(key);

            for (size_t i = 0; i < h[hash].size(); i++) {
                if (h[hash][i].first == key)
                    return h[hash][i].second;
            }

            assert(false); // should never reach this point.
        }

        void insert_default(const Key& key) {
            int hash = default_hash_policy<Key>()(key);

            h[hash].push_back(node(key, Value()));
        }

    public:
        typedef Iterator<Key, Value, hashtable<Key, Value>, pair<Key, Value> > iterator;
        typedef Iterator<Key, Value, const hashtable<Key, Value>, const pair<Key, Value> > const_iterator;

        hashtable() {

        }

        ~hashtable() {
            for (size_t i = 0; i < buckets(); i++) {
                h[i].clear();
            }
        }

        hashtable(const hashtable<Key, Value>& other) {
            for (size_t i = 0; i < buckets(); i++) {
                h[i] = other.h[i];
            }
        }

        hashtable& operator=(const hashtable<Key, Value>& other) {
            for (size_t i = 0; i < buckets(); i++) {
                h[i] = other.h[i];
            }

            return *this;
        }

        Value& operator[](const Key& key) {
            if (!count(key))
                insert_default(key);

            return value_find(key);
        }

        const Value& operator[](const Key& key) const {
            if (!count(key))
                insert_default(key);

            return value_find(key);
        }

        int count(const Key& key) const {
            int hash = default_hash_policy<Key>()(key);

            for (size_t i = 0; i < h[hash].size(); i++) {
                if (h[hash][i].first == key)
                    return 1;
            }

            return 0;
        }

        bool erase(const Key& key) {
            if (!count(key))
                return false;

            int hash = default_hash_policy<Key>()(key);

            for (size_t i = 0; i < h[hash].size(); i++) {
                if (h[hash][i].first == key) {
                    h[hash].remove_from_index(i);
                    return true;
                }
            }

            return false;
        }

        void clear() {
            for (size_t i = 0; i < buckets(); i++)
                h[i].clear();
        }

        iterator begin() {
            size_t first_bucket = 0;
            while (h[first_bucket].size() == 0 && first_bucket < buckets())
                first_bucket++;
            return iterator(this, first_bucket, 0);
        }

        iterator end() {
            return iterator(this, buckets(), 0);
        }

        const_iterator begin() const {
            size_t first_bucket = 0;
            while (h[first_bucket].size() == 0 && first_bucket < buckets())
                first_bucket++;
            return const_iterator(this, first_bucket, 0);
        }

        const_iterator end() const {
            return const_iterator(this, buckets(), 0);
        }
};

#endif /* HASHTABLE_H */
