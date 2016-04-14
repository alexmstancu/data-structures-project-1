#ifndef MAGAZIN_H
#define MAGAZIN_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <ctime>

#include "Bon.h"
#include "parser.h"
#include "vector.h"
#include "hashtable.h"
#include "pair.h"

class Magazin {

    private:
        std::string id;
        std::string name;

        vector<Bon> bonuri;

        hashtable<std::string, int> id2index; // from id to index in vector<Bon> bonuri.


    public:
        Magazin() {
        }

        Magazin(std::string id_, std::string name_) :
            id(id_), name(name_) {
        }

        Magazin(const Magazin& rhs) :
            id(rhs.id), name(rhs.name), bonuri(rhs.bonuri) {
        }

        Magazin operator=(const Magazin& rhs) {
            id = rhs.id;
            name = rhs.name;

            return *this;
        }

        // getters

        int get_number_of_bonuri() const {
            return bonuri.size();
        }

        const vector<Bon>& get_bonuri() const {
            return bonuri;
        }

        void add_bon(Bon b) {
            bonuri.push_back(b);
            id2index[b.get_id()] = bonuri.size() - 1;
        }

        const Bon* get_bon(std::string id) {
            if (id2index.count(id) == 0)
                return NULL;
            return &bonuri[id2index[id]];
        }

        std::string get_id() const {
            return id;
        }


        std::string get_name() const {
            return name;
        }


		void sort_bonuri() {
			bonuri.sort();
		}


        int sales_value() const {
            int sum = 0;
            for (int i = 0; i < (int)bonuri.size(); i++) {
                sum += bonuri[i].cost();
            }

            return sum;
        }

        pair<int,int> second_checkout_per_store() const {
            // pentru a afla cati clienti ar beneficia de o a doua casa pe ora intr-un magazin

            int sec,nr_hours = 0,nr_custom = 0,nr_days = 0;
            vector <pair<int,int> > queueperday;
            hashtable <int,vector<pair<int,int> > > day_clients;

            //day_clients e un hashtable care are ca si cheie ziua din an si ca si valoare un vector de perechi(nr produse, nr secunde trecute de la miezul noptii din ziua respectiva

            for (int i = 0; i < (int)bonuri.size(); i++) {
                sec = (bonuri[i].get_time().tm_hour)*3600 + bonuri[i].get_time().tm_min*60 + bonuri[i].get_time().tm_sec;
                day_clients[bonuri[i].get_time().tm_yday].push_back(::make_pair(bonuri[i].get_number_of_products(),sec));
            }

            //pentru fiecare zi copiem vectorul sau de perechi in queueperday

            for (hashtable <int,vector<pair<int,int> > > :: iterator it = day_clients.begin(); it != day_clients.end(); ++it){
                nr_days++;
                queueperday = it->second;

           //ordonam crescator dupa nr de secunde trecute pentru fiecare bon, adica in ordine cronologica

                int ok = 0;
                while (ok == 0){
                    ok = 1;
                    for (int j = 0; j + 1 < (int)queueperday.size(); j++)
                        if (queueperday[j].second > queueperday[j+1].second) {
                            pair <int,int> aux;
                            aux = queueperday[j];
                            queueperday[j] = queueperday[j+1];
                            queueperday[j+1] = aux;
                            ok = 0;
                        }
                  }
                int aftermax,i = 0,n = 0;
                float maxcoef = 0;
                //parcurgem vectorul de bonuri din ziua respeciva
                while(i < (int)queueperday.size()){
                    while ((queueperday[i].second - queueperday[n].second <= 3600) && (i < (int)queueperday.size()))
                        i++;

                    i--;
                    int aux;
                    aux = i;
                    i = n;
                    n = aux;
                    //gasim intervalul (i,n) ce corespunde unei ore
                    nr_hours++;
                    while (i < n){

                    //considerand ca daca mutam un client din coada vor beneficia toti cei de dupa, iar mutand un altul dupa el ar beneficia acelasi numar de clienti in acea ora, cautam clientul din fiecare ora care are cele mai multe produse, cei mai multi oameni dupa el si cea mai mica diferenta dintre el si urmatorul

                        int after = n-i;//nr de clienti din acea ora din spatele lui
                        int between = queueperday[i+1].second - queueperday[i+1].first - 10 - queueperday[i].second;

                        //diferenta dintre momentul cand termina el si momentul cand incepe urmatorul, considerand ca e necesara 1 secunda/produs si 10 secunde la final plata

                        if(between <= 0)
                            between = 1;
                        float coef = (queueperday[i].first * after)*1.0 / between; // cat de mult participa el la aglomerarea cozii
                        if (coef > maxcoef) { //cautam coeficientul maxim/ora, adica acel client pe care il mutam
                            maxcoef = coef;
                            aftermax = after; //cati clienti beneficiaza daca e mutat clientul cu coeficientul cel mai mare
                        }
                        i++;
                    }
                    nr_custom += aftermax;
                    i++;
                    n++;
                }
            }

         //facem asta pentru fiecare ora din zi si pentru fiecare zi din an
         //nr_custom returneaza numarul total de clienti beneficiari/an ai magazinului si nr_hours numarul total de ore pentru care s-a facut calculul
        //se returneaza cati clienti beneficiaza in medie pe ora si in medie pe zi de introducerea casei
            pair <int,int> rez;
            rez = ::make_pair(nr_custom/nr_hours, nr_custom/nr_days);
            return rez;

        }


