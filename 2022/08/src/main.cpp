#include <iostream>
#include <fstream> 
#include <string>

using namespace std;

class Forest{
    unsigned int gridHeight, gridWidth, gridSize;
    int *trees;

public:
    Forest(ifstream &ifs) {
        string str_buffer;
        
        while (ifs.get() != '\n') gridWidth++ ;
        gridHeight = 1;
        while (getline(ifs, str_buffer)) gridHeight++;
        trees =  new int[gridHeight*gridWidth*sizeof(int)];

        ifs.clear();
        ifs.seekg(0);

        for (int i = 0 ; i < gridHeight*gridWidth ; ++i) {
            trees[i] = (int) ifs.get() - (int) '0' ;
            if (ifs.peek() == '\n')
                ifs.ignore(1);
        }

    }

    ~Forest () {delete(trees);}
    
    friend ostream& operator<< (ostream &os, Forest &forest);

    unsigned int getHeight() {return gridHeight;}
    unsigned int getWidth() {return gridWidth;}

    int& getTree (int row, int col) {
        return trees[row * gridWidth + col];
    }

    bool isVisible(int row, int col) {
        bool isVisibleFromDir = true;
    
        //Forest border
        if ((row == 0) || (row == gridHeight-1) || (col == 0) || (col == gridWidth-1)){       
            return true;
        }
    
        //Look left
        for (int i = col-1 ; i >= 0 ; --i){
            if (getTree(row, i) >= getTree(row, col)){
                isVisibleFromDir = false;               
                break;
            }
        }
        if (isVisibleFromDir)
            return true;

        //Look right
        isVisibleFromDir = true;
        for (int i = col+1 ; i < gridWidth ; ++i){
            if (getTree(row, i) >= getTree(row, col)){
                isVisibleFromDir = false;               
                break;
            }
        }
        if (isVisibleFromDir)
            return true;

        //Look up
        isVisibleFromDir = true;
        for (int i = row-1 ; i >= 0 ; --i){
            if (getTree(i, col) >= getTree(row, col)){
                isVisibleFromDir = false;               
                break;
            }
        }
        if (isVisibleFromDir)
            return true;

        //Look down
        isVisibleFromDir = true;
        for (int i = row+1 ; i < gridHeight ; ++i){
            if (getTree(i, col) >= getTree(row, col)){
                isVisibleFromDir = false;               
                break;
            }
        }
        if (isVisibleFromDir)
            return true;

        return false;
    }

    int getScoreVision(int row, int col) {

        int lScore=0, rScore=0, uScore=0, dScore=0;

        //Forest border
        if ((row == 0) || (row == gridHeight-1) || (col == 0) || (col == gridWidth-1)){       
            return 0;
        }       

        //Look left
        for (int i = col-1 ; i >= 0 ; --i){
            lScore++;
            if (getTree(row, i) >= getTree(row, col)){               
                break;
            }
        }

        //Look right
        for (int i = col+1 ; i < gridWidth ; ++i){
            rScore++;
            if (getTree(row, i) >= getTree(row, col)){            
                break;
            }
        }

        //Look up
        for (int i = row-1 ; i >= 0 ; --i){
            uScore++;
            if (getTree(i, col) >= getTree(row, col)){             
                break;
            }
        }

        //Look down
        for (int i = row+1 ; i < gridHeight ; ++i){
            dScore++;
            if (getTree(i, col) >= getTree(row, col)){               
                break;
            }
        }
        
        return lScore * rScore * uScore * dScore;
    }
    
};


ostream& operator<< (ostream &os, Forest &forest) {
    int oneDimIdx = 0;

    os << endl;
    for (int i_row = 0 ; i_row < forest.gridHeight ; ++i_row) {
        for (int i_col = 0 ; i_col < forest.gridWidth ; ++i_col) {
            os << forest.trees[oneDimIdx];
            oneDimIdx++;
        }
        os << endl;
    }
    os << endl;

    return os;
}

int main (int argc, char **argv) {

    ifstream ifs_input ("input_08.txt");
    Forest forest(ifs_input);
    cout << forest << endl;
    
    int visibleTreesCount, maxVisionScore, visionScore;
    for (int i_row = 0 ; i_row < forest.getHeight() ; ++i_row) {
        for (int i_col = 0 ; i_col < forest.getWidth() ; ++i_col) {
            if (forest.isVisible(i_row, i_col))
                visibleTreesCount++;

            visionScore = forest.getScoreVision(i_row, i_col) ;
            if ( visionScore > maxVisionScore)
                maxVisionScore = visionScore;
        }
    }
  
    cout << "Day 8 - Part1  anwswer : " << visibleTreesCount << endl;
    cout << "Day 8 - Part2  anwswer : " << maxVisionScore << endl;

   
    return 0;
}
