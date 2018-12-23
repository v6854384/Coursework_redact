#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include "Calc.h"

using namespace std;

int main()
{
    Stack St;
    string Line;
    while(getline(cin,Line), Line!="")
    {
        St.Calc(Line);
    }
    return 0;
}
