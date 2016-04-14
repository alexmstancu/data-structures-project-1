#include <iostream>
#include <fstream>
#include <string>
#include "../include/hashtable.h"
#include "../include/vector.h"
#include "../include/pair.h"
#include "../include/Produs.h"

extern hashtable<std::string, Produs> produse;

std::ifstream header;
std::ifstream middle_1;
std::ifstream middle_2;
std::ifstream footer;

const std::string charts_path = "charts/output/";

void open_files() {
    header.open("charts/input/header");
    middle_1.open("charts/input/middle_1");
    middle_2.open("charts/input/middle_2");
    footer.open("charts/input/footer");
}

void copy_file(std::ifstream& in, std::ofstream& out) {
    std::string line;
    while(getline(in, line))
	out << line << std::endl;
}

void close_files() {
    header.close();
    middle_1.close();
    middle_2.close();
    footer.close();
}

void plot_hashtable(std::ofstream& chart, hashtable<std::string, int>& h, hashtable<std::string, Produs>& p, std::string chart_name) {
    std::string path = charts_path;
    path.append(chart_name);

    open_files();
    chart.open(path.c_str());
    
    copy_file(header, chart);

    for (hashtable <std::string, int>::iterator it = h.begin(); it != h.end(); ++it) {
	chart << "'" << p[it->first].id << " - " << p[it->first].name << "', ";
    }
    
    copy_file(middle_1, chart);
    
    for (hashtable<std::string, int>::iterator it = h.begin(); it != h.end(); ++it) {
	chart << "'" << p[it->first].id << "', ";
    }

    copy_file(middle_2, chart);

    for(hashtable<std::string, int>::iterator it = h.begin(); it != h.end(); ++it) {
	chart << it->second << ", ";
    }
 
    copy_file(footer, chart);
    close_files();
    chart.close();
}

void plot_vector_int(std::ofstream& chart, vector<pair<std::string, int> >& v, std::string chart_name) {
    std::string path = charts_path;
    path.append(chart_name);

    open_files();
    chart.open(path.c_str());

    copy_file(header, chart);

    for (int i = 0; i < (int)v.size(); i++) {
	chart << "'" << (i + 1) << " - " << v[i].first << "', ";
    }

    copy_file(middle_1, chart);

    for (int i = 0; i < (int)v.size(); i++) {
	chart << "'" << (i + 1) << "', ";
    }

    copy_file(middle_2, chart);

    for(int i = 0; i < (int)v.size(); i++) {
	chart << "'" << v[i].second << "', ";
    }

    copy_file(footer, chart);
    close_files();
    chart.close();
}

void plot_vector_double(std::ofstream& chart, vector<pair<std::string, double> >& v, std::string chart_name) {
    std::string path = charts_path;
    path.append(chart_name);

    open_files();
    chart.open(path.c_str());

    copy_file(header, chart);

    for (int i = 0; i < (int)v.size(); i++) {
	chart << "'" << (i + 1) << " - " << v[i].first << "', ";
    }

    copy_file(middle_1, chart);

    for (int i = 0; i < (int)v.size(); i++) {
	chart << "'" << (i + 1) << "', ";
    }

    copy_file(middle_2, chart);

    for(int i = 0; i < (int)v.size(); i++) {
	chart << "'" << v[i].second << "', ";
    }

    copy_file(footer, chart);
    close_files();
    chart.close();
}

