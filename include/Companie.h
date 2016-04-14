#ifndef COMPANIE_H
#define COMPANIE_H

#include "Magazin.h"
#include "Depozit.h"
#include "vector.h"
#include "hashtable.h"
#include "pair.h"
#include <iostream>

extern hashtable<std::string, Produs> produse;
extern hashtable<std::string, vector<int> > produs2paleti;

class Companie {
    public:
        vector<Magazin> magazine;
        Depozit depozit;

        Magazin* get_magazin(std::string id) {
            for (int i = 0; i < (int)magazine.size(); i++) {
                if (magazine[i].get_id() == id)
                    return &magazine[i];
            }

            return NULL;
        }

        vector<pair<std::string,int> > get_bill_content(std::string id_bon, std::string id_mag) {
        //pentru un id de bon si un id de magazin intoarce un vector cu id-urile produselor de pe bon si cantitatea
            for (int i = 0; i < (int)magazine.size(); i++) {
                if (magazine[i].get_id() == id_mag) {
                    const Bon* bonptr = magazine[i].get_bon(id_bon);
                    if (bonptr == NULL) continue;
                    return bonptr->get_produse();
                }
            }

            return vector<pair<std::string, int> >(); // empty vector.
        }

        //metoda pentru categoriile de produse care se vand cel mai bine pentru fiecare magazin:
        vector <pair<std::string,std::string> > best_categories(){
            vector <pair<std::string,std::string> > best_categ;
            for (int i = 0; i < (int)magazine.size(); i++)
                best_categ.push_back(magazine[i].best_categories_per_store());
            return best_categ;
        }

        pair <int,int> second_checkout(std::string name){
            for (int i = 0; i < (int)magazine.size(); i++) {

                if (magazine[i].get_name() == name)
                    return magazine[i].second_checkout_per_store();
            }

            return pair<int, int>();
        }

        vector<pair<std::string, double> > val_cos_med() const {
            vector<pair<std::string, double> > rez;
            int total_sales = 0, count_sales = 0;
            int sales, count;
            std::string overall = "Overall";

            for (int i = 0; i < (int)magazine.size(); i++) {
                sales = magazine[i].sales_value();
                count = magazine[i].get_number_of_bonuri();

                total_sales += sales;
                count_sales += count;
                rez.push_back(::make_pair(magazine[i].get_name(), (double)sales / count));
            }

            rez.push_back(::make_pair(overall, (double)total_sales / count_sales));

            return rez;
        }

        vector<pair<std::string, int> > total_sales_per_store() const {
            vector<pair<std::string, int> > total_sales;

            for (int i = 0; i < (int)magazine.size(); i++) {
                int sum = 0;
                for (int j = 0; j < (int)magazine[i].get_number_of_bonuri(); j++) {
                    sum += magazine[i].get_bonuri()[j].cost();
                } // computing total sales value for the current store
                total_sales.push_back(::make_pair(magazine[i].get_name(), sum));
            } // finished iterating through all stores

            return total_sales;
        }

        hashtable<std::string, int> sales_val_per_prod() const {
            hashtable <std::string, int> total_sales_per_prod;

            // initialize hashtable to 0
            for (hashtable<std::string, Produs>::iterator it = produse.begin(); it != produse.end(); ++it)
                total_sales_per_prod[it->first] = 0;

            for (int i = 0; i < (int)magazine.size(); i++) {
                for (int j = 0; j < (int)magazine[i].get_number_of_bonuri(); j++) {
                    for (int k = 0; k < (int)magazine[i].get_bonuri()[j].get_produse().size(); k++) {
                        std::string name = magazine[i].get_bonuri()[j].get_produse()[k].first;
                        int count = magazine[i].get_bonuri()[j].get_produse()[k].second;
                        total_sales_per_prod[name] += count * produse[name].price;
                    } // adding to the hashtable, iterating through all the products on the current bill in the current store
                } // finished iterating through all the bills in the current store
            } // finished iterating through all stores

            return total_sales_per_prod;
        }

        //metoda pentru perechile de produse care se vand cel mai bine impreuna:

    private:
        struct category_pair {
            std::string id1, id2;
            int value;

            category_pair() {}

            category_pair(std::string id1_, std::string id2_, int value_) :
                id1(id1_), id2(id2_), value(value_) {
            }

            inline bool operator<(const category_pair& rhs) const {
                if (value == rhs.value) {
                    if (id1 == rhs.id1)
                        return id2 < rhs.id2;
                    return id1 < rhs.id1;
                }
                return value > rhs.value;
            }

        };

