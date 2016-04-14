#include <iostream>
#include "../include/Companie.h"

extern Companie load(std::string input_dir);

int main() {
    load(std::string("input/"));

    Companie companie;
    //Magazin magazin;

    companie.val_cos_med();

    return 0;
}
