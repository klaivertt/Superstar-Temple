#include "Tools/Parser/Parser.hpp"

bool PRSR::ContentWord(std::string _str, std::string _word)
{
    return (_str.find(_word) != std::string::npos ? true: false);
}
