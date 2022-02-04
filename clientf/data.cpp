#include "data.h"

void Data::SetData(const string& str) {
    lock_guard<mutex>lock(mx);
    s = str;
    data_cond.notify_one();
}

string Data::GetData() {
    unique_lock<mutex>ulock(mx);
    data_cond.wait(ulock);
    return s;
}
