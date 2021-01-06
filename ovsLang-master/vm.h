
#ifndef Ovs_VM_H
#define Ovs_VM_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include "poliz.h"
#include "ovsobj.h"
#include "exception.h"

#define COMPARE(OPERATION) \
if (getItemOfCurStack(1)->type == getItemOfCurStack()->type && getItemOfCurStack(1)->type == Ovsobj::INT) { \
    bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value) \
               OPERATION *static_cast<int *>(getItemOfCurStack()->value); \
    curStack.pop_back(); \
    curStack.pop_back(); \
    curStack.push_back(new Ovsobj(Ovsobj::BOOL, static_cast<void*>(new bool(tmp)))); \
} \
else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type && getItemOfCurStack(1)->type == Ovsobj::DOUBLE) { \
    bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value) \
               OPERATION *static_cast<double *>(getItemOfCurStack()->value); \
    curStack.pop_back(); \
    curStack.pop_back(); \
    curStack.push_back(new Ovsobj(Ovsobj::BOOL, static_cast<void*>(new bool(tmp)))); \
} \
else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type && getItemOfCurStack(1)->type == Ovsobj::STRING) { \
    bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value) \
               OPERATION *static_cast<std::string *>(getItemOfCurStack()->value); \
    curStack.pop_back(); \
    curStack.pop_back(); \
    curStack.push_back(new Ovsobj(Ovsobj::BOOL, static_cast<void*>(new bool(tmp)))); \
} \
else { \
    throw Exception("no compare vars", curOp.numLine, curOp.numPos); \
}


class VM {
private:
    struct scope {
        std::map<std::string, std::pair<Poliz *, bool>> funcs;
        std::map<std::string, std::pair<Poliz *, bool>> classes;
        std::map<std::string, Ovsobj *> vars;
        enum {
            SIMPLE, FUNC, CLASS
        } type = SIMPLE;
    };

    class Level {
    public:
        std::string name;
        Poliz *poliz;
    };

    std::vector<Ovsobj *> curStack;
    std::vector<Level> stackTrace;
    std::vector<scope> bigScopes;
    std::vector<scope> scopes;
    std::istream *inputStream{};
    std::ostream *outputStream{};

    Ovsobj *getItemOfCurStack(int number = 0) {
        return curStack[curStack.size() - 1 - number];
    }

