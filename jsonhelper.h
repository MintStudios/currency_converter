//
// Created by Bera Koklu on 11/15/23.
//

#ifndef INC_11_15_PROJ_JSONHELPER_H
#define INC_11_15_PROJ_JSONHELPER_H

#include <fstream>
#include "HTTPRequest.hpp"
#include "json.hpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace nlohmann;
using http::Request;

json readJSON(string filename) {
    ifstream file(filename);
    string line;
    string open_response;
    while (getline(file, line)) {
        open_response += line;
    }
    file.close();
    return json::parse(open_response);
}

void writeJSON(string filename, string s_content) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "File not found (writing JSON)" << endl;
        exit(1);
    }
    file << s_content;
    file.close();
}

json updateJSON(string url, string filename) {
    try {
        http::Request request{url};
        string s_response;
        auto response = request.send("GET");
        cout << string{response.body.begin(), response.body.end()} << '\n';
        s_response = string{response.body.begin(), response.body.end()};

        writeJSON(filename, s_response);

        return json::parse(s_response);

    } catch (const exception &e) {
        cerr << "Request failed, error: " << e.what() << '\n';
        exit(1);
    }
}

json getJSON() {
    ifstream file("currencyrates.json");

    if (file.is_open()) {
        auto current_timestamp = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
        time_t next_timestamp = readJSON("currencyrates.json")["time_next_update_unix"];

        if ((next_timestamp - current_timestamp) < 0) {
            cout << "New file (Old expired)" << endl;
            file.close();
            return updateJSON("http://open.er-api.com/v6/latest/USD", "currencyrates.json");
        } else {
            cout << "Old File" << endl;
            file.close();
            return readJSON("currencyrates.json");
        }

    } else {
        cout << "New file (No file)" << endl;
        file.close();
        return updateJSON("http://open.er-api.com/v6/latest/USD", "currencyrates.json");
    }
}

#endif //INC_11_15_PROJ_JSONHELPER_H
