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
        char c = data[i];
        if(isspace(data[i])) continue;
        else if(contains(symbols, data[i]) != -1){
            tokens.push_back(std::string(1, data[i]));
            std::cout << tokens[tokens.size() - 1] << '\n';
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
            std::cout << tokens[tokens.size() - 1] << '\n';
        }
        else{
            std::string token;
            while(contains(symbols, data[i]) == -1) token += data[i++];
            i--;
            tokens.push_back(token);
            std::cout << tokens[tokens.size() - 1] << '\n';
        }
    }
    return tokens;
}
enum class States{
    OBJECT,
    LIST
};
Type Object::parser(const std::vector<std::string>& tokens){
    Type JSON = Type::Object();
    std::string state = "object";
    assert(tokens[0] == "{" && tokens[tokens.size() - 1] == "}");


    // std::vector<std::string> opening_symbols = {"{", "[", ":"};    
    // std::vector<std::string> closing_symbols = {"}", "]"};
    // std::stack<std::string> brackets;
    // std::stack<States> state;
    // std::regex string_literal(R"("|')(?:\\\1|[^\1])*?\1)");
    // std::regex string_literal(R"(\"\w*\")");
    // std::regex number_literal("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)"); /*Numbers */
    // for(size_t i = 0 ; i < tokens.size() ; i++){
    //     const std::string& token = tokens[i];
    //     int index;
    //     if((index = contains(opening_symbols, token)) != -1){
    //         if(tokens[i] == ":" || tokens[i] = "{") state.push(States::OBJECT);
    //         brackets.push(tokens[i]);
    //     }
    //     else if((index = contains(closing_symbols, token)) != -1){
    //         if(brackets.empty() || brackets.top() != opening_symbols[index]) throw std::runtime_error("Missing opening bracket for "  + tokens[i]);
    //         brackets.pop();
    //     }
    //     else if(std::regex_match(tokens[i], string_literal)){
    //         if(state.top() == States::OBJECT){

    //         }
    //         std::cout << tokens[i] << '\n';
    //     }
    //     else if(std::regex_match(tokens[i], number_literal)){
    //         std::cout << tokens[i] << '\n';
    //     }
    //     else if(tokens[i] == ":"){
    //         // state = ':';
    //     }
    //     else{
    //         throw std::runtime_error("Token : " + token[i] + std::string("not recognized"));
    //     }
    // }
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