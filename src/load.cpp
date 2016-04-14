#include <string>
#include <iostream>
#include <cassert>

#include "Companie.h"
#include "Produs.h"
#include "constants.h"
#include "parser.h"
#include "vector.h"
#include "hashtable.h"
#include "stack.h"

using std::string;

hashtable<string, Produs> produse;
hashtable<string, string> id2categ;
hashtable<string, string> categ2id;
hashtable<string, vector<int> > produs2paleti;

void load_produse(string path) {
    Parser p(path);

    string id, categ, name, dummy;
    int price;

    /* Read table header. */
    p >> dummy >> dummy >> dummy >> dummy;

    while (p) {
        p >> id >> name >> categ >> price;
        if (name == "")
            continue;
        produse[id] = Produs(id, name, categ, price);
    }
}

void load_categorii(string path) {
    Parser p(path);

    string id, name, dummy;

    /* Read table header. */
    p >> dummy >> dummy;

    while (p) {
        p >> id >> name;
        if (name == "")
            continue;

        id2categ[id] = name;
        categ2id[name] = id;
    }
}

void load_magazine(Companie& comp, string path) {
    Parser p(path);

    string id, nume, dummy;

    /* Read table header. */
    p >> dummy >> dummy;

    while (p) {
        p >> id >> nume;
        if (id == "") continue;

        comp.magazine.push_back(Magazin(id, nume));
    }
}

void load_bonuri(Companie& comp, string bonuri_path, string tranz_path) {
    Parser p(bonuri_path);
    hashtable<string, vector<pair<string, int> > > bonuri;
    string id_bon, id_produs, dummy;

    /* Read table header. */
    p >> dummy >> dummy;

    while (p) {
        p >> id_bon >> id_produs;
        if (id_bon == "")
            continue;

        bool found = 0;
        for (int i = 0; i < (int)bonuri[id_bon].size(); i++) {
            if (bonuri[id_bon][i].first == id_produs) {
                bonuri[id_bon][i].second++;
                found = 1;
                break;
            }
        }

        if (!found)
            bonuri[id_bon].push_back(::make_pair(id_produs, 1));
    }

    /* Read tranzactii.csv, associate timestamp & store. */
    Parser q(tranz_path);

    q >> dummy >> dummy >> dummy >> dummy;
    string id_tranz, timestamp, id_magazin;

    while (q) {
        q >> id_tranz >> timestamp >> id_bon >> id_magazin;
        if (id_tranz == "")
            continue;

        // This should never happen.
        assert(comp.get_magazin(id_magazin) != NULL);

        comp.get_magazin(id_magazin)->add_bon(Bon(id_bon, timestamp, bonuri[id_bon], id_magazin));
    }

	/* sort bonuri pt fiecare magazin*/
	for (int i = 0; i < (int)comp.magazine.size(); i++)
		comp.magazine[i].sort_bonuri();
	
	
}

void load_paleti(Companie& comp, string path) {
    Parser p(path);
    string dummy, id_palet, prod_type;
    int n_items, slot;


    /* Read table header. */
    p >> dummy >> dummy >> dummy >> dummy;

    while (p) {
        p >> id_palet >> prod_type >> n_items >> slot;

        if (id_palet == "")
            continue;

        comp.depozit.add_palet(slot, Palet(id_palet, prod_type, n_items));
        produs2paleti[prod_type].push_back(n_items);
    }

    for (hashtable<string, vector<int> >::iterator it = produs2paleti.begin(); it != produs2paleti.end(); ++it) {
        it->second.sort();
    }

}

Companie load(string input_dir) {
    Companie comp;

    load_produse(input_dir + "/produse.csv");
    load_categorii(input_dir + "/categorii.csv");
    load_magazine(comp, input_dir + "/magazine.csv");
    load_bonuri(comp, input_dir + "/bonuri.csv", input_dir + "/tranzactii.csv");
    load_paleti(comp, input_dir + "/paleti.csv");

    /* TODO */

#if DEBUG
    for (hashtable<string, Produs>::iterator it = produse.begin(); it != produse.end(); ++it) {
        std::cout << it->first << ' ' << it->second.id << ' ' << it->second.name << ' ' << it->second.categ << ' ' << it->second.price << id2categ[it->second.categ] << '\n';
    }

    for (hashtable<string, string>::iterator it = id2categ.begin(); it != id2categ.end(); ++it) {
        std::cout << it->first << " " << it->second << " " << categ2id[it->second] << "\n";
    }

    for (int i = 0; i < (int)comp.magazine.size(); i++) {
        std::cout << comp.magazine[i].get_id() << " " << comp.magazine[i].get_name() << " " << comp.magazine[i].get_number_of_bonuri() << '\n';
    }
#endif

    return comp;
}
