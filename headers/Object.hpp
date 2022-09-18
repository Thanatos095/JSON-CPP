#ifndef OBJECT_HPP
#define OBJECT_HPP
#include<iostream>
#include<memory>
#include<stack>
#include<algorithm>
#include<unordered_map>
#include<Type.hpp>
#include<fstream>
#include<regex>
#include<cassert>
class Object{
    public:
        Object(Type&);
        static Type FromJSON(const std::string&);
        void toJSON(const std::string&);
        void remove(const std::string&);
        Type keys();
        ~Object();
    private:
        template<typename T>
        static int contains(std::vector<T>& tokens, const T& value);
        static std::vector<std::string> lexer(const std::string& data);
        static Type parser(const std::vector<std::string>& tokens);
        std::unordered_map<std::string, std::shared_ptr<Type>>* _map;  
};

#endif