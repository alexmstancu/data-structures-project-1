#ifndef SLOT_H
#define SLOT_H

#include "Palet.h"
#include "stack.h"
#include "hashtable.h"

class Slot {
    private:
        stack<Palet> p;
        hashtable<std::string, int> h;

    public:
        Slot() {
        }

        ~Slot() {
        }

        stack<Palet>& get_stack() {
            return p;
        }

        void push(Palet palet) {
            p.push(palet);
            h[palet.prod] = p.size();
        }

        Palet& top() {
            return p.top();
        }

        const Palet& top() const {
            return p.top();
        }

        bool empty() const {
            return p.empty();
        }

        void pop() {
            p.pop();
        }

        bool has_prod(const std::string& prod) const {
            return h.count(prod) != 0;
        }

        int get_dist(const std::string& prod) {
            return p.size() - h[prod];
        }
};

#endif /* SLOT_H */
