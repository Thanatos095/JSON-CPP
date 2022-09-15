#ifndef TYPE_HPP
#define TYPE_HPP
#include<iostream>
#include<unordered_map>
#include<vector>
#include<memory>

#define field(X,...) (std::make_pair(#X, Type(__VA_ARGS__)))

enum TypeId{
    NUMBER = 1,
    TEXT = 2,
    LIST = 3,
    OBJECT = 4
};
enum Operations{
    SUM = 1,
    DIFF = 2,
    PROD = 3,
    DIV = 4, 
    REM = 5
};

inline size_t concatenateKeys(size_t a, size_t b, size_t c){
    size_t key = a * 100 + b * 10 + c;
    return key;
}

class Type
{
private:
    union{
        float _number;
        std::string _text;
        std::vector<Type> _list;
        std::unordered_map<std::string, std::shared_ptr<Type>> _object;

    };
    TypeId _id;

    friend Type computeNumberSum(const Type& _first, const Type& _second);
    friend Type computeNumberDiff(const Type& _first, const Type& _second);
    friend Type computeNumberProd(const Type& _first, const Type& _second);
    friend Type computeNumberDiv(const Type& _first, const Type& _second);
    friend Type computeNumberRem(const Type& _first, const Type& _second);
    friend Type computeStringConcat(const Type& _first, const Type& _second);
    friend Type computeListConcat(const Type& _first, const Type& _second);
    friend class List;
    void releaseResources();
    typedef Type (*function_type)(const Type& _first, const Type& _second);
    static std::unordered_map<size_t, function_type> _function_map;
    Type();
public:
    Type(const Type&);
    Type(Type&&);
    template<typename T>
    Type(T); /*Numeric types*/
    Type(const std::string&);
    Type(const char*);
    Type(const std::vector<Type>&);
    Type(const std::initializer_list<Type>&);
    Type(const std::initializer_list<std::pair<std::string, Type>>&);
    static Type List(); /* Empty List */
    static Type Object(); /* Empty Object*/
    Type& operator=(const Type&);
    bool operator==(const Type& object);
    Type operator+(const Type& object);
    Type operator-(const Type& object);
    Type operator*(const Type& object);
    Type operator/(const Type& object);
    Type operator%(const Type& object);
    Type& operator[](const std::string&);
    friend std::ostream& operator<< (std::ostream& stream, const Type& object);
    ~Type();
};

#endif