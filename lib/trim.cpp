#include "trim.h"

#include <algorithm>
#include <iterator>

using namespace std;

string & r_trim(string & str) {
    if (!isspace(str.front()))
        return str;
    str.erase(begin(str),
              find_if(begin(str), end(str), [](auto i) { return isspace(i); }));
    return str;
}

string & l_trim(string & str) {
    if (!isspace(str.back()))
        return str;
    str.erase(
        find_if(str.rbegin(), str.rend(), [](auto i) { return isspace(i); })
            .base(),
        end(str));
    return str;
}

string & trim(string & str) {
    return r_trim(l_trim(str));
}
