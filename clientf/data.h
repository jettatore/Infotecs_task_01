#pragma once
#include <condition_variable>

using std::string;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using std::condition_variable;

class Data {
public:
    Data():s(""){ }
    void SetData(const string& str);
    string GetData();

private:
    mutex mx;
    string s;
    condition_variable data_cond;
};
