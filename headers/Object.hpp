#ifndef OBJECT_HPP
#define OBJECT_HPP
#include<iostream>
#include<memory>
#include<stack>
#include<algorithm>
#include<string>
#include<unordered_map>
#include<Type.hpp>
#include<fstream>
#include<regex>
#include<cassert>
class Object{
    public:
        Object(Type&);
        Object(Type&&);
        static Type FromJSON(const std::string&);
        static void ToJSON(Type& object, const std::string&);
        bool contains(const std::string&);
        void remove(const std::string&);
        Type keys();
        ~Object();
    private:
        class Lexer{
                Lexer(const std::string&);
                const std::vector<std::string>& get();
                static bool isString(const std::string&);
            private:

                std::vector<std::pair<std::string, TypeId>> tokens;
        };
        static std::vector<std::string> lexer(const std::string& data);
        static Type parser(const std::vector<std::string>& tokens);
        std::unordered_map<std::string, std::shared_ptr<Type>>* _map;
        bool _owns_memory;
};

#endif