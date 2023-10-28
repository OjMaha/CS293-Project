#include <string>
#include <sstream>

// Function to strip both left and right spaces from a string
std::string strip(const std::string& input, const std::string& chars = " \t\n\r") {
    size_t start = input.find_first_not_of(chars);
    if (start == std::string::npos) {
        return ""; // Entire string is composed of specified characters
    }

    size_t end = input.find_last_not_of(chars);
    if (end == std::string::npos) {
        return ""; // Entire string is composed of specified characters
    }

    return input.substr(start, end - start + 1);
}