    public:

        vector< pair <pair<std::string,std::string>,int> > best_pairs(int x) const {
            hashtable<pair<std::string, std::string>,int > h;
            vector<category_pair> perechi;
            pair<std::string,std::string> auxp;

            //pentru fiecare magazin se actualizeaza hashtable-ul cu numarul de aparitii al fiecarei perechi:

            for(int i = 0 ; i < (int)magazine.size() ; i++){
                magazine[i].sales_by_product_pair(h);
            }

            //se retin toate perechile intr-ul vector care va fi ordonat apoi descrescator dupa valoarea din hashtable,astfel incat la final vectorul perechi va retine perechile de id -uri de produse ordonate descrescator dupa cat de des apar in tranzactii:

            for (hashtable<pair<std::string, std::string>,int>::iterator it = h.begin(); it != h.end(); ++it){
                 perechi.push_back(category_pair(it->first.first,it->first.second,it->second));
            }

            perechi.sort();

            // se pastreaza doar perechile de pe primele X pozitii
            vector< pair <pair<std::string, std::string>,int> > ans;

            for (int i = 0; i < (int)perechi.size() && i < x; i++)
                ans.push_back(::make_pair(::make_pair(perechi[i].id1, perechi[i].id2), perechi[i].value));

            return ans;
        }


        vector<pair<std::string, pair<vector<std::string>, int> > > best_days_sales(bool customers = false) const {
            vector<pair<std::string, pair<vector<std::string>, int> > > ans;
            for (int i = 0; i < (int)magazine.size(); i++) {
                ans.push_back(::make_pair(magazine[i].get_name(), magazine[i].best_days_sales_per_store(customers)));
            }

            return ans;
        }

        pair<pair<std::string, int>, std::string> task_12() const {
            hashtable<pair<std::string, std::string>, int> init;
            hashtable<pair<std::string, std::string>, int> quant;

            vector<Bon> bonuri;

            for (int i = 0; i < (int)magazine.size(); i++) {
                for (int j = 0; j < (int)magazine[i].get_bonuri().size(); j++) {
                    bonuri.push_back(magazine[i].get_bonuri()[j]);
                }
            }

            bonuri.sort();
            hashtable<std::string, vector<int> > paleti = produs2paleti;

            for (int i = 0; i < (int)magazine.size(); i++) {
                for (hashtable<std::string, vector<int> >::iterator it = paleti.begin(); it != paleti.end(); ++it) {
                    if (it->second.size() == 0) {
                        return ::make_pair(::make_pair(std::string(""), -1), std::string("")); // nu s-a putut gasi macar 1 palet din fiecare magazin initial.
                    }
                    init[::make_pair(magazine[i].get_id(), it->first)] = it->second.back();
                    quant[::make_pair(magazine[i].get_id(), it->first)] = it->second.back();
                    it->second.pop_back();
                }
            }

            for (int i = 0; i < (int)bonuri.size(); i++) {
                std::string id_bon = bonuri[i].get_id();
                std::string id_magazin = bonuri[i].id_magazin;
                for (int j = 0; j < (int)bonuri[i].get_produse().size(); i++) {
                    std::string id_produs = bonuri[i].get_produse()[j].first;
                    int quantity = bonuri[i].get_produse()[j].second;

                    // incerc sa aduc din depozit pana satisfac cererea clientului
                    while (quant[::make_pair(id_magazin, id_produs)] < quantity && paleti[id_produs].size() != 0) {
                        quant[::make_pair(id_magazin, id_produs)] += paleti[id_produs].back();
                        paleti[id_produs].pop_back();
                    }

                    if (quant[::make_pair(id_magazin, id_produs)] < quantity) {
                        return ::make_pair(::make_pair(id_bon, 0), produse[id_produs].name); // nu s-a putut satisface cererea cu id-ul id_bon
                    } else {
                        quant[::make_pair(id_magazin, id_produs)] -= quantity;
                        // incerc sa aduc din depozit cat timp cantitatea e < 10% din cantitatea initiala
                        if (quant[::make_pair(id_magazin, id_produs)] * 10 < init[::make_pair(id_magazin, id_produs)] && paleti[id_produs].size() != 0) {
                            quant[::make_pair(id_magazin, id_produs)] += paleti[id_produs].back();
                            paleti[id_produs].pop_back();
                        }
                    }
                }
            }

            return ::make_pair(::make_pair(std::string(""), 1), std::string(""));
        }
};

#endif /* COMPANIE_H */
