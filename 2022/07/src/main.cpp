#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "class_ElvesFilesytem.hpp"

using namespace std;

int main (int argc, char **argv) {

    ifstream ifs_input ("input_07.txt");
    stringstream ss;    
    string str_line, str_word;    
    unsigned int filesize;

    getline(ifs_input, str_line);
    ElvesFolder rootFolder("/");
    ElvesFolder *currentDir = &rootFolder;
    
    //ElvesInterpreter
    //----------------------------------------------------------------------------------------
    while (getline(ifs_input, str_line)) {
        ss.str(str_line);
        ss.clear();
        ss >> str_word;

        if (str_word == "$") {
            ss >> str_word ;

            if (str_word == "cd") {
                ss >> str_word;
                currentDir = (*currentDir).moveDir(str_word);
            }
            else if (str_word == "ls") {
                while ( (ifs_input.peek()!=EOF) && (ifs_input.peek()!='$') ){
                    getline(ifs_input, str_line);
                    ss.str(str_line);
                    ss.clear();
                    ss >> str_word;

                    if (str_word == "dir") {
                        ss >> str_word;
                        *currentDir->newDir(str_word);                        
                    }
                    else {
                        filesize = stoi(str_word);
                        ss >> str_word;
                        currentDir -> newFile(filesize, str_word);      
                    }
                }            
            }
        }
    }
    //----------------------------------------------------------------------------------------

    cout << rootFolder ;
    cout << "Day 7 - Part1  anwswer : " <<  rootFolder.getSmallFoldersSize(100000) << endl;
    currentDir = rootFolder.getSmallestFolder2FreeMemory(30000000 + rootFolder.getFilesize() - 70000000);
    cout << "Day 7 - Part2  anwswer : " << currentDir->getFilesize()   << " (Folder '" << currentDir->getFilename() << "')" << endl;


    return 0;
}

