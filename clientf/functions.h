#pragma once

#include <cstring>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <netdb.h>
#include <unistd.h>
#include "data.h"

using std::string;
using std::cin;
using std::cout;
using std::stoi;

extern Data gData;

bool Check(const string& str);
void ReplaceEven(string& str);
int SumFigures(const string& str);
void* PreparateData(void*);
void* ProcessData(void*);