        void sales_by_product_pair(hashtable<pair<std::string, std::string>, int>& h) const {
            for (int i = 0 ; i < (int)bonuri.size(); i++) {
                bonuri[i].sales_by_product_pair(h);
            }
        }

        pair<std::string,std::string> best_categories_per_store() const {
            hashtable<std::string, int> h;
            pair <std::string,std::string> rmax;
            int max = 0;

            //actualizeaza hashtable-ul ce contorizeaza cat de multe produse dintr-o categorie au fost vandute pentru toate bonurile din magazin:
            for (int i = 0; i < (int)bonuri.size(); i++) {
                bonuri[i].sales_by_category(h);
            }

            for (hashtable<std::string,int>::iterator it = h.begin(); it != h.end(); ++it) {
                if (it->second > max) {
                    rmax.first = it->first;
                    max = it->second;
                }
            }

            rmax.second = this->name;
            return rmax;
        }

        void sales_by_category(hashtable<std::string, int>& h) {
            for (int i = 0; i < (int)bonuri.size(); i++){
                bonuri[i].sales_by_category(h);
            }
        }

        pair<vector<std::string>, int> best_days_sales_per_store(bool customers = false) const {
            hashtable<int, pair<std::string, int> > h;
            for( int i = 0; i < (int)bonuri.size(); i++){
                int value;

                if (customers)
                    value = 1;
                else
                    value = bonuri[i].get_number_of_products();

                if (h.count(bonuri[i].day_and_year()) == 0) {
                    h[bonuri[i].day_and_year()] = ::make_pair(bonuri[i].prettydate(), value);
                    continue;
                }

                h[bonuri[i].day_and_year()].second += value;
            }

            // parcurgem hashtable-ul si construim vectorul max_sales care va contine doar 
            // zilele in care s-au vandut cele mai multe produse.
            vector<std::string> max_sales;
            int max = -1;
            for (hashtable<int, pair<std::string, int> >::iterator it = h.begin(); it != h.end(); ++it) {
                if (it->second.second > max) {
                    max = it->second.second;
                    max_sales.clear();
                    max_sales.push_back(it->second.first);
                } else if (it->second.second == max) {
                    max_sales.push_back(it->second.first);
                }
            }

            return ::make_pair(max_sales, max);
        }
};

#endif /* MAGAZIN_H */
