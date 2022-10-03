#include<Type.hpp>
void Type::releaseResources(){
    switch (this->_id)
    {
    case TypeId::TEXT:
        this->_text.~basic_string();
        break;
    case TypeId::LIST:
        this->_list.~vector();
        break;
    case TypeId::OBJECT:
        this->_object.~unordered_map<std::string, std::shared_ptr<Type>>();
        break;
    default:
        break;
    }

}
void Type::initializeResources(){
    switch(this->_id){
    case TypeId::TEXT:
        new (&this->_text) std::string();
        break;
    case TypeId::LIST:
        new (&this->_list) std::vector<Type>();
        break;
    case TypeId::OBJECT:
        new (&this->_object) std::unordered_map<std::string, std::shared_ptr<Type>>();
        break;
    default:
        break;
    }
}

Type::Type(const Type& ref){
    this->_id = ref._id;
    switch(this->_id){
    case TypeId::TEXT:
        new (&this->_text) std::string(ref._text);
        break;
    case TypeId::LIST:
        new (&this->_list) std::vector<Type>(ref._list);
        break;
    case TypeId::OBJECT:
        new (&this->_object) std::unordered_map<std::string, std::shared_ptr<Type>>(ref._object);
        break;
    default:
        this->_number = ref._number; /*NULL value, boolean, number*/
        break;
    }
}
Type::Type(Type&& source){
    this->_id = source._id;
    switch(this->_id){
    case TypeId::TEXT:
        new (&this->_text) std::string(std::move(source._text));
        break;
    case TypeId::LIST:
        new (&this->_list) std::vector<Type>(std::move(source._list));
        break;
    case TypeId::OBJECT:
        new (&this->_object) std::unordered_map<std::string, std::shared_ptr<Type>>(std::move(source._object));
        break;
    default:
        this->_number = source._number; /*NULL value, boolean, number*/
        break;
    }
}
Type::Type(){
    this->_id = TypeId::NULL_VALUE;
    this->_number = 0;
}
Type::Type(double value){
    this->_id = TypeId::NUMBER;
    this->_number = value;
}
Type::Type(int value){
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
Type::Type(bool boolean){
    this->_id = TypeId::BOOLEAN;
    this->_number = boolean;
}
Type Type::List(){
    Type _to_return;
    _to_return._id = TypeId::LIST;
    new ( &_to_return._list ) std::vector<Type>();
    return _to_return;
}
Type Type::Object(){
    Type _to_return;
    _to_return._id = TypeId::OBJECT;
    new ( &_to_return._object ) std::unordered_map<std::string, std::shared_ptr<Type>>();
    return _to_return;
}
Type Type::Null(){
    Type _to_return;
    _to_return._id = TypeId::NULL_VALUE;
    return _to_return;
}
Type& Type::operator=(const Type& ref){
    if(this->_id != ref._id){
        this->releaseResources();
        this->_id = ref._id;
        this->initializeResources();
    }
    switch(this->_id){
    case TypeId::TEXT:
        this->_text = ref._text;
        break;
    case TypeId::LIST:
        this->_list = ref._list;
        break;
    case TypeId::OBJECT:
        this->_object = ref._object;
        break;
    default:
        this->_number = ref._number; /*NULL value, boolean, number*/
        break;
    }
    return *this;
}
bool Type::operator==(const Type& object) const{
    switch(this->_id){
        case TypeId::LIST:
            return this->_list == object._list;
        case TypeId::OBJECT:
            return this->_object == object._object;
        case TypeId::TEXT:
            return this->_text == object._text;
        default:
            return this->_number == object._number;
    }
    return this == &object;
}
bool Type::operator!=(const Type& object) const{
    return !(*this == object);
}

Type& Type::operator[](const std::string& key){
    if(this->_id != TypeId::OBJECT) throw std::runtime_error("Type must be an object.");
    auto it = this->_object.find(key);
    if(it == this->_object.end())
        this->_object.insert(std::pair(key, std::make_shared<Type>(Type())));
    return *this->_object[key];
}
Type& Type::operator[](size_t index){
    if(this->_id != TypeId::LIST) throw std::runtime_error("Type must be a list.");
    return this->_list[index];
}
Type::~Type(){
    this->releaseResources();
}
void Type::prettyPrint(std::ostream& stream, const Type& object, size_t depth){
    switch (object._id)
    {
    case TypeId::NUMBER:
        stream << object._number;
        break;
    case TypeId::TEXT:
        stream << '\"' << object._text << '\"';
        break;
    case TypeId::LIST:
        stream  << "[\n";
        for(size_t i = 0 ; i < object._list.size() ; i++){
            stream << std::string(depth + 1, '\t');
            Type::prettyPrint(stream, object._list[i], depth + 1);
            if(i < object._list.size() - 1) stream << ", ";
            stream << '\n';
        }
        stream << std::string(depth - 1, '\t') << ']';
        break;
    case TypeId::OBJECT:{
        stream  << "{\n";
        size_t i = 1;
        for(auto pair : object._object){
            stream  << std::string(depth , '\t') << '\"' << pair.first << '\"' << " : ";
            Type::prettyPrint(stream, *pair.second, depth + 1);
            if(i != object._object.size()) stream << ","; /*Dont print comma on last element*/
            stream << '\n';
            i++;
        }
        stream << std::string(depth - 1, '\t') << '}';
        break;
    }
    case TypeId::NULL_VALUE:
        stream << "null";
        break;
    case TypeId::BOOLEAN:
        stream  << (object._number == 1 ? "true" : "false");
        break;
    }
}
std::ostream& operator<< (std::ostream& stream, const Type& object)
{
    object.prettyPrint(stream, object, 1);
    return stream;
}