#include <iostream>
#include <lwgetopts.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    using namespace lwgetopts;
    ArgsMetaMap meta = {
        // {argument, {number-of-parameters, description, required}}
        {"--flag", {0, "a simple flag", false}},
        {"--param", {1, "a required parameter", true}},
        {"--param-list", {3, "a parameter list looks for 3 params", false}},
        {"--help", {0, "print help", false}},
        {"--print", {0, "print arguments", false}}
    };
    
    auto args = parseArguments(argc, argv, meta);
    validateArguments(args, meta);

    if (args.count("--print")) {
        printArguments(args);
        return 0;
    }

    if (args.count("--help")) {
        for (auto &m : meta) {
            cout << m.first << "\t" << m.second.description << endl;
        }
        return 0;
    }

    cout << "param = " << args["--param"][0] << endl;
    return 0;
}