#ifndef STACK_H
#define STACK_H

#include <cassert>
#include <cstddef>
#include "vector.h"

/* Usage:
 *
 * Declare a stack of a certain type.
 * stack<int> st;
 *
 * st.push(x) inserts element x at the top of the stack.
 * st.pop() removes the element at the top of the stack.
 * st.top() returns a reference to the element at the top of the stack.
 * st.size() returns the number of elements in the stack.
 * st.empty() returns true if the stack is empty, false otherwise.
 *
 */

template <typename T>
class stack {
    private:
        struct node {
            T data;
            node *next;

            node(T data_, node *next_ = NULL) : data(data_), next(next_) {
            }
        };

        std::size_t size_;
        node* top_;

        void copy_stack(const stack& other) {
            if (other.size_ == 0)
                return;

            size_ = other.size_;

            top_ = new node(other.top_->data);

            node *crt_this = top_, *crt_other = other.top_->next;

            while (crt_other != NULL) {
                crt_this->next = new node(crt_other->data);
                crt_this = crt_this->next;
                crt_other = crt_other->next;
            }
        }

    public:
        stack() {
            top_ = NULL;
            size_ = 0;
        }

        stack(const stack& other) {
            copy_stack(other);
        }

        stack& operator=(const stack& other) {
            copy_stack(other);

            return *this;
        }

        ~stack() {
            while (!empty())
                pop();
        }

        T& top() {
            assert(!empty());

            return top_->data;
        }

        const T& top() const {
            assert(!empty());

            return top_->data;
        }

        void push(const T& value) {
            node *n = new node(value, top_);
            top_ = n;

            size_++;
        }

        void pop() {
            assert(!empty());

            node *n = top_->next;
            delete top_;
            top_ = n;
            size_--;
        }

        std::size_t size() const {
            return size_;
        }

        bool empty() const {
            return top_ == NULL;
        }
};

#endif /* STACK_H */
