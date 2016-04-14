#ifndef PRODUS_H
#define PRODUS_H

#include <string>

struct Produs {
    std::string id, name, categ;
    int price;

    Produs() {
    }

    Produs(std::string id_, std::string name_, std::string categ_, int price_) :
        id(id_), name(name_), categ(categ_), price(price_) {
    }

    Produs(const Produs& rhs) :
        id(rhs.id), name(rhs.name), categ(rhs.categ), price(rhs.price) {
    }

    Produs& operator=(const Produs& rhs) {
        id = rhs.id;
        name = rhs.name;
        categ = rhs.categ;
        price = rhs.price;

        return *this;
    }
};

#endif /* PRODUS_H */

