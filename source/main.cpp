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
    // Object::ToJSON(user_info, "user_info.json");
    Type data = Object::FromJSON("data.json");
    std::cout << data[0] << '\n';
    return 0;
}