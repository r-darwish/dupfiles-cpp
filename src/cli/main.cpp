#include <fstream>
#include <iostream>
#ifdef _MSC_VER
#include <iso646.h>
#endif
#include <dupfiles.hpp>

int main()
{
    auto errors = std::vector<std::string>();
    auto duplicates = dupfiles::find_duplicates(".", [&](std::string what) { errors.emplace_back(what); });

    std::ofstream report;
    report.open("Duplicate Files.txt", std::ios::trunc);
    if (not report.is_open()) {
        std::cerr << "Failed to open the report file" << std::endl;
        return 1;
    }

    if (not errors.empty()) {
        report << "Errors: " << std::endl;
        for (const auto& error : errors) {
            report << error << std::endl;
        }
        report << std::endl;
    }

    for (const auto& duplicate_group : duplicates) {
        report << duplicate_group.size() << " Duplicate files: " << std::endl;
        for (const auto& duplicate_entry : duplicate_group) {
            report << "    " << duplicate_entry << std::endl;
        }
    }
}
