#ifndef DEPOZIT_H
#define DEPOZIT_H

#include "stack.h"
#include "vector.h"
#include "Slot.h"
#include "Palet.h"
#include <iostream>
#include "hashtable.h"

class Depozit {
    private:
    vector<Slot> sloturi;

    public:
    Depozit() {
    }

    Depozit(const Depozit& rhs) :
        sloturi(rhs.sloturi) {
    }

    Depozit& operator=(const Depozit& rhs) {
        sloturi = rhs.sloturi;

        return *this;
    }

    ~Depozit() {

    }

    void add_palet(int index, Palet p) {
        // expand sloturi to required size.
        while ((int)sloturi.size() < index + 1)
            sloturi.push_back(Slot());

        sloturi[index].push(p);
    }

    int first_slot_that_contains_type(const std::string& type) {
        for (int i = 1; i < (int)sloturi.size(); i++) {
            if (sloturi[i].has_prod(type))
                return i;
        }

        return -1;
    }

    pair<pair<int, int>, int> pallet_moves(const std::string& type) {
        int position = -1;
        int to = -1;

        // luam slotul pe care paletul de tipul cerut se afla cel mai sus
        // si mutam de acolo pe un alt slot, ales la intamplare, diferit de cel de pe care luam.

        int min = 0x3f3f3f3f;
        int minSize = 0x3f3f3f3f;

        for (int i = 1; i < (int)sloturi.size(); i++) {
            if (!sloturi[i].has_prod(type))
                continue;

            int dist = sloturi[i].get_dist(type);
            if (dist < min) {
                min = dist;
                position = i;
            }
        }

        if (position == -1)
            return pair<pair<int, int>, int>(pair<int, int>(), -1); // no moves available

        for (int i = 1; i < (int)sloturi.size(); i++) {
            if (i == position) continue;
            if ((int)sloturi[i].get_stack().size() < minSize) {
                minSize = sloturi[i].get_stack().size();
                to = i;
            }
        }

        if (to == -1)
            return pair<pair<int, int>, int>(pair<int, int>(), -1); // no moves available

        return pair<pair<int, int>, int>(pair<int, int>(position, to), min);
    }
};

#endif
