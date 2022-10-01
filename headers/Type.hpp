#ifndef TYPE_HPP
#define TYPE_HPP
#include<iostream>
#include<unordered_map>
#include<vector>
#include<memory>

#define field(X,...) std::make_pair(std::string(#X), Type(__VA_ARGS__))

enum class TypeId{
    NUMBER = 1,
    TEXT = 2,
    LIST = 3,
    OBJECT = 4,
    NULL_VALUE = 5,
    BOOLEAN = 6
};



class Type
{
private:
    union{
        double _number;
        std::string _text;
        std::vector<Type> _list;
        std::unordered_map<std::string, std::shared_ptr<Type>> _object;
    };
    TypeId _id;
    friend class List;
    friend class Object;
    void releaseResources();
    void initializeResources();
    typedef Type (*function_type)(const Type& _first, const Type& _second);
    static std::unordered_map<size_t, function_type> _function_map;
    Type(); 
    static void prettyPrint(std::ostream& stream, const Type& object, size_t depth);
public:
    Type(const Type&);
    Type(Type&&);
    Type(double); /*Numeric types*/
    Type(int);
    Type(const std::string&);
    Type(const char*);
    Type(const std::vector<Type>&);
    Type(const std::initializer_list<Type>&);
    Type(const std::initializer_list<std::pair<std::string, Type>>&);
    Type(bool boolean);
    static Type List(); /* Empty List */
    static Type Object(); /* Empty Object*/
    static Type Null();
    Type& operator=(const Type&);
    bool operator==(const Type& object);
    Type& operator[](const std::string&);
    Type& operator[](size_t index);
    friend std::ostream& operator<< (std::ostream& stream, const Type& object);
    ~Type();
};

#endif