#ifndef Ovs_OvsOBJ_H
#define Ovs_OvsOBJ_H

class Ovsobj {
public:
    enum {
        INT, DOUBLE, BOOL, STRING, LIST, DICT, USER_CLASS, NONE
    } type;
    void *value;

    Ovsobj() {}

    Ovsobj(decltype(type) type, void *value = nullptr) : type(type), value(value) {};

};

#endif //Ovs_OvsOBJ_H
