#ifndef VECTOR_H
#define VECTOR_H

#include <cassert>
#include <cstddef>

/* Usage:
 *
 * Declare a vector.
 * vector<int> v;
 *
 * v.push_back(x) inserts element at the end of v.
 * v.push_front(x) inserts element at the beginning of v. Time complexity for this operation is linear in the number of
 * elements in v. So... do not use it unless you have a really good reason.
 * v.pop_back() deletes element at the end of v.
 * v.pop_front() deletes element at the beginning of v. Same time complexity as push_front.
 * v.clear() clears the entire vector.
 * v.size() returns the number of elements in v.
 * v.remove_all(x) removes all elements equal to x.
 * v.remove_from_index(x) removes element at index x.
 * v.remove_element(x) removes the first occurence of an element equal to x.
 *
 * v.front() returns a reference to the first element.
 * v.back() returns a reference to the last element.
 *
 * Iterating over all elements in a vector:
 *  for (int i = 0; i < v.size(); i++) {
 *      int element = v[i];
 *      // Do stuff with v[i]
 *  }
 *
 */

template <typename T>
class vector {
    private:

        std::size_t size_;
        std::size_t capacity;
        T *vect;

    public:

        vector(int capacity = 1) {
            size_ = 0;
            this->capacity = capacity;
            vect = new T[capacity];
        }

        vector(const vector& other) {
            size_ = other.size_;
            capacity = other.capacity;
            vect = new T[capacity];

            for (std::size_t i = 0; i < size_; i++)
                vect[i] = other.vect[i];
        }

        vector& operator=(const vector& other) {
            delete[] vect;

            size_ = other.size_;
            capacity = other.capacity;
            vect = new T[capacity];

            for (std::size_t i = 0; i < size_; i++)
                vect[i] = other.vect[i];
            return *this;
        }

        //Adds an element to the end of the vector

        void push_back(T elem) {
            // If vect is full, double its capacity
            if (size_ == capacity) {
                double_capacity();
            }

            vect[size_] = elem;
            size_++;
        }

        //Adds an element to the beginning of the vector

        void push_front(T elem) {
            //If vector is empty
            if (!size_) {
                vect[size_] = elem;
                return;
            }

            if (size_ + 1 == capacity) {
                double_capacity();
            }

            for (std::size_t i = size_ + 1; i >= 1; i--) {
                vect[i] = vect[i - 1];
            }

            size_++;
            vect[0] = elem;
        }

        void pop_back() {
            size_--;
            if (size_ < (capacity+1)/2)
                shrink();
        }

        void pop_front() {
            for (std::size_t i = 1; i < size_; i++) {
                vect[i - 1] = vect[i];
            }

            size_--;
            if (size_ < (capacity+1)/2)
                shrink();
        }

        //Returns first element

        T& front() {
            assert(size_ != 0);
            return vect[0];
        }

        const T& front() const {
            assert(size_ != 0);
            return vect[0];
        }

        //Returns last element

        T& back() {
            assert(size_ != 0);
            return vect[size_ - 1];
        }

        const T& back() const {
            assert(size_ != 0);
            return vect[size_ - 1];
        }

        T& operator[](int idx) {
            return vect[idx];
        }

        const T& operator[](int idx) const {
            return vect[idx];
        }

        void remove_from_index(int index) {
            if (index < 0)
                return;

            for (std::size_t j = index; j + 1 < size_; j++) {
                vect[j] = vect[j + 1];
            }
            size_--;

            if (size_ < (capacity+1)/2)
                shrink();
        }

        //Removes all elements identical to elem

        void remove_all(T elem) {
            for (std::size_t i = 0; i < size_; i++) {
                if (vect[i] == elem) {
                    remove_from_index(i);
                    i--;
                }
            }
        }

        void remove_element(T elem) {
            for (std::size_t i = 0; i < size_; i++) {
                if (vect[i] == elem) {
                    remove_from_index(i);
                    break;
                }
            }
        }

        void sort() {
            mergesort_(0, size() - 1);
        }

        size_t size() const {
            return size_;
        }


        void clear() {
            while (size()) pop_back();
        }

        ~vector() {
            delete[] vect;
        }

    private:

        void shrink() {
            T *tmp = new T[(capacity+1)/2];

            for (size_t i = 0; i < size_; i++) {
                tmp[i] = vect[i];
            }

            capacity = (capacity+1)/2;
            delete[] vect;
            vect = tmp;
        }

        void double_capacity() {
            T *tmp = new T[2 * capacity];
            for (size_t i = 0; i < size_; i++) {
                tmp[i] = vect[i];
            }

            delete[] vect;
            vect = tmp;
            capacity *= 2;
        }

        void mergesort_(int left, int right) {
            if (left == right) {
                return;
            }

            int mid = left + (right - left) / 2;

            mergesort_(left, mid);
            mergesort_(mid + 1, right);

            combine_(left, mid, right);
        }

        void combine_(int left, int mid, int right) {
            // TODO: in-place mergesort maybe?
            vector<T> w;

            for (int i = left, j = mid + 1; i <= mid || j <= right; ) {
                if (i == mid + 1) {
                    w.push_back(vect[j]);
                    j++;
                } else if (j == right + 1) {
                    w.push_back(vect[i]);
                    i++;
                } else if (!(vect[j] < vect[i])) {
                    w.push_back(vect[i]);
                    i++;
                } else {
                    w.push_back(vect[j]);
                    j++;
                }
            }

            for (int i = left; i <= right; i++) {
                vect[i] = w[i - left];
            }
        }
};

#endif // VECTOR_H
