#include <iostream>
#include <string>
#include "fread.h"
#include "interpreter.h"

int main() {
    Runner runner(readFile(std::string("testcode.txt")));
    return 0;
}