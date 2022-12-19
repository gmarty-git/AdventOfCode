#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Slice {
    public:
        Slice(unsigned int width = 1000, unsigned int height = 200) {
            abyssHeight = 0;
            view_Xmin = 300;
            view_Xmax = 700;
            view_Ymin = 0;
            view_Ymax = 0;
            for (int i = 0 ; i <  height; ++i)
                grid.emplace_back(width, '.');
        }

        char& operator() (unsigned int x, unsigned int y) {
            return grid.at(y).at(x);
        }

        void readsRocksFromFile(string fileName) {
            ifstream ifs(fileName);
            stringstream ss;
            string str_line, str_buffer;
            unsigned int x_prev, y_prev, x, y, x_increment, y_increment;
       
            while (getline(ifs, str_line)){
                ss.str(str_line);
                ss.clear();
                ss >> x_prev;
                ss.ignore(1, ',');
                ss >> y_prev;
                if (y_prev > abyssHeight)
                    abyssHeight = y_prev;
                (*this)(x_prev,y_prev) = '#';

                view_Xmin = x_prev < view_Xmin ? x_prev : view_Xmin ;
                view_Xmax = x_prev > view_Xmax ? x_prev : view_Xmax ;
                view_Ymax = y_prev > view_Ymax ? y_prev : view_Ymax ;

                while (ss >> str_buffer) {
                    ss >> x;
                    ss.ignore(1, ',');
                    ss >> y;
                    if (y > abyssHeight)
                        abyssHeight = y;
                    x_increment = x > x_prev ? 1 : -1 ;
                    y_increment = y > y_prev ? 1 : -1 ;
                    for (int i_x = x_prev ; i_x != x + x_increment ; i_x += x_increment) {
                        for (int i_y = y_prev ; i_y != y + y_increment ; i_y += y_increment) {
                            (*this)(i_x, i_y) = '#';

                            view_Xmin = i_x < view_Xmin ? i_x : view_Xmin ;
                            view_Xmax = i_x > view_Xmax ? i_x : view_Xmax ;
                            view_Ymax = i_y > view_Ymax ? i_y : view_Ymax ;
                        }
                    }
                        x_prev = x;
                        y_prev = y;
                }
            }
        }

        bool generateSand(unsigned int unitsCount = 1) {
            bool sandReachedAbyss = false, obstructesSource = true, moved;
            unsigned int x, y;            

            for (int i = 0 ; i < unitsCount ; ++i){
                x=500, y=0;
                
                do {
                    moved = false;
                    if ((*this)(x, y+1) == '.'){
                        y++;
                        moved = true;
                    }
                    else if ((*this)(x-1, y+1) == '.'){
                        y++;
                        x--;
                        moved = true;
                    }
                    else if ((*this)(x+1, y+1) == '.'){
                        y++;
                        x++;
                        moved = true;
                    }
                    if (y >= abyssHeight) {
                        moved = false;
                        sandReachedAbyss = true;
                    }

                    if (moved)
                        obstructesSource = false;
                } while (moved);
            
                if (y < abyssHeight)
                    (*this)(x, y) = 'o';
             }  

            return !sandReachedAbyss && !obstructesSource;
        }
        
        ostream& print(ostream &os) {
            for (int i_y = view_Ymin ; i_y <= view_Ymax ; ++i_y) {
               for (int i_x = view_Xmin ; i_x <= view_Xmax ; ++i_x)
                  os << (*this)(i_x, i_y) ;

                os << endl;
                }               
            
            return os;
        }     

        void setFloor(unsigned int floorHeight = 0) {
            if (floorHeight == 0)
                floorHeight = abyssHeight + 2;
            for (auto it = grid.at(floorHeight).begin() ; it != grid.at(floorHeight).end() ; ++it)
                *it = '#';

            abyssHeight =  floorHeight          ;

            if (floorHeight > view_Ymax)
                view_Ymax =  floorHeight;
        }

    public:
       std::vector<vector<char>> grid;
       unsigned int abyssHeight, view_Xmin, view_Xmax, view_Ymin, view_Ymax;
};


class Application
{
public:
    Application(){
        m_window = SDL_CreateWindow("I love sand !!!",
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    1200, 800,
                                    0);

        if(!m_window)
        {
            std::cout << "Failed to create window\n";
            std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
            return;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        caveSlice.readsRocksFromFile("input_14.txt");
        caveSlice.setFloor();

    }

    ~Application(){
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
    }

    void loop(){
        bool keep_window_open = true;
        while(keep_window_open)
        {
            while(SDL_PollEvent(&m_window_event) > 0)
            {
                switch(m_window_event.type)
                {
                    case SDL_QUIT:
                        keep_window_open = false;
                        break;
                }
            }

            update();
            draw();
        }
    }

    void update(){
        caveSlice.generateSand(25);
        //SDL_Delay(0);
    }

    void draw()
    {
       int screenW, screenH;
       SDL_GetWindowSize(m_window, &screenW, &screenH);
  
       SDL_Rect rect;        
       rect.w = screenW / (caveSlice.view_Xmax - caveSlice.view_Xmin);
       rect.h = screenH / (caveSlice.view_Ymax - caveSlice.view_Ymin);
       SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
       SDL_RenderClear(m_renderer);
       
       for (int i_y = caveSlice.view_Ymin ; i_y <= caveSlice.view_Ymax ; ++i_y) {
            for (int i_x = caveSlice.view_Xmin ; i_x <= caveSlice.view_Xmax ; ++i_x){

                rect.x = (i_x - caveSlice.view_Xmin) * rect.w;
                rect.y = (i_y - caveSlice.view_Ymin) * rect.h;
                
                if (caveSlice(i_x, i_y) == '#') {
                    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
                }
                else if (caveSlice(i_x, i_y) == 'o') {
                    SDL_SetRenderDrawColor(m_renderer, 140, 100, 65, 255);
                }
                else {
                    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
                }

                SDL_RenderFillRect(m_renderer, &rect);
            }
        }
        
        SDL_RenderPresent(m_renderer);
        SDL_UpdateWindowSurface(m_window);
    }

private:
    SDL_Window  *m_window;
    SDL_Event    m_window_event;
    SDL_Renderer *m_renderer;
    Slice caveSlice;
};


int main()
{
    Application app;
    app.loop();
}
