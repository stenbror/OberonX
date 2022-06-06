
#include <iostream>
#include <memory>

#include "Tokenizer.h"
#include "Parser.h"

extern std::map<std::string, TokenCode> reservedKeywords;

int main()
{
    std::cout << "OberonX Compiler, Version 0.01" << std::endl;
    std::cout << "Written by Richard Magnor Stenbro. All rights reserved!" << std::endl << std::endl;

    std::cout << reservedKeywords.size()<< std::endl;


    auto lexer = std::make_shared<Tokenizer>(Tokenizer());
    auto parser = std::make_shared<Parser>(Parser(lexer));
    auto node = parser->ParseOberon();

    return 0;
}