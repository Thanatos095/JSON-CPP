#include <Object.hpp>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

Object::Object(Type& ref){
    if(ref._id != OBJECT) throw std::runtime_error("Object is not a map type.");
    this->_map = &ref._object;
    this->_owns_memory = false;
}
Object::Object(Type&& obj){
    if(obj._id != OBJECT) throw std::runtime_error("Object is not a map type.");
    this->_map = new std::unordered_map<std::string, std::shared_ptr<Type>>(std::move(obj._object));
    this->_owns_memory = true;
}
std::vector<std::string> Object::lexer(const std::string& data){
    
    auto contains = [](const std::vector<char>& tokens, char value){
        auto it = std::find(tokens.begin(), tokens.end(), value);
        return it == tokens.end() ? -1 : it - tokens.begin();
    };

    std::vector<char> symbols = {'{', '}', '[', ']', ',', ':'};    
    std::vector<std::string> tokens;

    for(size_t i = 0 ; i < data.length() ; i++){
        // char c = data[i];
        if(isspace(data[i])) continue;
        else if(contains(symbols, data[i]) != -1){
            tokens.push_back(std::string(1, data[i]));
        }
        else if(data[i] == '\"'){
            std::string token = "\"";
            i++;
            while(data[i] != '\"'){               
                token += data[i++];
            }
            token += '\"';
            trim(token);
            tokens.push_back(token);
        }
        else{
            std::string token;
            while(contains(symbols, data[i]) == -1) token += data[i++];
            i--;
            trim(token);
            tokens.push_back(token);
        }
    }
    return tokens;
}

/* This function is written in reference to a push down automata*/
Type Object::parser(const std::vector<std::string>& tokens){
    auto isString = [](const std::string& token){
        bool str = token[0] == '\"' && token[token.size() - 1] == '\"'; 
        if(!str) return false;
        for(size_t i = 1 ;i < token.size() - 1 ; i++){
            if(token[i] == '\\') {
                if(++i >= token.size())
                    return false;
                else{
                    switch(token[i]){
                        case '\\': break;
                        case '/': break;
                        case '\b': break;
                        case '\f': break;
                        case '\n': break;
                        case '\r': break;
                        case '\t': break;
                        default : return false;
                    }
                }
            }
        } 
        return true;
    };
    auto isNumeric = [](const std::string& token){
        std::regex number_literal("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
        return std::regex_match(token, number_literal);
    };
    auto isNull = [](const std::string& token){
        return token == "null";
    };
    auto isBoolean = [](const std::string& token){
        return token == "true" || token == "false";
    };
    Type JSON;
    std::stack<Type*> stack;
    std::string activeKey;
    stack.push(&JSON);
    int state = 1;
    for(size_t i = 0 ; i < tokens.size() ; i++){
        switch(state){
            case 1:
                 if(tokens[i] == "{"){
                    *stack.top()= Type::Object();
                    state = 2;
                }
                else if(tokens[i] == "["){
                    *stack.top()= Type::List();
                    state = 6;
                }
                else state = -1;
                break;
            case 2:
                if(isString(tokens[i])){
                    activeKey  = tokens[i].substr(1, tokens[i].length() - 2);
                    state = 3;
                }
                else if(tokens[i] == "}"){
                    if(stack.top()->_id == OBJECT){
                        stack.pop();
                        state = 5;
                    } 
                    else state = -1;

                }
                else state = -1;
                break;
            case 3:
                if(tokens[i] == ":") state = 4;
                else state = -1;
                break;
            case 4: {
                bool isStr = false, isNum = false, isBool = false, isNulled = false;
                if(tokens[i] == "{"){
                    (*stack.top())[activeKey] = Type::Object();
                    stack.push(&(*stack.top())[activeKey]);
                    state = 2;
                }
                else if(tokens[i] == "["){
                    (*stack.top())[activeKey] = Type::List();
                    stack.push(&(*stack.top())[activeKey]);
                    state = 6;
                }
                else if((isStr = isString(tokens[i])) ||
                        (isNum = isNumeric(tokens[i])) ||
                        (isBool = isBoolean(tokens[i])) ||
                        (isNulled = isNull(tokens[i]))){
                            Type object;
                            if(isStr) object = tokens[i].substr(1, tokens[i].length() - 2); /*Trimming the quotes*/
                            else if(isNum) object = std::atof(tokens[i].data());
                            else if(isBool) object = tokens[i][0] == 'f' ? false : true;
                            else if(isNulled) object = Type::Null();
                            if(stack.top()->_id == OBJECT)
                            {
                                (*stack.top())[activeKey] = object;
                            }   
                            else if(stack.top()->_id == LIST){
                                stack.top()->_list.push_back(object);
                            }         
                            state = 5;
                            
                        }
                else state = -1;
                break;
            }
            case 5:
                if(tokens[i] == ","){
                    if(stack.top()->_id == OBJECT){
                        state = 2;
                    }
                    else if(stack.top()->_id == LIST){
                        state = 6;
                    }
                }
                else if(tokens[i] == "}"){
                    if(stack.top()->_id == OBJECT){
                        stack.pop();
                    } 
                    else state = -1;
                    /* remain at state 5*/
                }
                else if(tokens[i] == "]"){
                    if(stack.top()->_id == LIST){
                        stack.pop();
                    } 
                    else state = -1;
                    /* remain at state 5*/
                }
                else state = -1;
                break;
            case 6: {
                bool isStr = false, isNum = false, isBool = false, isNulled = false;
                if(tokens[i] == "{")
                {
                    stack.top()->_list.push_back(Type::Object());
                    stack.push(&(*stack.top())[stack.top()->_list.size() - 1]);
                    state = 2;
                }
                else if(tokens[i] == "["){
                    stack.top()->_list.push_back(Type::List());
                    stack.push(&(*stack.top())[stack.top()->_list.size() - 1]);
                    /*Remain at state 6*/
                }
                else if(tokens[i] == "]"){
                    if(stack.top()->_id == LIST){
                        stack.pop();
                        state = 5;
                    } 
                    else state = -1;
                }
                else if((isStr = isString(tokens[i])) ||
                        (isNum = isNumeric(tokens[i])) ||
                        (isBool = isBoolean(tokens[i])) ||
                        (isNulled = isNull(tokens[i]))){
                            Type object;
                            if(isStr) object = tokens[i].substr(1, tokens[i].length() - 2); /*Trimming the quotes*/
                            else if(isNum) object = std::atof(tokens[i].data());
                            else if(isBool) object = tokens[i][0] == 'f' ? false : true;
                            else if(isNulled) object = Type::Null();
                            if(stack.top()->_id == OBJECT)
                            {
                                (*stack.top())[activeKey] = object;
                            }   
                            else if(stack.top()->_id == LIST){

                                stack.top()->_list.push_back(object);
                            }         
                            state = 5;
                        }
                else state = -1;
                break;
            }
            case -1: /*Error state*/
                throw std::runtime_error("Error while parsing: " + tokens[i - 1]);
        }
    }
    assert(stack.empty());
    return JSON;
}
Type Object::FromJSON(const std::string& file_name){
    std::ifstream _handle(file_name);
    if(!_handle.is_open()) throw std::runtime_error("File : \"" + file_name + "\" could not be opened.");
    std::string buffer;
    std::string temp;
    while (std::getline(_handle, temp))
    {
        buffer += temp;
    }
    _handle.close();
    std::vector<std::string> tokens = Object::lexer(buffer);
    return Object::parser(tokens);
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