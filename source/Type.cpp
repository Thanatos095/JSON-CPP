#include<Type.hpp>
void Type::releaseResources(){
    switch (this->_id)
    {
    case TypeId::NUMBER:
        break;
    case TypeId::TEXT:
        this->_text.~basic_string();
        break;
    case TypeId::LIST:
        this->_list.~vector();
        break;
    case TypeId::OBJECT:
        this->_object.~unordered_map<std::string, std::shared_ptr<Type>>();
        break;
    }
}
void Type::initializeResources(){
    switch(this->_id){
    case TypeId::NUMBER:
        break;
    case TypeId::TEXT:
        new (&this->_text) std::string();
        break;
    case TypeId::LIST:
        new (&this->_list) std::vector<Type>();
        break;
    case TypeId::OBJECT:
        new (&this->_object) std::unordered_map<std::string, std::shared_ptr<Type>>();
        break;
    }
}
Type computeNumberSum(const Type& _first, const Type& _second){
    return _first._number + _second._number;
}
Type computeNumberDiff(const Type& _first, const Type& _second){
    return _first._number - _second._number;
}
Type computeNumberProd(const Type& _first, const Type& _second){
    return _first._number * _second._number;
}
Type computeNumberDiv(const Type& _first, const Type& _second){
    if(_second._number == 0) throw std::runtime_error("Division by 0");
    return _first._number / _second._number;
}
Type computeNumberRem(const Type& _first, const Type& _second){
    return int(_first._number) % int(_second._number);
}
Type computeStringConcat(const Type& _first, const Type& _second){
    return _first._text + _second._text;
}
Type computeListConcat(const Type& _first, const Type& _second){
    std::vector<Type> _result(_first._list);
    _result.insert(_result.end(), _second._list.begin(), _second._list.end());
    return _result;
}
std::unordered_map<size_t, Type::function_type> Type::_function_map = {
    {concatenateKeys(NUMBER, NUMBER, SUM), computeNumberSum},
    {concatenateKeys(NUMBER, NUMBER, DIFF), computeNumberDiff},
    {concatenateKeys(NUMBER, NUMBER, PROD), computeNumberProd},
    {concatenateKeys(NUMBER, NUMBER, DIV), computeNumberDiv},
    {concatenateKeys(NUMBER, NUMBER, REM), computeNumberRem},
    {concatenateKeys(TEXT, TEXT, SUM), computeStringConcat},
    {concatenateKeys(LIST, LIST, SUM), computeListConcat}
};
Type::Type(const Type& ref){
    this->_id = ref._id;
    switch(this->_id){
    case TypeId::NUMBER:
        this->_number = ref._number;
        break;
    case TypeId::TEXT:
        new (&this->_text) std::string(ref._text);
        break;
    case TypeId::LIST:
        new (&this->_list) std::vector<Type>(ref._list);
        break;
    case TypeId::OBJECT:
        new (&this->_object) std::unordered_map<std::string, std::shared_ptr<Type>>(ref._object);
        break;
    }
}
Type::Type(Type&& source){
    this->_id = source._id;
    switch(this->_id){
    case TypeId::NUMBER:
        this->_number = source._number;
        break;
    case TypeId::TEXT:
        new (&this->_text) std::string(std::move(source._text));
        break;
    case TypeId::LIST:
        new (&this->_list) std::vector<Type>(std::move(source._list));
        break;
    case TypeId::OBJECT:
        new (&this->_object) std::unordered_map<std::string, std::shared_ptr<Type>>(std::move(source._object));
        break;
    }
}
Type::Type(){

}
template<typename T>
Type::Type(T value){
    this->_id = TypeId::NUMBER;
    this->_number = value;
}
Type::Type(const std::string& value){
    this->_id = TypeId::TEXT;
    new ( &this->_text ) std::string(value);
}
Type::Type(const char* value){
    this->_id = TypeId::TEXT;
    new ( &this->_text ) std::string(value);
}
Type::Type(const std::vector<Type>& list){
    this->_id = TypeId::LIST;
    new ( &this->_list ) std::vector<Type>(list);
}
Type::Type(const std::initializer_list<Type>& list){
    this->_id = TypeId::LIST;
    new ( &this->_list ) std::vector<Type>(list);
}
Type::Type(const std::initializer_list<std::pair<std::string, Type>>& pairs){
    this->_id = TypeId::OBJECT;
    new ( &this->_object ) std::unordered_map<std::string, std::shared_ptr<Type>>();
    for (size_t i = 0; i < pairs.size() ; i++)
    {
        this->_object.insert(std::pair<std::string, std::shared_ptr<Type>>((pairs.begin() + i)->first, std::make_shared<Type>((pairs.begin() + i)->second)));
    }
}
Type Type::List(){
    Type _to_return;
    _to_return._id = LIST;
    new ( &_to_return._list ) std::vector<Type>();
    return _to_return;
}
Type Type::Object(){
    Type _to_return;
    _to_return._id = OBJECT;
    new ( &_to_return._object ) std::unordered_map<std::string, std::shared_ptr<Type>>();
    return _to_return;
}
Type& Type::operator=(const Type& ref){
    if(this->_id != ref._id){
        this->releaseResources();
        this->_id = ref._id;
        this->initializeResources();
    }
    switch(this->_id){
    case TypeId::NUMBER:
        this->_number = ref._number;
        break;
    case TypeId::TEXT:
        this->_text = ref._text;
        break;
    case TypeId::LIST:
        this->_list = ref._list;
        break;
    case TypeId::OBJECT:
        this->_object = ref._object;
        break;
    }
    return *this;
}
bool Type::operator==(const Type& object){
    return this == &object;
}