    std::map<std::string, Ovsobj *> runLevel() {
        auto &curLevel = stackTrace.back();
        int i = 0;
        for (; i < stackTrace.back().poliz->operations.size(); i++) {
            auto &curOp = stackTrace.back().poliz->operations[i];
            switch (curOp.TYPE) {
                case Element::OR: {
                    if (getItemOfCurStack(1)->type == Ovsobj::BOOL
                        && getItemOfCurStack(0)->type == Ovsobj::BOOL) {
                        bool value = *(static_cast<bool *>(getItemOfCurStack(1)->value)) or
                                     *(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }

                    else if (getItemOfCurStack(1)->type == Ovsobj::INT
                             && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        bool value = *(static_cast<int *>(getItemOfCurStack(1)->value)) or
                                     *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::AND: {
                    if (getItemOfCurStack(1)->type == Ovsobj::BOOL
                        && getItemOfCurStack(0)->type == Ovsobj::BOOL) {
                        bool value = *(static_cast<bool *>(getItemOfCurStack(1)->value)) and
                                     *(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::INT
                             && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        bool value = *(static_cast<int *>(getItemOfCurStack(1)->value)) and
                                     *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::NOT: {
                    if (getItemOfCurStack(1)->type == Ovsobj::BOOL
                        && getItemOfCurStack(0)->type == Ovsobj::BOOL) {
                        bool value = not*(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::XOR: {
                    if (getItemOfCurStack(1)->type == Ovsobj::BOOL
                        && getItemOfCurStack(0)->type == Ovsobj::BOOL) {
                        bool value = *(static_cast<bool *>(getItemOfCurStack(1)->value)) xor
                                     *(static_cast<bool *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::BOOL, new bool(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::OR_BIT: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) |
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::AND_BIT: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) &
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::SHIFT_LEFT_BIN: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) <<
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::SHIFT_RIGHT_BIN: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) >>
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::IMPORT: {
                    std::string fileName = curOp.stringValue.substr(0, curOp.intValue);
                    std::string className = curOp.stringValue.substr(curOp.intValue);
                    std::string importCode = readFile(fileName);
                    Tokenizer importTokenizer(importCode);
                    auto lines = split(importCode);
                    std::vector<Token> importTokens;
                    try {
                        importTokens = importTokenizer.tokenize();
                    } catch (std::string e) {
                        throw Exception("not parsed imported file\n" + e, curOp.numLine, curOp.numPos);
                    }
                    Poliz *importOvsByteCode;
                    try {
                        Translator importTranslator(importTokens);
                        importOvsByteCode = importTranslator.translate();
                    } catch (std::string e) {
                        throw Exception("Not translated imported file\n" + e, curOp.numLine, curOp.numPos);
                    }
                    if (bigScopes.back().classes.count(className)) {
                        throw Exception("invalid import, class already exist", curOp.numLine, curOp.numPos);
                    }
                    bigScopes.back().classes[className].first = scopes.back().classes[className].first = importOvsByteCode;
                    bigScopes.back().classes[className].second = scopes.back().classes[className].second = true;
                    break;
                }
                case Element::PLUS: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) +
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) +
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::STRING
                             && getItemOfCurStack()->type == Ovsobj::STRING) {
                        std::string value = (*(static_cast<std::string *>(getItemOfCurStack(1)->value)))
                                            + (*(static_cast<std::string *>(getItemOfCurStack()->value)));
                        auto tmpobj = new Ovsobj(Ovsobj::STRING, new std::string(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MINUS: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) -
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) -
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MUL: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) *
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) *
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::DIV: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        double value = (double) *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MOD: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) %
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::INTDIV: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::UNAR_PLUS: {
                    if (getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::UNAR_MINUS: {
                    if (getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = -*(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else if (getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = -*(static_cast<double *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::DOUBLE, new double(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::UNAR_TILDA: {
                    if (getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = ~*(static_cast<int *>(getItemOfCurStack()->value));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::POW: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = pow(*(static_cast<int *>(getItemOfCurStack(1)->value)),
                                        *(static_cast<int *>(getItemOfCurStack()->value)));
                        auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(tmpobj);
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::COPY: {
                    switch (getItemOfCurStack()->type) {
                        case Ovsobj::INT:
                        case Ovsobj::STRING:
                        case Ovsobj::DOUBLE:
                        case Ovsobj::BOOL:
                        case Ovsobj::NONE:
                            if (getItemOfCurStack()->type == Ovsobj::INT) {
                                getItemOfCurStack(1)->value = static_cast<void *>(new int(
                                        *static_cast<int *>(getItemOfCurStack()->value)));
                            }
                            else if (getItemOfCurStack()->type == Ovsobj::STRING) {
                                getItemOfCurStack(1)->value = static_cast<void *>(new std::string(
                                        *static_cast<std::string *>(getItemOfCurStack()->value)));
                            }
                            else if (getItemOfCurStack()->type == Ovsobj::DOUBLE) {
                                getItemOfCurStack(1)->value = static_cast<void *>(new double(
                                        *static_cast<double *>(getItemOfCurStack()->value)));
                            }
                            else if (getItemOfCurStack()->type == Ovsobj::BOOL) {
                                getItemOfCurStack(1)->value = static_cast<void *>(new bool(
                                        *static_cast<bool *>(getItemOfCurStack()->value)));
                            }
                            else if (getItemOfCurStack()->type == Ovsobj::NONE) {
                                // nothing
                            }
                            getItemOfCurStack(1)->type = getItemOfCurStack()->type;
                            curStack.pop_back();
                            break;
                        case Ovsobj::LIST:
                            getItemOfCurStack(1)->type = Ovsobj::LIST;
                            getItemOfCurStack(1)->value = new std::vector<Ovsobj *>(
                                    *static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack()->value));
                            curStack.pop_back();
                            break;
                        case Ovsobj::USER_CLASS:
                            getItemOfCurStack(1)->type = Ovsobj::USER_CLASS;
                            getItemOfCurStack(1)->value = new std::pair<Poliz *, std::map<std::string, Ovsobj *>>(
                                    *static_cast<std::pair<Poliz *, std::map<std::string, Ovsobj *>> *>(getItemOfCurStack()->value));
                            curStack.pop_back();
                            break;
                        case Ovsobj::DICT:
                            //todo
                            break;
                    }
                    break;
                }
                case Element::INIT_PARAM: {
                    bigScopes.back().vars[curOp.stringValue]
                            = scopes.back().vars[curOp.stringValue]
                            = getItemOfCurStack();
                    curStack.pop_back();
                    break;
                }
                case Element::GET_VALUE: {
                    if (bigScopes.back().vars.count(curOp.stringValue)) {
                        curStack.push_back(bigScopes.back().vars[curOp.stringValue]);
                    }
                    else {
                        curStack.push_back(new Ovsobj(Ovsobj::NONE));
                        bigScopes.back().vars[curOp.stringValue] = scopes.back().vars[curOp.stringValue] = curStack.back();
                    }
                    break;
                }
                case Element::INDEX_VALUE: {
                    if (getItemOfCurStack()->type == Ovsobj::INT) {
                        int ind = *static_cast<int *>(getItemOfCurStack()->value);
                        if (getItemOfCurStack(1)->type == Ovsobj::LIST) {
                            if (ind < static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack(1)->value)->size()) {
                                auto tmp = (*static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack(1)->value))[ind];
                                curStack.pop_back();
                                curStack.pop_back();
                                curStack.push_back(tmp);
                            }
                            else {
                                throw Exception("index error", curOp.numLine, curOp.numPos);
                            }
                        }
                    }
                    break;
                }
                case Element::DEF_FUNC: {
                    bigScopes.back().funcs[curOp.stringValue].first = scopes.back().funcs[curOp.stringValue].first = curLevel.poliz->funcs[curOp.stringValue];
                    bigScopes.back().funcs[curOp.stringValue].second = scopes.back().funcs[curOp.stringValue].second = true;
                    break;
                }
                case Element::DEF_CLASS: {
                    bigScopes.back().classes[curOp.stringValue].first = scopes.back().classes[curOp.stringValue].first = curLevel.poliz->classes[curOp.stringValue];
                    bigScopes.back().classes[curOp.stringValue].second = scopes.back().classes[curOp.stringValue].second = true;
                    break;
                }
                case Element::CALL_FUNC: {
                    if (curOp.stringValue == "input") {
                        auto *tmp = new std::string;
                        *inputStream >> *tmp;
                        curStack.push_back(new Ovsobj(Ovsobj::STRING, static_cast<void *>(tmp)));
                    }
                    else if (curOp.stringValue == "print") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == Ovsobj::STRING) {
                            *outputStream << *(static_cast<std::string *>(arg->value));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::NONE));
                        }
                        else if (arg->type == Ovsobj::INT) {
                            *outputStream << *(static_cast<int *>(arg->value));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::NONE));
                        }
                        else if (arg->type == Ovsobj::DOUBLE) {
                            *outputStream << *(static_cast<double *>(arg->value));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::NONE));
                        }
                        else if (arg->type == Ovsobj::BOOL) {
                            *outputStream << (*(static_cast<bool *>(arg->value)) ? "True" : "False");
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::NONE));
                        }
                        else if (arg->type == Ovsobj::NONE) {
                            *outputStream << "None";
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::NONE));
                        }
                        else {
                            throw Exception("no printble type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "int") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == Ovsobj::STRING) {
                            int *tmp = new int(std::stoi(*(static_cast<std::string *>(arg->value))));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::INT, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == Ovsobj::DOUBLE) {
                            int *tmp = new int((int) *(static_cast<double *>(arg->value)));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::INT, static_cast<void *>(tmp)));
                        }
                        else {
                            throw Exception("not numbery type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "double") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == Ovsobj::STRING) {
                            auto *tmp = new double(std::stod(*(static_cast<std::string *>(arg->value))));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::DOUBLE, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == Ovsobj::INT) {
                            auto *tmp = new double((double) *(static_cast<int *>(arg->value)));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::DOUBLE, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == Ovsobj::DOUBLE) {
                            auto *tmp = new double((double) *(static_cast<double *>(arg->value)));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::DOUBLE, static_cast<void *>(tmp)));
                        }
                        else {
                            throw Exception("not numbery type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "max") {
                        if (getItemOfCurStack(1)->type == Ovsobj::INT
                            && getItemOfCurStack(0)->type == Ovsobj::INT) {
                            int value = std::max(*(static_cast<int *>(getItemOfCurStack(1)->value)),
                                                 *(static_cast<int *>(getItemOfCurStack()->value)));
                            auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                            curStack.pop_back();
                            curStack.pop_back();
                            curStack.push_back(tmpobj);
                        }
                        else {
                            throw Exception("invalid args", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "min") {
                        if (getItemOfCurStack(1)->type == Ovsobj::INT
                            && getItemOfCurStack(0)->type == Ovsobj::INT) {
                            int value = std::min(*(static_cast<int *>(getItemOfCurStack(1)->value)),
                                                 *(static_cast<int *>(getItemOfCurStack()->value)));
                            auto tmpobj = new Ovsobj(Ovsobj::INT, new int(value));
                            curStack.pop_back();
                            curStack.pop_back();
                            curStack.push_back(tmpobj);
                        }
                        else {
                            throw Exception("invalid args", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "bool") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == Ovsobj::INT) {
                            bool *tmp = new bool((*(static_cast<int *>(arg->value))) != 0);
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::BOOL, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == Ovsobj::BOOL) {

                        }
                        else {
                            throw Exception("not booly type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "str") {
                        auto arg = getItemOfCurStack();
                        if (arg->type == Ovsobj::INT) {
                            auto *tmp = new std::string(std::to_string(*(static_cast<int *>(arg->value))));
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::STRING, static_cast<void *>(tmp)));
                        }
                        else if (arg->type == Ovsobj::BOOL) {
                            auto *tmp = new std::string(*(static_cast<bool *>(arg->value)) ? "True" : "False");
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::STRING, static_cast<void *>(tmp)));
                        }
                        else {
                            throw Exception("not strinble type", curOp.numLine, curOp.numPos);
                        }
                    }
                    else if (curOp.stringValue == "vector") {
                        curStack.push_back(
                                new Ovsobj(Ovsobj::LIST, static_cast<void *>(new std::vector<Ovsobj *>)));
                    }
                    else if (curOp.stringValue == "size") {
                        if (getItemOfCurStack()->type == Ovsobj::LIST) {
                            int sz = (*(static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack()->value))).size();
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::INT, new int(sz)));
                        }
                        else if (getItemOfCurStack()->type == Ovsobj::STRING) {
                            int sz = (*(static_cast<std::string *>(getItemOfCurStack()->value))).size();
                            curStack.pop_back();
                            curStack.push_back(new Ovsobj(Ovsobj::INT, new int(sz)));
                        }
                        else {
                            throw Exception("not countable var", curOp.numLine, curOp.numPos);
                        }
                    }
                    else {
                        if (bigScopes.back().funcs.count(curOp.stringValue)) {
                            stackTrace.push_back({curOp.stringValue, bigScopes.back().funcs[curOp.stringValue].first});
                            bigScopes.push_back(scopes.back());
                            bigScopes.back().type = scope::FUNC;
                            scopes.push_back({scopes.back().funcs, {}, {}, scope::FUNC}); // todo
                            for (auto &now : scopes.back().funcs) {
                                now.second.second = false;
                            }
                            runLevel();
                        }
                        else {
                            throw Exception("Called undefined function", curOp.numLine, curOp.numPos);
                        }
                    }
                    break;
                }
                case Element::CREATE_CLASS: {
                    stackTrace.push_back({curOp.stringValue, bigScopes.back().classes[curOp.stringValue].first});
                    bigScopes.push_back({scopes.back().funcs, scopes.back().classes, {}, scope::CLASS});
                    scopes.push_back({{}, {}, {}, scope::CLASS});
                    auto fields = runLevel();
                    fields["__name__"] = new Ovsobj(Ovsobj::STRING, new std::string(curOp.stringValue));
                    curStack.pop_back();
                    curStack.push_back(new Ovsobj(Ovsobj::USER_CLASS,
                                                  static_cast<void *>(new std::pair<Poliz *, std::map<std::string, Ovsobj *>>(
                                                          {bigScopes.back().classes[curOp.stringValue].first,
                                                           fields}))));

                    break;
                }
                case Element::GET_FIELD: {
                    auto tmp = *static_cast<std::pair<Poliz *, std::map<std::string, Ovsobj *>> *>(getItemOfCurStack()->value);
                    if (tmp.second.count(curOp.stringValue)) {
                        curStack.pop_back();
                        curStack.push_back(tmp.second[curOp.stringValue]);
                    }
                    else {
                        throw Exception("undefined field of class", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::EVAL_METHOD: {
                    if (curOp.stringValue == "push_back" && getItemOfCurStack(1)->type == Ovsobj::LIST) {
                        (*static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack(1)->value)).push_back(
                                getItemOfCurStack(0));
                        curStack.pop_back();
                    }
                    else if (curOp.stringValue == "pop_back" && getItemOfCurStack()->type == Ovsobj::LIST) {
                        if (static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack()->value)->empty()) {
                            throw Exception("pop from empty list", curOp.numLine, curOp.numPos);
                        }
                        static_cast<std::vector<Ovsobj *> *>(getItemOfCurStack()->value)->pop_back();
                        curStack.pop_back();
                    }
                    else if (curOp.stringValue == "split" && getItemOfCurStack()->type == Ovsobj::STRING) {
                        auto tmp = split(*static_cast<std::string *>(getItemOfCurStack()->value), ' ');
                        auto tmp2 = new std::vector<Ovsobj *>;
                        for (auto &now : tmp) {
                            tmp2->push_back(new Ovsobj(Ovsobj::STRING, static_cast<void *> (new std::string(now))));
                        }
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::LIST, static_cast<void *>(tmp2)));
                    }
                    else if (getItemOfCurStack()->type == Ovsobj::USER_CLASS) {
                        auto tid = static_cast<std::pair<Poliz *, std::map<std::string, Ovsobj *>> *>(getItemOfCurStack(
                                curOp.countParams)->value);
                        stackTrace.push_back({curOp.stringValue, tid->first->funcs[curOp.stringValue]});
                        auto className = static_cast<std::string *>(tid->second["__name__"]->value);
                        bigScopes.push_back({{}, {}, tid->second});
                        for (const auto &now : tid->first->funcs) {
                            bigScopes.back().funcs[now.first] = {now.second, false};
                        }
                        bigScopes.back().type = scope::FUNC;
                        scopes.push_back({scopes.back().funcs, {}, {}, scope::FUNC}); // todo
                        for (auto &now : scopes.back().funcs) {
                            now.second.second = false;
                        }
                        curStack.pop_back();
                        runLevel();
                    }
                    else {
                        throw Exception("invalid method", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::GET_VALUE_INT: {
                    void *tmpInt = new int(curOp.intValue);
                    curStack.push_back(new Ovsobj(Ovsobj::INT, tmpInt));
                    break;
                }
                case Element::GET_VALUE_DOUBLE: {
                    void *tmpDouble = new double(curOp.doubleValue);
                    curStack.push_back(new Ovsobj(Ovsobj::DOUBLE, tmpDouble));
                    break;
                }
                case Element::GET_VALUE_STR: {
                    void *tmpString = new std::string(curOp.stringValue);
                    curStack.push_back(new Ovsobj(Ovsobj::STRING, tmpString));
                    break;
                }
                case Element::GET_VALUE_NONE: {
                    curStack.push_back(new Ovsobj(Ovsobj::NONE));
                    break;
                }
                case Element::GET_VALUE_BOOL: {
                    void *tmpBool = new bool(curOp.intValue);
                    curStack.push_back(new Ovsobj(Ovsobj::BOOL, tmpBool));
                    break;
                }
                case Element::CLEAR_STACK: {
                    curStack.pop_back();
                    break;
                }
                case Element::POSITIVE_JMP: {
                    if (getItemOfCurStack()->type == Ovsobj::BOOL) {
                        if (*(static_cast<bool *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            curStack.pop_back();
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack()->type == Ovsobj::INT) {
                        if (*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            curStack.pop_back();
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for comp", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::NEGATIVE_JMP: {
                    if (getItemOfCurStack()->type == Ovsobj::BOOL) {
                        if (!*(static_cast<bool *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            curStack.pop_back();
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack()->type == Ovsobj::INT) {
                        if (!*(static_cast<int *>(getItemOfCurStack()->value))) {
                            i = curOp.posJump - 1;
                            curStack.pop_back();
                            continue;
                        }
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for comp", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::JMP: {
                    i = curOp.posJump - 1;
                    break;
                }
                case Element::CMP_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == Ovsobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   == *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == Ovsobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   == *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == Ovsobj::BOOL) {
                        bool tmp = *static_cast<bool *>(getItemOfCurStack(1)->value)
                                   == *static_cast<bool *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == Ovsobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   == *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw Exception("no compare vars", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::CMP_NOT_EQUAL: {
                    if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                        && getItemOfCurStack(1)->type == Ovsobj::INT) {
                        bool tmp = *static_cast<int *>(getItemOfCurStack(1)->value)
                                   != *static_cast<int *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == Ovsobj::DOUBLE) {
                        bool tmp = *static_cast<double *>(getItemOfCurStack(1)->value)
                                   != *static_cast<double *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == Ovsobj::BOOL) {
                        bool tmp = *static_cast<bool *>(getItemOfCurStack(1)->value)
                                   != *static_cast<bool *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else if (getItemOfCurStack(1)->type == getItemOfCurStack()->type
                             && getItemOfCurStack(1)->type == Ovsobj::STRING) {
                        bool tmp = *static_cast<std::string *>(getItemOfCurStack(1)->value)
                                   != *static_cast<std::string *>(getItemOfCurStack()->value);
                        curStack.pop_back();
                        curStack.pop_back();
                        curStack.push_back(new Ovsobj(Ovsobj::BOOL, new bool(tmp)));
                    }
                    else {
                        throw Exception("no compare vars", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::CMP_LESS: {
                    COMPARE(<);
                    break;
                }
                case Element::CMP_MORE: {
                    COMPARE(>)
                    break;
                }
                case Element::CMP_MORE_EQUAL: {
                    COMPARE(>=)
                    break;
                }
                case Element::CMP_LESS_EQUAL: {
                    COMPARE(<=)
                    break;
                }
                case Element::BEGIN_SCOPE: {
                    scopes.push_back({scopes.back().funcs});
                    for (auto &now : scopes.back().funcs) {
                        now.second.second = false;
                    }
                    break;
                }
                case Element::END_SCOPE: {
                    for (const auto &now : scopes.back().vars) {
                        bigScopes.back().vars.erase(now.first);
                    }
                    for (const auto &now : scopes.back().funcs) {
                        if (now.second.second == 1) {
                            bigScopes.back().funcs.erase(now.first);
                        }
                    }
                    for (const auto &now : scopes.back().classes) {
                        if (now.second.second == 1) {
                            bigScopes.back().classes.erase(now.first);
                        }
                    }
                    scopes.pop_back();
                    break;
                }
                case Element::RETURN_VALUE: {
//                    curStack.push_back({getItemOfCurStack()});
                    i = stackTrace.back().poliz->operations.size() + 1; // чтобы не совпало с jmp
                    break;
                }
                case Element::DEL: {
                    //todo
                    curStack.pop_back();
                    break;
                }
                case Element::XOR_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) ^
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::OR_BIT_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) |
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::AND_BIT_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) &
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::LEFT_SHIFT_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) <<
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::RIGHT_SHIFT_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) >>
                                                                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::PLUS_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) +
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) +
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<double *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::STRING
                             && getItemOfCurStack()->type == Ovsobj::STRING) {
                        std::string value = (*(static_cast<std::string *>(getItemOfCurStack(1)->value)))
                                            + (*(static_cast<std::string *>(getItemOfCurStack()->value)));
                        curStack.pop_back();
                        *static_cast<std::string *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MINUS_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) -
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) -
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<double *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MUL_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) *
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) *
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<double *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                }
                case Element::DIV_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        double value = (double) *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        curStack.back()->type = Ovsobj::DOUBLE;
                        *static_cast<double *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else if (getItemOfCurStack(1)->type == Ovsobj::DOUBLE
                             && getItemOfCurStack(0)->type == Ovsobj::DOUBLE) {
                        double value = *(static_cast<double *>(getItemOfCurStack(1)->value)) /
                                       *(static_cast<double *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        curStack.back()->type = Ovsobj::DOUBLE;
                        *static_cast<double *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MOD_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) %
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::INTDIV_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = *(static_cast<int *>(getItemOfCurStack(1)->value)) /
                                    *(static_cast<int *>(getItemOfCurStack()->value));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::POW_INPLACE: {
                    if (getItemOfCurStack(1)->type == Ovsobj::INT
                        && getItemOfCurStack(0)->type == Ovsobj::INT) {
                        int value = pow(*(static_cast<int *>(getItemOfCurStack(1)->value)),
                                        *(static_cast<int *>(getItemOfCurStack()->value)));
                        curStack.pop_back();
                        *static_cast<int *>(curStack.back()->value) = value;
                        curStack.pop_back();
                    }
                    else {
                        throw Exception("invalid type for operation", curOp.numLine, curOp.numPos);
                    }
                    break;
                }
                case Element::MAKE_LIST: {
                    auto tmp = new Ovsobj(Ovsobj::LIST,
                                          static_cast<void *>(new std::vector<Ovsobj *>));
                    static_cast<std::vector<Ovsobj *> *>(tmp->value)->reserve(curOp.countParams);
                    for (int j = 0; j < curOp.countParams; j++) {
                        static_cast<std::vector<Ovsobj *> *>(tmp->value)->push_back(getItemOfCurStack());
                        curStack.pop_back();
                    }
                    std::reverse(static_cast<std::vector<Ovsobj *> *>(tmp->value)->begin(),
                                 static_cast<std::vector<Ovsobj *> *>(tmp->value)->end());
                    curStack.push_back(tmp);
                    break;
                }
            }
        }
        if (i != stackTrace.back().poliz->operations.size() + 1 + 1) {
            curStack.push_back(new Ovsobj(Ovsobj::NONE));
        }

        for (const auto &now : scopes.back().vars) {
            bigScopes.back().vars.erase(now.first);
        }
        for (const auto &now : scopes.back().funcs) {
            bigScopes.back().funcs.erase(now.first);
        }
        for (const auto &now : scopes.back().classes) {
            bigScopes.back().classes.erase(now.first);
        }
        auto returnValue = scopes.back().vars;
        scopes.pop_back();
        bigScopes.pop_back();
        stackTrace.pop_back();
        return returnValue;
    }

public:
    int run(Poliz *OvsByteCode, std::string name = "") {
        stackTrace.push_back({name, OvsByteCode});
        bigScopes.emplace_back();
        scopes.emplace_back();
        runLevel();
        return 0;
    }

    VM() {}

    VM(std::istream *inputStream, std::ostream *outputStream) : inputStream(inputStream), outputStream(
            outputStream) {}
};

#endif //Ovs_VM_H