#include "functions.h"

bool Check(const string& str) {
    return  str.size() < 64 &&
            str[0] <= 0x39 &&
            str[str.size() - 1] >= 0x30;
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
    std::string str = "";
    while(true) {
        cin >> str;
        sort(str.rbegin(), str.rend());
        if (Check(str)) {
            ReplaceEven(str);
            gData.SetData(str);
        } else {
            cout << "Only ciphers allowed and length no more than 64 symbols" << '\n';
        }
    }
}

void* ProcessData(void*) {

    int sckt;
    int cnct;
    int snd;

    struct addrinfo hints;
    struct addrinfo* servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int gai = getaddrinfo(NULL, "1234", &hints, &servinfo);
    if (gai != 0) {
        printf("getaddrinfo: %s\n", gai_strerror(gai));
        exit(1);
    }

    sckt = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sckt == -1) {
        perror("socket");
        exit(1);
    }

    cnct = connect(sckt, servinfo->ai_addr, servinfo->ai_addrlen);
    if (cnct == -1) {
        perror("connect (server didn't started?)");
        close(sckt);
        exit(1);
    }

    while(true) {
	string data(gData.GetData());
	printf("Data: %s\n", data.c_str());
        std::stringstream ss;
        ss << SumFigures(data);
        std::string sm = ss.str();
        cnct = connect(sckt, servinfo->ai_addr, servinfo->ai_addrlen);
        snd = send(sckt, sm.c_str(), sizeof(sm), 0);
        if (snd == 0) {
            perror("send");
            exit(1);
        }
    }
    freeaddrinfo(servinfo);
    close(sckt);
}
