#ifndef PALET_H
#define PALET_H

#include <string>
#include <iostream>

#include "constants.h"
#include "parser.h"

struct Palet {
    std::string id;
    std::string prod;
    int num;

    Palet() {
    }

    Palet(const std::string& id_, const std::string& prod_, int num_) :
        id(id_), prod(prod_), num(num_) {
#if DEBUG
        std::cerr << id << ' ' << prod << ' ' << num << '\n';
#endif
    }

    Palet(const Palet& rhs) :
        id(rhs.id), prod(rhs.prod), num(rhs.num) {
    }

    Palet& operator=(const Palet& rhs) {
        id = rhs.id;
        prod = rhs.prod;
        num = rhs.num;

        return *this;
    }

    ~Palet() {

    }
};

#endif /* PALET_H */
