// web_server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<fstream>
#include"server.h"
#include"json.h"
using namespace std;

void test_json_engine() {
    fstream newfile;
    string file_s;
    string line;
    newfile.open("json_text.txt", ios::in);
    if (newfile.is_open()) {
        while (getline(newfile, line)) {
            file_s.append(line);
        }
    }
    newfile.close();
    cout << "----------Testing the json string----------" << endl;
    cout << file_s << endl;

    json _test;
    _test.load_json_string(file_s);

    cout << "brackets: " << _test.validate_brackets() << endl;
    cout << "entries: " << _test.validate_entries() << endl;
}

void test_number_parser() {
    json _test;
    _test.load_json_string("-.3");
    //pair<number_info, int> a = _test.process_number(0);
}


int main()
{
    /*
    server my_server;
    my_server.setup();
    my_server.initiate();
    my_server.accept_connection();
    my_server.close_server();
    */

    test_json_engine();
    //test_number_parser();

    //test.load_json_string(std::string(s));
    //cout << test.validate_entries(s) << "\n";

    std::cout << "Hello World!\n";
    std::cin.get();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
