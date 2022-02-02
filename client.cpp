#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstring>
#include <netdb.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <mutex>
#include <pthread.h>
#include <condition_variable>
#include <sstream>

using namespace std;

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
            cout << "error" << endl;
        }
    }
}

//struct sockaddr_in local;

void* ProcessData(void*) {
//    int s = socket(AF_INET, SOCK_STREAM, 0);
//    local.sin_family = AF_INET;
//    local.sin_port = htons(1234);
//    local.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//    inet_ntop(AF_INET, "127.0.0.1", &(local.sin_addr));
//    int cn = connect(s, (struct sockaddr*)&local, sizeof(local));

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

//    std::string word;
//    while(true) {
//	std::cin >> word;
//	word += "\n";
//	int sd = send(s, word.c_str(), sizeof(word), 0);
//	std::this_thread::sleep_for(1000ms);
//	if (sd <= 0) break;
//    }

    while(true) {
        cout << "I'm waiting..." << endl;
        string data = gData.GetData();
        int sum = SumFigures(data);
        cout << data << ", sum = " << sum << endl;
	std::stringstream ss;
	ss << sum;
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
