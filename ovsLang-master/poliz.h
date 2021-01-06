#ifndef Ovs_POLIZ_H
#define Ovs_POLIZ_H

#include <vector>
#include <string>
#include <unordered_map>
#include <map>


class Element {
public:
    int countParams = 0; // TODO возможно можно перенести
    enum PrimeFuncs {
        OR, AND, NOT,
        XOR, OR_BIT,
        POSITIVE_JMP, NEGATIVE_JMP, JMP,
        AND_BIT, SHIFT_LEFT_BIN, SHIFT_RIGHT_BIN,
        PLUS, MINUS, MUL, DIV, MOD, INTDIV,
        UNAR_PLUS, UNAR_MINUS, UNAR_TILDA,
        POW, COPY, INIT_PARAM,
        GET_VALUE, INDEX_VALUE,
        DEF_FUNC,
        CALL_FUNC, CREATE_CLASS,
        GET_FIELD, EVAL_METHOD,
        DEF_CLASS,
        GET_VALUE_INT, GET_VALUE_DOUBLE, GET_VALUE_STR,
        GET_VALUE_NONE, GET_VALUE_BOOL,
        CLEAR_STACK, DEL,
        CMP_EQUAL, CMP_NOT_EQUAL, CMP_LESS, CMP_MORE, CMP_MORE_EQUAL, CMP_LESS_EQUAL,
        END_SCOPE,
        IMPORT,
        BEGIN_SCOPE,
        RETURN_VALUE,
        PLUS_INPLACE, MINUS_INPLACE, POW_INPLACE,
        MUL_INPLACE, DIV_INPLACE, INTDIV_INPLACE, MOD_INPLACE,
        OR_BIT_INPLACE, AND_BIT_INPLACE,
        XOR_INPLACE, LEFT_SHIFT_INPLACE,
        RIGHT_SHIFT_INPLACE, MAKE_LIST
    } TYPE;
    std::string toStr(){
        switch (TYPE) {
            case OR: return "OR";
            case AND: return "AND";
            case NOT: return "NOT";
            case XOR: return "XOR";
            case OR_BIT: return "OR_BIT";
            case POSITIVE_JMP: return std::string("JMP_IF_TRUE ") + std::to_string(posJump);
            case NEGATIVE_JMP: return std::string("JMP_IF_FALSE ") + std::to_string(posJump);
            case JMP: return std::string("JMP ") + std::to_string(posJump);
            case AND_BIT: return "AND_BIT";
            case SHIFT_LEFT_BIN: return "SHIFT_LEFT_BIN";
            case SHIFT_RIGHT_BIN: return "SHIFT_RIGHT_BIN";
            case PLUS: return "PLUS";
            case MINUS: return "MINUS";
            case MUL: return "MULTIPLY";
            case DIV: return "DIV";
            case MOD: return "MOD";
            case INTDIV: return "INTDIV";
            case UNAR_PLUS: return "UNAR_PLUS";
            case UNAR_MINUS: return "UNAR_MINUS";
            case UNAR_TILDA: return "UNAR_TILDA";
            case POW: return "POW";
            case COPY: return "COPY";
            case INIT_PARAM: return "INIT_PARAM";
            case GET_VALUE: return std::string("INT_VALUE ") + stringValue;
            case INDEX_VALUE: return "INDEX_VALUE";
            case DEF_FUNC: return std::string("DEFINE FUNCTION ") + stringValue;
            case CALL_FUNC: return std::string("CALL ") + stringValue;
            case CREATE_CLASS: return "\n";
            case GET_FIELD: return "";
            case EVAL_METHOD: return std::string("EVAL_METHOD ") + stringValue;
            case GET_VALUE_INT: return std::string("PUSH_INT_CONST ") + std::to_string(intValue);
            case GET_VALUE_DOUBLE: return std::string("PUSH_DOUBLE_CONST ") + std::to_string(doubleValue);
            case GET_VALUE_STR: return std::string("PUSH_STRING_CONST ") + stringValue;
            case GET_VALUE_NONE: return "PUSH_NONE";
            case GET_VALUE_BOOL: return std::string("PUSH_BOOL_CONST ") + std::to_string(intValue);
            case CLEAR_STACK: return "POP";
            case DEL: return "DEL";
            case CMP_EQUAL: return "CMP_EQUAL";
            case CMP_NOT_EQUAL: return "CMP_NOT_EQUAL";
            case CMP_LESS: return "CMP_LESS";
            case CMP_MORE: return "CMP_MORE";
            case CMP_MORE_EQUAL:return "CMP_MORE_EQUAL";
            case CMP_LESS_EQUAL: return "CMP_LESS_EQUAL";
            case END_SCOPE: return "END_BLOCK";
            case BEGIN_SCOPE: return "BEGIN_BLOCK";
            case RETURN_VALUE: return "RETURN";
            case PLUS_INPLACE: return "PLUS_INPLACE";
            case MINUS_INPLACE: return "MINUS_INPLACE";
            case POW_INPLACE: return "POW_INPLACE";
            case MUL_INPLACE: return "MUL_INPLACE";
            case DIV_INPLACE: return "DIV_INPLACE";
            case INTDIV_INPLACE: return "INTDIV_INPLACE";
            case MOD_INPLACE: return "MOD_INPLACE";
            case OR_BIT_INPLACE: return "OR_BIT_";
            case AND_BIT_INPLACE: return "AND_BIT_INPLACE";
            case XOR_INPLACE: return "XOR_INPLACE";
            case LEFT_SHIFT_INPLACE: return "LEFT_SHIFT_INPLACE";
            case RIGHT_SHIFT_INPLACE: return "RIGHT_SHIFT_INPLACE";
            case MAKE_LIST: return "MAKE_VECTOR";
        }
    }
    int posJump = 0;
    int intValue = 0;
    double doubleValue = 0.0; // TODO можно переписать на объединение
    std::string stringValue = "";
    int numLine, numPos;
};


class Poliz {
public:
    std::map<std::string, Poliz *> funcs;
    std::map<std::string, Poliz *> classes;
    std::vector<Element> operations;
};


#endif //Ovs_POLIZ_H
