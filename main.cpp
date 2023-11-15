#include <iostream>
#include "jsonhelper.h"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main() {
    json j_response = getJSON();
    cout << j_response["rates"]["EUR"] << endl;
    return 0;
}
