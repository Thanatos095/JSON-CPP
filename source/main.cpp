#include<iostream>
#include<Type.hpp>
#include<List.hpp>
#include<Object.hpp>
#include <unordered_map>
#include <fstream>
using namespace std;
// class Graph{ /* indirected */
//     public:
//         Graph() : _vertices(Type::Object()), _edges(Type::Object()){
//         }
//         void add_vertex(const std::string& vertex_name){
//             assert(!Object(_vertices).containsKey(vertex_name));
//             _vertices[vertex_name] = Type::List();
//         }
//         void add_edge(const std::string& source, const std::string& target, Type _data = Type::Null()){
//             /*Make sure both vertices exist*/

//             assert(Object(_vertices).containsKey(source));
//             assert(Object(_vertices).containsKey(target));

//             List(_vertices[source]).push(target);
//             // if(source != target)  put condition here
//             List(_vertices[target]).push(source);
//             _edges[makeKey(source, target)] = _data;
//         }
//         void printAdjacencyList(){
//             std::cout << this->_vertices << '\n';
//         }
//         size_t degree(const std::string& vertex_name){
//             assert(Object(_vertices).containsKey(vertex_name));
//             return List(_vertices[vertex_name]).length();
//         }
//         // void bfs(void (*call_back)(const std::string&)){

//         // }

//     private:
//         static std::string makeKey(const std::string& first, const std::string& second){
//             return first > second ? first + second : second + first;
//         }
//         Type _vertices;
//         Type _edges;
// };

int main(int argc, char *argv[]){
    // Type user_info = {
    //         field(name, "Mahad Hameed"),
    //         field(age, 19),
    //         field(dob, "16-OCT-2002"),
    //         field(address, {
    //                         field(city, "Karachi"),
    //                         field(country, "Pakistan"),
    //                         field(town, "Gulshan-e-Hadeed"),
    //                         field(code, 75010)
    //                     }),
    //         field(friends, {"Abdullah", "Siraj", "Abbas", "Raahim"})
    //     };
    // Object::ToJSON(user_info, "user_info.json");
    // Type data = Object::FromJSON("user_info.json");
    // std::cout << data << '\n';

    Type data = {1, 2, 3, "Mahad"};

    std::cout << List(data).contains("Mahad") << '\n';
    std::cout << List(data).contains(1) << '\n';
    std::cout << List(data).contains("1") << '\n';
    // std::cout << data << '\n';
    // Graph g;
    // g.add_vertex("A");
    // g.add_vertex("B");
    // g.add_vertex("C");
    // g.add_vertex("D");

    // g.add_edge("A", "B");
    // g.add_edge("A", "C");
    // g.add_edge("A", "A");
    // std::cout << g.degree("A") << '\n';
    // std::cout << g.degree("B")<< '\n';
    // std::cout << g.degree("C")<< '\n';
    // std::cout << g.degree("D")<< '\n';



    return 0;
}