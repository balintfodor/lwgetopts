#ifndef __lwgetopts_hpp__
#define __lwgetopts_hpp__

#include <string>
#include <exception>
#include <vector>
#include <map>
#include <iostream>

namespace lwgetopts {

    struct ArgsMeta {
        int numParams;
        std::string description;
        bool required;
    };

    typedef std::map<std::string, ArgsMeta> ArgsMetaMap;
    typedef std::map<std::string, std::vector<std::string>> ArgsMap;

    ArgsMap parseArguments(int argc, char *argv[], const ArgsMetaMap meta)
    {
        using namespace std;
        ArgsMap result;

        for (int i = 1; i < argc; ++i) {
            bool matched = false;
            for (auto &a : meta) {
                if (string(argv[i]) == a.first) {
                    result[a.first].clear();
                    for (int j = 1; j <= a.second.numParams; ++j) {
                        if (i + j >= argc) {
                            throw runtime_error("missing argument param at '" + a.first + "'");
                        }
                        result[a.first].push_back(string(argv[i + j]));
                    }
                    i += a.second.numParams;
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                throw runtime_error("unknown arg '" + string(argv[i]) + "'");
            }
        }
        return result;
    }

    void validateArguments(ArgsMap &args, const ArgsMetaMap argsMeta)
    {
        using namespace std;
        for (auto &m : argsMeta) {
            if (args.count(m.first)) {
                if (args[m.first].size() != m.second.numParams) {
                    throw runtime_error("argument '" + string(m.first) + "' needs " + to_string(m.second.numParams) + " params, got " + to_string(args[m.first].size()));
                }
            } else {
                if (m.second.required) {
                    throw runtime_error("missing required argument '" + string(m.first) + "'");
                }
            }
        }
    }

    void printArguments(const ArgsMap &args)
    {
        using namespace std;
        for (auto &a : args) {
            cout << a.first << " :";
            for (auto &v : a.second) {
                cout << " " << v;
            }
            cout << endl;
        }
    }

}

// --- USAGE:
// #include <iostream>
// #include "lwgetopts.hpp"
// #include <map>
// #include <vector>
// #include <string>

// using namespace std;

// int main(int argc, char *argv[])
// {
//     using namespace lwgetopts;
//     ArgsMetaMap meta = {
//         {"--help", {0, "print help", false}},
//         {"--exp", {2, "set exp time, 0-1080", false}},
//         {"--diff", {1, "produce difference image, -1 no threshold, otherwise 0-255", false}},
//         {"--raw", {0, "produce raw image", false}}
//     };
//     auto args = parseArguments(argc, argv, meta);
//     validateArguments(args, meta);
//     printArguments(args);
//     return 0;
// }

#endif // __lwargopts_hpp__