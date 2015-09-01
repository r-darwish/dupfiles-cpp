#include <iostream>
#include <dupfiles.hpp>

int main()
{
    std::cout << "" << std::endl;
    for (auto duplicate_group : dupfiles::findDuplicates(".")) {
        std::cout << duplicate_group.size() << " Duplicate files: " << std::endl;
        for (auto duplicate_entry : duplicate_group) {
            std::cout << "    " << duplicate_entry << std::endl;
        }
    }
}
