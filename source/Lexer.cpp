#include <Object.hpp>

Object::Lexer::Lexer(std::ifstream& handle) : _handle(handle), _symbols{'{', '}', '[', ']', ',', ':'}{
}
const std::vector<std::pair<std::string, int>>& Object::Lexer::get(){
    this->_tokens.clear();
    if(!_handle.eof()){
        std::getline(_handle, this->_buffer);
        for(size_t i = 0 ; i < this->_buffer.length() ; i++){
            if(std::isspace(this->_buffer[i])) continue;
            else if(this->contains(this->_symbols, this->_buffer[i]) != -1){
                this->_tokens.push_back(std::pair(std::string(1, this->_buffer[i]), 0));
            }
            else if(this->_buffer[i] == '\"'){
                std::string token = "\"";
                i++;
                while(this->_buffer[i] != '\"'){
                    
                    if(this->_buffer[i] == '\\'){ /* Handling escape sequences */
                        switch(this->_buffer[i + 1]){
                            case '\"' : break;
                            case '\\' : break;
                            case '/'  : break;
                            case 'b'  : break;
                            case 'f'  : break;
                            case 'n'  : break;
                            case 'r'  : break;
                            case 't'  : break;
                            default : throw std::runtime_error("Token:" + std::string("\'" + token + "\'") + "not recognized");
                        }
                        token += this->_buffer.substr(i, 2);
                        i += 2;
                    }               
                    else token += this->_buffer[i++];
                }
                token += '\"';
                trim(token);
                this->_tokens.push_back(std::pair(token, int(TypeId::TEXT)));
            }
            else{
                std::string token; int identifier;
                while(contains(_symbols, this->_buffer[i]) == -1) token += this->_buffer[i++];
                i--;
                trim(token);
                if(token == "null")                          identifier = int(TypeId::NULL_VALUE);
                else if(token == "false" || token == "true") identifier = int(TypeId::BOOLEAN);
                else if (this->isNumeric(token))                identifier = int(TypeId::NUMBER);
                else throw std::runtime_error("Token:" + std::string("\'" + token + "\'") + "not recognized");
                
                this->_tokens.push_back(std::pair(token, identifier));
            }
        }
    }
    return this->_tokens;
}
bool Object::Lexer::isNumeric(const std::string& token){
    std::regex number_literal("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
    return std::regex_match(token, number_literal);
}
int Object::Lexer::contains(const std::vector<char>& tokens, char value){
        auto it = std::find(tokens.begin(), tokens.end(), value);
        return it == tokens.end() ? -1 : it - tokens.begin();
}
void Object::Lexer::trim(std::string& str){
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}