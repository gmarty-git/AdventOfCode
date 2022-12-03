#include "utils_fileIO.hpp"

std::string readTxtFile2String(std::string str_file_path){
    std::ifstream ifs(str_file_path);
    std::string str_fileContent( (std::istreambuf_iterator<char>(ifs) ),
                                  (std::istreambuf_iterator<char>()    ) );

    return str_fileContent;
}
