#include "utils_string.hpp"

std::vector<std::string> str_split(const std::string str, char char_sep){
    std::vector<std::string> strVec_split;
    std::stringstream ss(str);
    std::string str_tmp;

    while(getline(ss, str_tmp, char_sep)){
      strVec_split.push_back(str_tmp);
   }

    return strVec_split;

}