Type Type::operator+(const Type& object){
    size_t key = concatenateKeys(this->_id, object._id, SUM);
    auto it = Type::_function_map.find(key);
    if(it == Type::_function_map.end()) throw std::runtime_error("This operation is not valid/defined.");
    return it->second(*this, object);
}
Type Type::operator-(const Type& object){
    size_t key = concatenateKeys(this->_id, object._id, DIFF);
    auto it = Type::_function_map.find(key);

    if(it == Type::_function_map.end()) throw std::runtime_error("This operation is not valid/defined.");
    return it->second(*this, object);
}
Type Type::operator*(const Type& object){
    size_t key = concatenateKeys(this->_id, object._id, PROD);
    auto it = Type::_function_map.find(key);
    if(it == Type::_function_map.end()) throw std::runtime_error("This operation is not valid/defined.");
    return it->second(*this, object);
}
Type Type::operator/(const Type& object){
    size_t key = concatenateKeys(this->_id, object._id, DIV);
    auto it = Type::_function_map.find(key);
    if(it == Type::_function_map.end()) throw std::runtime_error("This operation is not valid/defined.");
    return it->second(*this, object);
}
Type Type::operator%(const Type& object){
    size_t key = concatenateKeys(this->_id, object._id, REM);
    auto it = Type::_function_map.find(key);
    if(it == Type::_function_map.end()) throw std::runtime_error("This operation is not valid/defined.");
    return it->second(*this, object);
}

Type& Type::operator[](const std::string& key){
    if(this->_id != OBJECT) throw std::runtime_error("Type must be an object.");
    auto it = this->_object.find(key);
    if(it == this->_object.end()) throw std::runtime_error("The key: \"" + key + "\" was not found in the map.");
    return *it->second;
}
Type& Type::operator[](size_t index){
    if(this->_id != LIST) throw std::runtime_error("Type must be a list.");
    return this->_list[index];
}
Type::~Type(){
    this->releaseResources();
}
std::ostream& operator<< (std::ostream& stream, const Type& object)
{
    switch (object._id)
    {
    case TypeId::NUMBER:
        stream << object._number;
        break;
    case TypeId::TEXT:
        stream << '\"' << object._text << '\"';
        break;
    case TypeId::LIST:
        stream << '[';
        for(size_t i = 0 ; i < object._list.size() ; i++){
            stream << object._list[i];
            if(i < object._list.size() - 1) stream << ", ";
        }
        stream << ']';
        break;
    case TypeId::OBJECT:
        stream << "{\n";
        for(auto pair : object._object){
            stream << "     " << '\"' << pair.first << '\"' << " : " << *pair.second << ",\n";
        }
        stream << "\n}";
        break;
    }
    return stream;
}

