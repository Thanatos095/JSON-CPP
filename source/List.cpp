#include<List.hpp>

List::List(Type& list){
    if(list._id != LIST) throw std::runtime_error("Object is not an iterable type.");
    this->_list = &list._list;
}
void List::push(const Type& item){
    this->_list->push_back(item);
}
void List::push(Type&& item){
    this->_list->push_back(std::move(item));
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
Type List::pop(){
    Type _to_return = (*this->_list)[this->_list->size() - 1];
    this->_list->pop_back();
    return _to_return;
}
List::~List(){
    this->_list = nullptr;
}
