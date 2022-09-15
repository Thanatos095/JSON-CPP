#include<iostream>
#include<Type.hpp>
#include<List.hpp>
#include <unordered_map>


int main(){
        Type data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        Type squared = List(data).map([](Type& item, size_t index){ return item * 2; });
        std::cout << squared << '\n';

    //     Type obj = {
    //         field(name, "Mahad Hameed"),
    //         field(age, 19),
    //         field(dob, "16-OCT-2002"),
    //         field(address, {
    //                         field(city, "Karachi"),
    //                         field(country, "Pakistan"),
    //                         field(town, "Gulshan-e-Hadeed")
    //                     }),
    //         field(friends, {"Abdullah", "Siraj", "Abbas", "Raahim"})
    // };
    return 0;
}