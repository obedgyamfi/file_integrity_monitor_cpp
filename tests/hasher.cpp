#include <iostream>
#include <string>
#include <functional> // required for std::hash 

// size_t hasher(const std::string &text);

int main()
{
    std::string s = "hello";
    std::hash<std::string> hasher;
    size_t hash_value = hasher(s);
    std::cout << "Hash of 'hello': " << hash_value << std::endl;
    return 0;
}

// size_t hasher(const std::string &text)
// {

// }