#include <iostream>
#include "connection.h"

using std::string;
using std::cout;

int main() {

    Connection connect;
    connect.Init();

    while(true) {
        connect.Receive();
        string result = connect.GetData();
	cout << "Data: " << result << " ";
        if (result.size() > 2 && !(stoi(result)%32)) {
            cout << "Correct data\n";
        } else {
            cout << "Wrong data\n";
        }
    }

    connect.Close();

    return 0;
}
