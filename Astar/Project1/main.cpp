/*
    This program is a pathfinding demonstrator. Given a starting point, the user is able to click on any of the
    white blocks. The program then generates the shortest path to that point and showcases the path in green.
    The algorithm used is A*
*/



#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include "path_find.h"
using namespace std;


bool found_path = false;

void print_board(char board[20][20]);
int main()
{
    //loading all the block sprties
    sf::Texture begin_texture;
    if (!begin_texture.loadFromFile("begin.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite begin_sprite(begin_texture);
    
    sf::Texture blocked_texture;
    if (!blocked_texture.loadFromFile("blocked.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite blocked_sprite(blocked_texture);

    sf::Texture normal_texture;
    if (!normal_texture.loadFromFile("normal.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite normal_sprite(normal_texture);

    sf::Texture path_texture;
    if (!path_texture.loadFromFile("path.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite path_sprite(path_texture);

    sf::Texture end_texture;
    if (!end_texture.loadFromFile("end.png")) {
        return EXIT_FAILURE;
    }
    sf::Sprite end_sprite(end_texture);

    const int tileSize = end_texture.getSize().x;

    const int windowSize = 20 * tileSize;

    char board[20][20];

    //render a 20  by 20 window with the pixel size in mind
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Astar", sf::Style::Titlebar | sf::Style::Close);  
    int choseColInt = 0;
    int choseRowInt = 0;
    bool clicked = false;
    vector<pair<int, int>>path;
    while (window.isOpen())
    {
        clicked = false;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();

            }
            //if clicked, check if it is clicked on valid position. If yes, use that as the chosen row and col
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                int choseColInt_potential = localPosition.y / tileSize;
                int choseRowInt_potential = localPosition.x / tileSize;
                if (board[choseRowInt_potential][choseColInt_potential] != 'B'){
                        clicked = true;
                        std::cout << "clicked!" << std::endl;
                        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                        choseColInt = choseColInt_potential;
                        choseRowInt = choseRowInt_potential;

                }

            }
           
        }

        //draw all the blocks according to the type. This can be start,end,blocked,path,normal.

        for (int i = 0; i < 20 * tileSize; i = i + tileSize) {
            for (int j = 0; j < 20 * tileSize; j = j + tileSize) {

               if ((j / tileSize <= 10) && (i / tileSize == 5)){
                    window.draw(blocked_sprite);
                    blocked_sprite.setPosition(i, j);
                    board[i/ tileSize][j / tileSize] = 'B';
                }
                else if ((j / tileSize >= 10) && (j / tileSize <= 15) && (i/tileSize == j/tileSize)) {
                    window.draw(blocked_sprite);
                    blocked_sprite.setPosition(i, j);
                    board[i / tileSize][j / tileSize] = 'B';

                }
                else if ((j/tileSize==17)&&((i/tileSize)>=7 &&(i/tileSize<=13))) {
                    window.draw(blocked_sprite);
                    blocked_sprite.setPosition(i,j);
                    board[i / tileSize][j / tileSize] = 'B';

                }
                
                else if ((i / tileSize == 2) && (j / tileSize == 2)) {
                   window.draw(begin_sprite);
                   begin_sprite.setPosition(i, j);
                   board[i / tileSize][j / tileSize] = 'S';

               }
                else {
                    window.draw(normal_sprite);
                    normal_sprite.setPosition(i, j);
                    board[i / tileSize][j / tileSize] = 'N';
                }
               //if a valid block is clicked find path
               if (clicked) {
                   window.draw(end_sprite);
                   end_sprite.setPosition(choseRowInt*tileSize, choseColInt*tileSize);
                   board[choseRowInt][choseColInt] = 'E';

                  path = find_path(2, 2, choseColInt, choseRowInt, board);
               }
               //if there is an existing path, draw it (this is to prevent recalculating path over and over if path doesnt change)
               if (path.size() > 0) {
                   for (int i = 1; i < path.size() - 1; i++) {
                       window.draw(path_sprite);
                       path_sprite.setPosition(path[i].second * tileSize, path[i].first * tileSize);

                   }
                   window.draw(end_sprite);
                   end_sprite.setPosition(choseRowInt * tileSize, choseColInt * tileSize);
                   board[choseRowInt][choseColInt] = 'E';


               }

            }


        }
        window.display();
    }

    return 0;
}


//just used for debugging
void print_board(char board[20][20]) {

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 20; x++) {
            std::cout << board[x][y];
        }
        std::cout << std::endl;
    }
}
