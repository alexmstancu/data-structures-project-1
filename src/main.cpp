#include <iostream>
#include <cstdlib>
#include <string>

#include "../include/constants.h"
#include "../include/Companie.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

extern Companie load(string input_dir);
extern hashtable<string, string> id2categ;

extern void open_files();
extern void close_files();
extern void copy_file(std::ifstream& in, std::ofstream& out);
extern std::ifstream header;
extern std::ifstream middle_1;
extern std::ifstream middle_2;
extern std::ifstream footer;
extern void plot_hashtable(std::ofstream& chart, hashtable<std::string, int>& h, hashtable<std::string, Produs>& p, std::string chart_name);
extern void plot_vector_int(std::ofstream& chart, vector<pair<std::string, int> >& v, std::string chart_name);
extern void plot_vector_double(std::ofstream& chart, vector<pair<std::string, double> >& v, std::string chart_name);

void exit_message() {
    cout << "Thanks for using our application." << endl;
}

void nyi() {
    cout << "This option is not implemented yet." << endl;
}

void menu() {
    cout << "\tThese are the options:" << endl << endl;
    cout << "\t 1) Show me a list of all stores and their sales" << endl;
    cout << "\t 2) Show me a list of all products and the corresponding transactions" << endl;
    cout << "\t 3) Show me the value of the average shopping basket" << endl;
    cout << "\t 4) Show me the best selling categories of products" << endl;
    cout << "\t 5) Show me the products that are sold best together" << endl;
    cout << "\t 6) Show me the days with top sales" << endl;
    cout << "\t 7) Show me the days with the largest number of clients" << endl;
    cout << "\t 8) Show me the content of a bill" << endl;
    cout << "\t 9) Show me how many clients would benefit from a second checkout" << endl;
    cout << "\t10) Show me on which slot is a certain type of product in the deposit" << endl;
    cout << "\t11) Show me the necessary moves in order to get a certain type of product" << endl;
    cout << "\t12) Show me the first request from the deposit that can't be fulfilled" << endl;
    cout << "\t 0) Exit "<< endl;
    cout << "\tPick an option:" << endl;
}

