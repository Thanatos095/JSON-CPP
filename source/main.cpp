#include<iostream>
#include<Type.hpp>
#include<List.hpp>
#include<Object.hpp>
#include <unordered_map>
#include <fstream>



int main(){
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

    // Type user_info = {1, 2, "Mahad Hameed", {1, 2, 3, 4, 5, {3, 4, 5, 6}}};  
    // std::cout << user_info << '\n';
    // std::cout << user_info["address"] << '\n';    
    std::cout << Object::FromJSON("lists.json");
    return 0;
}