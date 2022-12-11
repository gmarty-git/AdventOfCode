#include "class_ElvesFilesytem.hpp"

//ElvesFile
//#################################"
ElvesFile::ElvesFile (unsigned int filesize, std::string filename){
    this->filename = filename;
    this->filesize = filesize;
}

ElvesFile::~ElvesFile () {};

unsigned int ElvesFile::getFilesize (){
    return filesize;
}

std::string ElvesFile::getFilename (){
    return filename;
}

std::ostream& ElvesFile::disp (std::ostream &os, unsigned int treeFolderDepth){
    os << std::string(2*treeFolderDepth, ' ') << "- " << filename << " (file, size=" << filesize << ")" ;
    return os;
}

bool ElvesFile::operator== (std::string filename) {
    return this->filename == filename;
}

bool ElvesFile::operator< (ElvesFile elvesFile) {
    return this->filename < elvesFile.getFilename();
}

std::ostream& operator << (std::ostream &os, ElvesFile &elvesFile){
    return elvesFile.disp(os, 0);
}


//ElvesFolder
//#################################"
ElvesFolder::ElvesFolder (std::string dirname, ElvesFolder *parentFolder) : ElvesFile::ElvesFile(0, dirname) {
    this->parentFolder = parentFolder;
} ;

ElvesFolder::~ElvesFolder () {};

int ElvesFolder::increaseFolderSize (unsigned int filesize, ElvesFolder *folder){
    folder->filesize += filesize;

    if ((*folder).parentFolder != nullptr)
             increaseFolderSize(filesize, (*folder).parentFolder);

    return 0;
};

ElvesFolder* ElvesFolder::newFile (unsigned int filesize, std::string filename){
    ElvesFile elvesfile(filesize, filename);
    files.push_back(elvesfile);
    increaseFolderSize(filesize, this);
    return this;
}

ElvesFolder* ElvesFolder::newDir (std::string dirname){
    ElvesFolder elvevesfolder(dirname, this);
    folders.push_back(elvevesfolder);
}

ElvesFolder* ElvesFolder::moveDir (std::string  filename) {
    if (filename == ".")
        return this ;
    
    else if (filename == "..")
        if (parentFolder != nullptr)
            return parentFolder ;
        else{
            std::cout << "[ERROR] - moveFolder : already at root level : '" << this->filename << "' !" <<  "Abort execution." << std::endl;
            exit(-1);
        }
    
    else {
        std::vector<ElvesFolder>::iterator it = std::find(folders.begin(), folders.end(), filename);
        if (it != folders.end()) {
            return &(*it) ;
        }
        else {
            std::cout << "[ERROR] - moveFolder : folder not found !" << std::endl;
            exit(-1);
        }
    }
}

std::ostream&  ElvesFolder::disp (std::ostream &os, unsigned int treeFolderDepth){
    os << std::string(2*treeFolderDepth, ' ') << "- " << filename << " (dir, size=" << filesize << ")" << std::endl ;
    for (auto it = folders.begin() ; it != folders.end() ; ++it){
        it->disp(os, treeFolderDepth+1);
    }
    for (auto it = files.begin() ; it != files.end() ; ++it){
        it->disp(os, treeFolderDepth+1);
        os << std::endl ;
    }

    os << std::endl;

    return os;
}

std::ostream& operator << (std::ostream &os, ElvesFolder &elvesFolder){
    return elvesFolder.disp(os, 0);
}

unsigned int ElvesFolder::getSmallFoldersSize (unsigned int smallFolderThreshold, unsigned int currentSmallFolderSize){   
    if (filesize <= smallFolderThreshold)
        currentSmallFolderSize += filesize;

    for (auto it = folders.begin() ; it != folders.end() ; ++it)
        currentSmallFolderSize = (*it).getSmallFoldersSize(smallFolderThreshold, currentSmallFolderSize);        

    return currentSmallFolderSize;
}

ElvesFolder* ElvesFolder::getSmallestFolder2FreeMemory (unsigned int minimumSize, ElvesFolder* currentFolderCandidate){
    if (currentFolderCandidate == nullptr)
        currentFolderCandidate = this;

    else if ( (filesize >= minimumSize) && (filesize < currentFolderCandidate->filesize) )
        currentFolderCandidate = this;

    for (auto it = folders.begin() ; it != folders.end() ; ++it)
        currentFolderCandidate = (*it).getSmallestFolder2FreeMemory(minimumSize, currentFolderCandidate);        

    return currentFolderCandidate;
}
//#####################