int main(int argc, char * argv[]) {
    int N,x;
    string  id,s, store;
    char option;

    vector< pair < pair<std::string,std::string>,int > > pairs;
    vector <pair<std::string,std::string> > categs_per_store;
    vector<pair<std::string,int> > bill;
    pair<pair<int, int>, int> moves;
    vector<pair<std::string, pair<vector<std::string>, int> > > days;
    pair<pair<std::string, int>, std::string> bon_failed;


    if (argc != 2) {
        cout << "Numar invalid de parametri." << endl;
        exit(1);
    }

    Companie companie = load(argv[1]);

    cout << endl << "Enable chart generation (plotting requires extra time)? y/n" << endl;
    cin >> option;

    if(option == 'y') {
	vector<pair<string, int> > rez_case_1 = companie.total_sales_per_store();
	hashtable<string, int> rez_case_2 = companie.sales_val_per_prod();
	vector<pair<string, double> > rez_case_3 = companie.val_cos_med();
	std::ofstream chart_1, chart_2, chart_3;

	plot_vector_int(chart_1, rez_case_1, "store_sales_chart.html");
	plot_hashtable(chart_2, rez_case_2, produse, "product_sales_chart.html");
	plot_vector_double(chart_3, rez_case_3, "avg_shopping_val_chart.html");
    }

    do {
        menu();

        if (!(cin >> N)) // test for stdin closing.
            break;

        switch (N) {
            case 0:
                exit_message();
                return 0;
                break;

            case 1:
            {
                cout << endl;
                cout << "All stores and the corresponding transactions:" << endl;
                cout << endl;
                vector<pair<string, int> > rez;

		rez = companie.total_sales_per_store();

                for (int i = 0; i < (int)rez.size(); i++) {
                    cout << rez[i].first << " store sales: " << rez[i].second << endl;
                }
                cout << endl;
                break;
            }

            case 2:
            {
                cout << endl;
                cout << "All products and the correspoding transactions:" << endl;
                cout << endl;
                hashtable<string, int> rez;
                rez = companie.sales_val_per_prod();

                for (hashtable<string, int>::iterator it = rez.begin(); it != rez.end(); ++it) {
                    std::cout << produse[it->first].name << ": " << it->second << endl;
                }
                cout << endl;
                break;
            }

            case 3:
	    {
                cout << endl;
                cout << "Average shopping basket value: ";
                cout << endl;
		vector<pair<string, double> > rez = companie.val_cos_med();

		for (int i = 0; i < (int)rez.size(); i++) {
		    cout << rez[i].first << ": " << rez[i].second << endl;
		}
                break;
	    }

            case 4:
                cout << "The best selling categories of products are: " << endl;
                categs_per_store = companie.best_categories();
                for(int i = 0; i < (int)categs_per_store.size(); i++)
                    cout << "For store " << categs_per_store[i].second << ": " << id2categ[categs_per_store[i].first] << endl;
                break;

            case 5:
                cout << "You want to see top x best selling pairs of products. Enter x: " << endl;
                cin >> x;

                pairs = companie.best_pairs(x);
                for (int i = 0; i < x; i++){
                    cout << produse[pairs[i].first.first].name << " and " << produse[pairs[i].first.second].name << endl;
                }

                break;

            case 6:
                days = companie.best_days_sales(false);
                for (int i = 0; i < (int)days.size(); i++) {
                    cout << "For store " << days[i].first << ", the following days had the most products sold (" << days[i].second.second << "):\n";
                    for (int j = 0; j < (int)(days[i].second.first.size()) - 1; j++) {
                        cout << days[i].second.first[j] << ", ";
                    }
                    cout << days[i].second.first.back() << ".\n";
                }
                break;

            case 7:
                days = companie.best_days_sales(true);
                for (int i = 0; i < (int)days.size(); i++) {
                    cout << "For store " << days[i].first << ", the following days had the most customers (" << days[i].second.second << "):\n";
                    for (int j = 0; j < (int)(days[i].second.first.size()) - 1; j++) {
                        cout << days[i].second.first[j] << ", ";
                    }
                    cout << days[i].second.first.back() << ".\n";
                }
                break;

            case 8:
                cout << "Enter bill id and store id:" << endl;
                cin >> id >> store;
                bill = companie.get_bill_content(id,store);
                if (bill.size() == 0) {
                    cout << "That bill is empty or it doesn't even exist." << endl;
                } else {
                    cout << "The bill content:" << endl;
                    for(int i = 0; i < (int)bill.size(); i++)
                        cout << bill[i].second << " x " << produse[bill[i].first].name << endl;
                }
                break;

            case 9:

                cout << "Enter the store name you want to see the report for: " << endl;
                cin >> s;
                cout << "For store " << s << ", " << companie.second_checkout(s).first << " clients would benefit per hour and " << companie.second_checkout(s).second << " clients per day." << endl;
                break;

            case 10:
                cout << "Enter product type:" << endl;
                cin >> s;
                if (companie.depozit.first_slot_that_contains_type(s) == -1)
                    cout << "Requested product type is not in the deposit" << endl;
                else
                    cout << s << " can be found on slot number " << companie.depozit.first_slot_that_contains_type(s) << endl;
                break;

            case 11:
                cout << "Enter product type:" << endl;
                cin >> s;
                moves = companie.depozit.pallet_moves(s);
                if (moves.second == -1) {
                    cout << "Requested product type is not in the deposit" << endl;
                } else {
                    if (moves.second)
                        cout << "Move from slot " << moves.first.first << " to slot " << moves.first.second << " " << moves.second << " times." << endl;
                    cout << "Item at the top of slot " << moves.first.first << " is the required pallet.\n";
                }

                break;

            case 12:
                bon_failed = companie.task_12();
                if (bon_failed.first.second == -1)
                    cout << "There are not enough pallets for each type of product.\n";
                if (bon_failed.first.second == 1)
                    cout << "All the orders were satisfied successfully.\n";
                if (bon_failed.first.second == 0)
                    cout << "The bill with id " << bon_failed.first.first << " was not satisfied successfully, the missing product was " << bon_failed.second << ".\n";
                break;

            default:
                cout  <<  "Wrong input."  <<  endl;
                break;
        }
    } while (N != 0);

    return 0;
}
