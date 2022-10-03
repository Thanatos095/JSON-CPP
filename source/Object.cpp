#include <Object.hpp>

Object::Object(Type& ref){
    if(ref._id != TypeId::OBJECT) throw std::runtime_error("Object is not a map type.");
    this->_map = &ref._object;
    this->_owns_memory = false;
}
Object::Object(Type&& obj){
    if(obj._id != TypeId::OBJECT) throw std::runtime_error("Object is not a map type.");
    this->_map = new std::unordered_map<std::string, std::shared_ptr<Type>>(std::move(obj._object));
    this->_owns_memory = true;
}
/* This function is written in reference to a push down automata*/
Type Object::parser(Lexer& lexer){
    Type JSON;
    std::stack<Type*> stack;
    std::string activeKey;
    stack.push(&JSON);
    int state = 1;
    while(true){
        const std::vector<std::pair<std::string, int>>& tokens = lexer.get();
        if(tokens.empty()) break;

        for(size_t i = 0 ; i < tokens.size() ; i++){
            switch(state){
                case 1:
                    if(tokens[i].first == "{"){
                        *stack.top()= Type::Object();
                        state = 2;
                    }
                    else if(tokens[i].first == "["){
                        *stack.top()= Type::List();
                        state = 6;
                    }
                    else state = -1;
                    break;
                case 2:
                    if(tokens[i].second == int(TypeId::TEXT)){
                        activeKey  = tokens[i].first.substr(1, tokens[i].first.length() - 2);
                        state = 3;
                    }
                    else if(tokens[i].first == "}"){
                        if(stack.top()->_id == TypeId::OBJECT){
                            stack.pop();
                            state = 5;
                        } 
                        else state = -1;

                    }
                    else state = -1;
                    break;
                case 3:
                    if(tokens[i].first == ":") state = 4;
                    else state = -1;
                    break;
                case 4: {
                    bool isStr = false, isNum = false, isBool = false, isNulled = false;
                    if(tokens[i].first == "{"){
                        (*stack.top())[activeKey] = Type::Object();
                        stack.push(&(*stack.top())[activeKey]);
                        state = 2;
                    }
                    else if(tokens[i].first == "["){
                        (*stack.top())[activeKey] = Type::List();
                        stack.push(&(*stack.top())[activeKey]);
                        state = 6;
                    }
                    else if((isStr = tokens[i].second == int(TypeId::TEXT)) ||
                            (isNum = tokens[i].second == int(TypeId::NUMBER)) ||
                            (isBool = tokens[i].second == int(TypeId::BOOLEAN)) ||
                            (isNulled = tokens[i].second == int(TypeId::NULL_VALUE))){
                                Type object;
                                if(isStr) object = tokens[i].first.substr(1, tokens[i].first.length() - 2); /*Trimming the quotes*/
                                else if(isNum) object = std::atof(tokens[i].first.data());
                                else if(isBool) object = tokens[i].first[0] == 'f' ? false : true;
                                else if(isNulled) object = Type::Null();
                                if(stack.top()->_id == TypeId::OBJECT)
                                {
                                    (*stack.top())[activeKey] = object;
                                }   
                                else if(stack.top()->_id == TypeId::LIST){
                                    stack.top()->_list.push_back(object);
                                }         
                                state = 5;
                                
                            }
                    else state = -1;
                    break;
                }
                case 5:
                    if(tokens[i].first == ","){
                        if(stack.top()->_id == TypeId::OBJECT){
                            state = 2;
                        }
                        else if(stack.top()->_id == TypeId::LIST){
                            state = 6;
                        }
                    }
                    else if(tokens[i].first == "}"){
                        if(stack.top()->_id == TypeId::OBJECT){
                            stack.pop();
                        } 
                        else state = -1;
                        /* remain at state 5*/
                    }
                    else if(tokens[i].first == "]"){
                        if(stack.top()->_id == TypeId::LIST){
                            stack.pop();
                        } 
                        else state = -1;
                        /* remain at state 5*/
                    }
                    else state = -1;
                    break;
                case 6: {
                    bool isStr = false, isNum = false, isBool = false, isNulled = false;
                    if(tokens[i].first == "{")
                    {
                        stack.top()->_list.push_back(Type::Object());
                        stack.push(&(*stack.top())[stack.top()->_list.size() - 1]);
                        state = 2;
                    }
                    else if(tokens[i].first == "["){
                        stack.top()->_list.push_back(Type::List());
                        stack.push(&(*stack.top())[stack.top()->_list.size() - 1]);
                        /*Remain at state 6*/
                    }
                    else if(tokens[i].first == "]"){
                        if(stack.top()->_id == TypeId::LIST){
                            stack.pop();
                            state = 5;
                        } 
                        else state = -1;
                    }
                    else if((isStr = tokens[i].second == int(TypeId::TEXT)) ||
                            (isNum = tokens[i].second == int(TypeId::NUMBER)) ||
                            (isBool = tokens[i].second == int(TypeId::BOOLEAN)) ||
                            (isNulled = tokens[i].second == int(TypeId::NULL_VALUE))){
                                Type object;
                                if(isStr) object = tokens[i].first.substr(1, tokens[i].first.length() - 2); /*Trimming the quotes*/
                                else if(isNum) object = std::atof(tokens[i].first.data());
                                else if(isBool) object = tokens[i].first[0] == 'f' ? false : true;
                                else if(isNulled) object = Type::Null();
                                if(stack.top()->_id == TypeId::OBJECT)
                                {
                                    (*stack.top())[activeKey] = object;
                                }   
                                else if(stack.top()->_id == TypeId::LIST){

                                    stack.top()->_list.push_back(object);
                                }         
                                state = 5;
                            }
                    else state = -1;
                    break;
                }
                case -1: /*Error state*/
                    throw std::runtime_error("Error while parsing: " + tokens[i - 1].first);
            }
        }
    }
    assert(stack.empty());
    return JSON;
}
Type Object::FromJSON(const std::string& file_name){
    std::ifstream _handle(file_name);
    Lexer lexer(_handle);
    Type to_return = parser(lexer);
    _handle.close();
    return to_return;
}

void Object::ToJSON(Type& object,const std::string& file_name){
    std::ofstream _handle(file_name);
    _handle << object;
    _handle.close();
}
void Object::remove(const std::string& key){
    auto it = this->_map->find(key);
    if(it == this->_map->end()) throw std::runtime_error("The key: \"" + key + "\" was not found in the map.");
    this->_map->erase(it);
}
Type Object::keys(){
    Type list = Type::List();
    for(auto pair : *this->_map){
        list._list.push_back(pair.first);
    }
    return list;
}
bool Object::contains(const std::string& key){
    auto it = this->_map->find(key);
    return it != this->_map->end();
}

Object::~Object(){
    if(this->_owns_memory) delete this->_map;
    this->_map = nullptr;
}