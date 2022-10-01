#ifndef LIST_HPP
#define LIST_HPP
#include <iostream>
#include <Type.hpp>
#include <algorithm>
#include <vector>
class List{
    public:
        List(Type&);
        List(Type&&);
        void push(const Type&);
        void push(Type&&);
        void clear();
        int contains(const Type&);
        Type map(Type(*)(Type&, size_t));
        void forEach(void(*)(Type&, size_t));
        Type pop();
        size_t length();
        ~List();
    private:
        std::vector<Type>* _list;
        bool _owns_memory;
};

#endif