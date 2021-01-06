#ifndef Ovs_STR_SPLIT_H
#define Ovs_STR_SPLIT_H

#include <vector>
#include <string>
#include <algorithm>

std::vector<std::string> split(std::string str, char delimeter = '\n') {
    std::vector<std::string> ans;
    int last = 0;
    while (last < str.size()) {
        int now = std::find(str.begin() + last, str.end(), delimeter) - str.begin();
        ans.push_back(str.substr(last, now - last));
        last = now + 1;
    }
    return ans;
}

#endif //Ovs_STR_SPLIT_H
