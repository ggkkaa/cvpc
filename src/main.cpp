#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "lexer.hpp"

int main(int argc, char* argv[]) {

    std::vector<std::string> arguments;
    std::string input_file = "";
    std::string output_file = "";

    std::cout << argc << "\n";

    for (int i = 1; i < argc; ++i) {
        arguments.push_back(argv[i]);
    }

    for (size_t i = 0; i < argc; i++) {
        std::string arg = arguments[i];
        std::cout << arg << "\n";

        if (arg == "") continue;

        if(arg[0] != '-' && arg != "-o") {
            if(input_file != "") {
                std::cout << "ERROR: Input file specified more than once. The first one is '" << input_file << "' and the second one is '" << arg << "'\n";
                return 1;
            }
            input_file = arg;
            continue;
        }

        if(arg == "-o") {
            if(output_file != "") {
                std::cout << "ERROR: Output file specified more than once.\n";
                return 1;
            }
            output_file = arguments[i + 1];
            std::cout << output_file << "\n";
            i++;
            continue;
        }
    }

    std::ifstream file_stream(input_file);

    file_stream.open("r");

    if(!file_stream.is_open()) {
        std::cerr << "ERROR: Cannot open '" << input_file << "'\n";
        return 2;
    }

    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    std::string file = buffer.str();

    std::cout << file;

    Lexer* lexer = new Lexer(file);

    std::cout << "Hello, World!\n";
}