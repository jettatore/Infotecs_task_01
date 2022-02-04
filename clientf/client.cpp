#include <cstring>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <netdb.h>
#include <unistd.h>
#include "data.h"
#include "functions.h"

using std::mutex;

Data gData;

int main(){

    pthread_t th1;
    pthread_t th2;
    pthread_create(&th1, NULL, PreparateData, NULL);
    pthread_create(&th2, NULL, ProcessData, NULL);


    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}
