#include <Object.hpp>

#define FALSE_LENGTH 5
#define TRUE_LENGTH 4
#define NULL_LENGTH 4
Object::Object(Type& ref){
    if(ref._id != OBJECT) throw std::runtime_error("Object is not a map type.");
    this->_map = &ref._object;
}
template<typename T>
int Object::contains(std::vector<T>& tokens, const T& value){
    auto it = std::find(tokens.begin(), tokens.end(), value);
    return it == tokens.end() ? -1 : it - tokens.begin();
}

std::vector<std::string> Object::lexer(const std::string& data){
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
                if(data[i] == '\\'){
                    token += data[++i];
                    i++;
                }
                else{
                    token += data[i++];
                }
            }
            token += '\"';
            tokens.push_back(token);
        }
        else{
            std::string token;
            while(contains(symbols, data[i]) == -1) token += data[i++];
            i--;
            tokens.push_back(token);
        }
    }
    return tokens;
}
enum class States{
    OBJECT,
    LIST
};




/* This function is return in reference to a push down automata*/
Type Object::parser(const std::vector<std::string>& tokens){
    auto isString = [](const std::string& token){
        std::regex string_literal(R"(\"\w*\")");
        return std::regex_match(token, string_literal);
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
    Type JSON = Type::Object();
    std::stack<Type*> stack;
    std::string activeKey;
    assert(tokens[0] == "{");
    stack.push(&JSON);
    int state = 2;
    for(size_t i = 1 ; i < tokens.size() ; i++){
        const std::string& token = tokens[i];
        std::cout << JSON << '\n';
        switch(state){
            case 2:
                if(isString(tokens[i])){
                    std::string temp = tokens[i].substr(1, tokens[i].length() - 2);
                    activeKey = temp;
                    state = 3;
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
                            std::string temp = tokens[i];
                            if(isStr) temp = tokens[i].substr(1, tokens[i].length() - 2); /*Trimming the quotes*/
                            if(stack.top()->_id == OBJECT)
                            {
                                (*stack.top())[activeKey] = temp;
                            }   
                            else if(stack.top()->_id == LIST){
                                (*stack.top())._list.push_back(temp);
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
                else if((isStr = isString(tokens[i])) ||
                        (isNum = isNumeric(tokens[i])) ||
                        (isBool = isBoolean(tokens[i])) ||
                        (isNulled = isNull(tokens[i]))){
                            std::string temp = tokens[i];
                            if(isStr) temp = tokens[i].substr(1, tokens[i].length() - 2); /*Trimming the quotes*/
                            if(stack.top()->_id == OBJECT)
                            {
                                (*stack.top())[activeKey] = temp;
                            }   
                            else if(stack.top()->_id == LIST){

                                (*stack.top())._list.push_back(temp);
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
    auto x = Object::lexer(buffer);
    // for(auto i : x){
    //     std::cout << i << '\n';
    // }
    _handle.close();
    return parser(x);
}


void Object::toJSON(const std::string& file_name){

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
Object::~Object(){
    this->_map = nullptr;
}