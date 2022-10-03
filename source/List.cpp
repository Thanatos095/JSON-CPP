#include<List.hpp>

List::List(Type& list){
    if(list._id != TypeId::LIST) throw std::runtime_error("Object is not an iterable type.");
    this->_list = &list._list;
    this->_owns_memory = false;
}
List::List(Type&& source){
    if(source._id != TypeId::LIST) throw std::runtime_error("Object is not an iterable type.");
    this->_list = new std::vector<Type>(std::move(source._list));
    this->_owns_memory = true;
}
void List::push(const Type& item){
    this->_list->push_back(item);
}
void List::push(Type&& item){
    this->_list->push_back(std::move(item));
}
int List::contains(const Type& value){
    auto it = std::find(this->_list->begin(), this->_list->end(), value);
    return (it == this->_list->end() ? -1 : it - this->_list->begin());
}
void List::clear(){
    this->_list->clear();
}
Type List::map(Type(*call_back)(Type&, size_t)){
    Type _to_return = Type::List();
    for (size_t i = 0; i < this->_list->size(); i++)
    {
        _to_return._list.push_back(call_back((*this->_list)[i], i));
    }
    return _to_return;
}
void List::forEach(void(*call_back)(Type&, size_t)){
    for(size_t i = 0 ; i < this->_list->size() ; i++){
        call_back((*this->_list)[i], i);
    }
}
Type List::pop(){
    Type _to_return = (*this->_list)[this->_list->size() - 1];
    this->_list->pop_back();
    return _to_return;
}
size_t List::length(){
    return this->_list->size();
}
List::~List(){
    if(this->_owns_memory) delete this->_list;
    else this->_list = nullptr;
}
