#ifndef PARSER_H
#define PARSER_H

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

/* Usage:
 *
 * Instantiezi clasa:
 *  Parser nume (nume_fisier);
 *  Apelarea functiei de citire returneaza urmatorul camp din fisier,
 *  de la pozitia curenta pana la urmatorul delimitator ( "," sau "\r" sau "\n")
 *
 *  Exemplu:
 *
 *  Parser p("paleti.csv");
 *
 *  // in fisierul paleti.csv, prima coloana memoreaza stringuri, urmatoarele 3 memoreaza inturi
 *  string a;
 *  string col1, col2, col3, col4; //este nevoie de stringuri pentru procesarea capului de tabel
 *  int b, c, d;
 *
 *  //procesare cap de tabel
 *  p >> col1 >> col2 >> col3 >> col4;
 *
 *  // cat timp exista linii neprocesate, se citesc campurile si se afiseaza
 *  while (p >> a >> b >> c >> d) {
 *      cout << a << " " << b << " " << c << " " << d << '\n'; // a este string; b, c si d sunt int
 *  }
 */


class Parser {
    private:
        std::ifstream f_;
        std::string line_;
        int last_comma_;
        bool line_ended_;

        // Read next line from file
        void next_line() {
            line_ended_ = false;
            last_comma_ = -1;

            std::getline(f_, line_);

            line_ = trim_crlf(line_);
        }

        // Eliminate de CR and LF from the end of the line
        std::string trim_crlf(std::string field) {
            while (field.size() && (field[field.size() - 1] == '\r' ||
                                    field[field.size() - 1] == '\n'))
                field = field.substr(0, field.size() - 1);

            return field;
        }

        // Read next field
        std::string next_field() {
            if (line_ended_)
                next_line();

            // Input file seems to have \r instead of \n. Error or intended?

            int comma = line_.find_first_of(",\r\n", last_comma_ + 1);
            std::string field;
            if (comma == (int)std::string::npos) {
                line_ended_ = true;
                field = line_.substr(last_comma_ + 1);
            } else {
                field = line_.substr(last_comma_ + 1, comma - last_comma_ - 1);
                last_comma_ = comma;
            }

            return trim_crlf(field);
        }

    public:

        // User-defined constructor
        Parser(const char* filename) :
            f_(filename), line_ended_(true) {
            assert(f_); // Check that file has been opened successfully
        }

        Parser(std::string filename) :
            f_(filename.c_str()), line_ended_(true) {
            assert(f_);
        }

        // Destructor
        ~Parser() {
            f_.close();
        }

        bool eof() const {
            return f_.eof() && line_ended_ == true;
        }

        operator bool() const {
            return !eof();
        }

        Parser& operator>>(std::string& field) {
            field = next_field();
            return *this;
        }

        Parser& operator>>(int& ifield) {
            if (!(*this))
                return *this;
            std::string field = next_field();
            assert(field.size() <= 9);
            std::istringstream iss(field);
            iss >> ifield;
            return *this;
        }
};

#endif /* PARSER_H */
