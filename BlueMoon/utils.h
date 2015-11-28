#ifndef UTILS
#define UTILS

#include <string>
#include <iostream>
#include <cstdio>
//#include <memory>
#include <tr1/memory>

std::string exec(const char* cmd) {
    std::tr1::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

#endif // UTILS

