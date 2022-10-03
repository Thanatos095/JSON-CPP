#ifndef OBJECT_HPP
#define OBJECT_HPP
#include<iostream>
#include<memory>
#include<stack>
#include<vector>
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
            public:
                Lexer(std::ifstream& handle);
                const std::vector<std::pair<std::string, int>>& get();
            private:
                static bool isNumeric(const std::string&);
                static int contains(const std::vector<char>& tokens, char value);
                static void trim(std::string&);
                std::ifstream& _handle;
                std::vector<std::pair<std::string, int>> _tokens;
                std::vector<char> _symbols;
                std::string _buffer;
        };
        static Type parser(Lexer&);
        std::unordered_map<std::string, std::shared_ptr<Type>>* _map;
        bool _owns_memory;
};

#endif