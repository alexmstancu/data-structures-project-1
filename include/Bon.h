#ifndef BON_H
#define BON_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "pair.h"
#include "vector.h"
#include "hashtable.h"
#include "Produs.h"

extern hashtable<std::string, Produs> produse;
//extern hashtable<string, Categorie> categs;
//extern hashtable<string, string> categ2id;

static tm time_cp;

class Bon {
    private:
        std::string id;
        vector<pair<std::string, int> > p; // <id, cantitate>
        struct tm time;

    public:
        std::string id_magazin;

        Bon() {

        }

        Bon(std::string id_, std::string timestamp, vector<pair<std::string, int> > produse_, std::string id_magazin_) :
            id(id_), p(produse_), id_magazin(id_magazin_) {
            strptime(timestamp.c_str(), "%Y-%m-%d%t%T", &time);
        }

        Bon(const Bon& rhs) :
            id(rhs.id), p(rhs.p), time(rhs.time), id_magazin(rhs.id_magazin) {
        }

        Bon& operator=(const Bon& rhs) {
            id   = rhs.id;
            p    = rhs.p;
            time = rhs.time;
            id_magazin = rhs.id_magazin;

            return *this;
        }

        int day_and_year() const {
            return time.tm_yday * 366 + time.tm_year;
        }

        std::string prettydate() const {
            const int BUFFER_SIZE = 256;
            char str[BUFFER_SIZE];

            strftime(str, BUFFER_SIZE, "%d %b %Y", &time);

            return std::string(str);
        }

		// bool stuff

		//let me tell you why mktime is a pain in the a**:
		// it can change the date if he thinks it's broken
		// therefore you cant be sure that rhs is not changed
		// so bye bye const

		// fix required: o alta metoda decat creerea unui element nou

        bool operator==(const Bon& rhs) {
			time_cp = rhs.time;
        	return mktime(&time) == mktime(&time_cp);
        }

        bool operator==(const struct tm& rhs) {
			time_cp = rhs;
        	return mktime(&time) == mktime(&time_cp);
        }

        bool operator==(const time_t& rhs) {
        	return mktime(&time) == rhs;
        }

        bool operator<(const Bon& rhs) {
			time_cp = rhs.time;
        	return mktime(&time) < mktime(&time_cp);
        }

        bool operator<(const struct tm& rhs) {
			time_cp = rhs;
        	return mktime(&time) < mktime(&time_cp);
        }

        bool operator<(const time_t& rhs) {
        	return mktime(&time) < rhs;
        }

        bool operator>(const Bon& rhs) {
			time_cp = rhs.time;
        	return mktime(&time) > mktime(&time_cp);
        }

        bool operator>(const struct tm& rhs) {
			time_cp = rhs;
        	return mktime(&time) > mktime(&time_cp);
        }

        bool operator>(const time_t& rhs) {
        	return mktime(&time) > rhs;
        }

        bool operator!=(const Bon& rhs) {
        	return !(*this==rhs);
        }

        bool operator!=(const struct tm& rhs) {
        	return !(*this==rhs);
        }

        bool operator!=(const time_t& rhs) {
        	return !(*this==rhs);
        }

        bool operator<=(const Bon& rhs) {
        	return !(*this>rhs);
        }

        bool operator<=(const struct tm& rhs) {
        	return !(*this>rhs);
        }

        bool operator<=(const time_t& rhs) {
        	return !(*this>rhs);
        }

        bool operator>=(const Bon& rhs) {
        	return !(*this<rhs);
        }

        bool operator>=(const struct tm& rhs) {
        	return !(*this<rhs);
        }

        bool operator>=(const time_t& rhs) {
        	return !(*this<rhs);
        }

		// bool stuff end


        

        ~Bon() {

        }

        int get_number_of_products() const {
            int num = 0;
            for (int i = 0; i < (int)p.size() ; i++)
                num += p[i].second;
            return num;
        }

		void set_time(struct tm& aux) {
            time = aux;
        }

        struct tm get_time() const {
            return this->time;
        }

        const std::string get_id() const{
            return id;
        }

        const vector<pair<std::string, int> >& get_produse() const {
            return p;
        }

        int cost() const {
            int sum = 0;
            for (int i = 0; i < (int)p.size(); i++)
                sum += produse[p[i].first].price * p[i].second;

            return sum;
        }

        //pentru fiecare pereche de produse de pe bon se incrementeaza valoarea din hashtable-ul ce contorizeaza de cate ori apare perechea respectiva in tranzactii:
        void sales_by_product_pair(hashtable<pair<std::string, std::string>, int>& h) const {
            for (int i = 0; i < (int)p.size(); i++)
                for (int j = i + 1; j < (int)p.size(); j++)
                    if (produse[p[i].first].id != produse[p[j].first].id) {
                        // exista p[i].second * p[j].second moduri sa combini produse de tipul p[i].first, p[j].first
                        // pentru a fi siguri ca nu repetam perechi de tipul (x, y) si (y, x)
                        // bagam in h (min(x, y), max(x,y))
                        std::string x = p[i].first < p[j].first ? p[i].first : p[j].first;
                        std::string y = p[i].first > p[j].first ? p[i].first : p[j].first;
                        h[::make_pair(x, y)] += p[i].second * p[j].second;
                    }
        }

        //pentru fiecare produs se incrementeaza valoarea din hashtable-ul ce contorizeaza cat de multe produse dintr-o categorie au fost vandute:
        void sales_by_category (hashtable<std::string, int >& h) const {
            for (int i = 0; i < (int)p.size(); i++) {
                h[produse[p[i].first].categ] += p[i].second;
            }
        }
};

#endif /* BON_H */
