#ifndef CLASS_ELVES_FILESYSTEM_YOM
#define CLASS_ELVES_FILESYSTEM_YOM

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class ElvesFile
{protected:
	std::string filename;
    unsigned int filesize;
   

 public:
    ElvesFile (unsigned int filesize, std::string filename);
    ~ElvesFile ();
    unsigned int getFilesize ();
    std::string getFilename ();
    std::ostream& disp (std::ostream &os, unsigned int treeFolderDepth);
    friend std::ostream &operator << (std::ostream &os, ElvesFile &elvesFile);
    bool operator== (std::string filename);
    bool operator< (ElvesFile elvesFile);

};

class ElvesFolder : public ElvesFile
{private:
    std::vector<ElvesFolder> folders;
    std::vector<ElvesFile> files;
    ElvesFolder *parentFolder;
    int increaseFolderSize(unsigned int filesize, ElvesFolder *parentFolder);
      

 public:
    ElvesFolder (std::string dirname, ElvesFolder *folder = nullptr);
    ~ElvesFolder ();
    ElvesFolder* newFile (unsigned int filesize, std::string filename);
    ElvesFolder* newDir (std::string dirname);
    ElvesFolder* moveDir (std::string filename);
    std::ostream& disp (std::ostream &os, unsigned int treeFolderDepth);
    friend std::ostream &operator << (std::ostream &os, ElvesFolder &elvesFolder);
    unsigned int getSmallFoldersSize (unsigned int smallFolderThreshold, unsigned int currentSmallFolderSize=0);
    ElvesFolder* getSmallestFolder2FreeMemory( unsigned int freeMemoryTarget, ElvesFolder* currentFolderCandidate = nullptr);
};

bool operator< (ElvesFile file, ElvesFolder folder);
bool operator< (ElvesFolder folder, ElvesFile file);

#endif
