//#include <stdio.h>
//#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <algorithm>

//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>

//#include <mutex>
//#include <pthread.h>
#include <condition_variable>

using std::string;
using std::mutex;
using std::condition_variable;
using std::lock_guard;
using std::unique_lock;
using std::stoi;
using std::cin;
using std::cout;

class Data {
private:
    mutex mx;
    string s = "";
    condition_variable data_cond;
public:
    void SetData(const string& str) {
        lock_guard<mutex>lock(mx);
        s = str;
        data_cond.notify_one();
    }
    string GetData() {
        unique_lock<mutex>ulock(mx);
        data_cond.wait(ulock);
        return s;
    }
};

Data gData;

bool Check(const string& str) {
    return  str.size() < 64 &&
            str[0] >= 0x30 &&
            str[str.size() - 1] <= 0x39;
}

void ReplaceEven(string& str) {
    string result = "";
    for (char item : str){
        if (stoi(&item) % 2)
            result += item;
        else
            result += "KB";
    }
    str = result;
}

int SumFigures(const string& str){
    int result = 0;
    for (char item : str) {
        if (item >= 0x30 && item <= 0x39) {
            result += stoi(&item);
        }
    }
    return result;
}

void* PreparateData(void*) {
    string str = "";
    while(true) {
        cin >> str;
        sort(str.begin(), str.end());
        if (Check(str)) {
            ReplaceEven(str);
            gData.SetData(str);
        } else {
            cout << "error" << '\n';
        }
    }
}

void* ProcessData(void*) {

    struct addrinfo hints;
    struct addrinfo* servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int ga = getaddrinfo(NULL, "1234", &hints, &servinfo);
    printf("GetAddrInfo: %i\n", ga);

    int s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    printf("Socket: %i\n", s);

    int cn = connect(s, servinfo->ai_addr, servinfo->ai_addrlen);
    printf("Connect: %i\n", cn);

    while(true) {
        cout << "I'm waiting..." << '\n';
        std::stringstream ss;
        ss << SumFigures(gData.GetData());
        std::string word = ss.str();
        int sd = send(s, word.c_str(), sizeof(word), 0);
    }
    freeaddrinfo(servinfo);
}

int main(){

    pthread_t th1;
    pthread_t th2;
    pthread_create(&th1, NULL, PreparateData, NULL);
    pthread_create(&th2, NULL, ProcessData, NULL);


    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}
