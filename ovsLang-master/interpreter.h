#ifndef Ovs_Ovs_interpreter_H
#define Ovs_Ovs_interpreter_H

#include "tokenizer.h"
#include "translator.h"
#include "exception.h"
#include "str_split.h"
#include "vm.h"

class Runner {
public:
    Runner(std::string code, std::istream *inputStream = &std::cin,
           std::ostream *outputStream = &std::cout,
           std::ostream *errorStream = &std::cerr) {
        Tokenizer tokenizer(code);
        auto linesOfCode = split(code);
        std::vector<Token> tokens;
        try {
            tokens = tokenizer.tokenize();
        } catch (std::string e) {
            *errorStream << "Not parsed\n"
                         << e;
            return;
        }
        Poliz *OvsByteCode;
        try {
            Translator translator(tokens);
            OvsByteCode = translator.translate();
        } catch (std::string e) {
            *errorStream << "Not translated\n"
                         << e;
            return;
        }
        try {
            VM vm(inputStream, outputStream);
            writePoliz(OvsByteCode);
            vm.run(OvsByteCode);
        } catch (Exception e) {
            *errorStream << "Runtime error\n"
                         << e.message << "\n"
                         << e.line << ": " << linesOfCode[e.line - 1] << "\n";
        }
    }

    void writePoliz(Poliz* code){
        std::ofstream O("poliz.txt");
        for(auto i:code->operations)
            O << i.toStr() << '\n';
    }

};

#endif //Ovs_Ovs_interpreter_H
