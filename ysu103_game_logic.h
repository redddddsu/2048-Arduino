
//DEMO 1 https://www.youtube.com/watch?v=2Reih4ck2CU
//DEMO 2 https://youtu.be/osiF3AG_7-I
#ifndef GAME_H
#define GAME_H

#include "ysu103_ST7735.h"
#include <stdlib.h>
#include <time.h>
#include "serialATmega-4.h"

unsigned int seed = 0;
unsigned int score = 0;
bool hasMove = false;

void randTileStartGame() {
    short x;
    short y;
    short x2;
    short y2; 

    srand(seed);

    x = rand() % 4;
    y = rand() % 4;
    
    x2 = rand() % 4;
    y2 = rand() % 4;

    while ((x == x2) | (y == y2)) {
        x = rand() % 4;
        y = rand() % 4;
    }

    while ((x == x2) | (y == y2)) {
        x2 = rand() % 4;
        y2 = rand() % 4;
    }

    drawTile(x, y, two);
    drawTile(x2, y2, two);

}
void spawnRandTile() {
    short x;
    short y;
    short two_or_four;

    srand(seed);

    x = rand() % 4;
    y = rand() % 4;
    two_or_four = rand() % 4;

    while (grid[y][x].whatTile.tileNum != 0) {
        x = rand() % 4;
        y = rand() % 4;
    }

    if (two_or_four > 0) {
        drawTile(x, y, two);
    }
    else {
        drawTile(x, y, four);
    }

}

tile computeTile(tile input) {
    unsigned int combined = input.tileNum * 2;
    score += input.tileNum;
    tile result;
    if (combined == 4) {
        result = four;
    }
    else if (combined == 8) {
        result = eight;
    }
    else if (combined == 16) {
        result = sixteen;
    }    
    else if (combined == 32) {
        result = thirtyTwo;
    }    
    else if (combined == 64) {
        result = sixtyFour;
    }    
    else if (combined == 128) {
        result = TwentyEight;
    }    
    else if (combined == 256) {
        result = FiftySix;
    }    
    else if (combined == 512) {
        result = Twelve;
    }    
    else if (combined == 1024) {
        result = TwentyFour;
    }    
    else if (combined == 2048) {
        result = FourtyEight;
    }    
    
    return result;
}

void mergeRight() {
    for (unsigned char y = 0; y < 4; y++) {
        if (grid[y][2].whatTile.tileNum != 0) {
            if (grid[y][2].whatTile.tileNum == grid[y][3].whatTile.tileNum) {
                drawTile(3, y, computeTile(grid[y][2].whatTile));
                drawTile(2, y, zero);
                hasMove = true;
            }
        }
        if (grid[y][1].whatTile.tileNum != 0) {
            if (grid[y][2].whatTile.tileNum == grid[y][1].whatTile.tileNum) {
                drawTile(2, y, computeTile(grid[y][1].whatTile));
                drawTile(1, y, zero);
                hasMove = true;
            }
            else if (grid[y][3].whatTile.tileNum == grid[y][1].whatTile.tileNum && grid[y][2].whatTile.tileNum == 0) {
                drawTile(3, y, computeTile(grid[y][1].whatTile));
                drawTile(1, y, zero);
                hasMove = true;
            }
        }
        if (grid[y][0].whatTile.tileNum != 0) {
            if (grid[y][1].whatTile.tileNum == grid[y][0].whatTile.tileNum) {
                drawTile(1, y, computeTile(grid[y][0].whatTile));
                drawTile(0, y, zero);
                hasMove = true;
            }
            else if (grid[y][2].whatTile.tileNum == grid[y][0].whatTile.tileNum && grid[y][1].whatTile.tileNum == 0) {
                drawTile(2, y, computeTile(grid[y][0].whatTile));
                drawTile(0, y, zero);
                hasMove = true;
            }
            else if (grid[y][3].whatTile.tileNum == grid[y][0].whatTile.tileNum && grid[y][1].whatTile.tileNum == 0 && grid[y][2].whatTile.tileNum == 0) {
                drawTile(3, y, computeTile(grid[y][0].whatTile));
                drawTile(0, y, zero);
                hasMove = true;
            }
        }
    }
}

void moveRight() {
    mergeRight();
    for (unsigned char y = 0; y < 4; y++) {
        if (grid[y][2].whatTile.tileNum != 0) {
            if (grid[y][3].whatTile.tileNum == 0) {
                drawTile(3, y, grid[y][2].whatTile);
                drawTile(2, y, zero);
                hasMove = true;
            }
        }
        if (grid[y][1].whatTile.tileNum != 0) {
            if (grid[y][2].whatTile.tileNum == 0) {
                drawTile(2, y, grid[y][1].whatTile);
                drawTile(1, y, zero);
                hasMove = true;

            }
            if (grid[y][3].whatTile.tileNum == 0) {
                drawTile(3, y, grid[y][2].whatTile);
                drawTile(2, y, zero);
                hasMove = true;
            }
        }
        if (grid[y][0].whatTile.tileNum != 0) {
            if (grid[y][1].whatTile.tileNum == 0) {
                drawTile(1, y, grid[y][0].whatTile);
                drawTile(0, y, zero);
                hasMove = true;
            }
            if (grid[y][2].whatTile.tileNum == 0) {
                drawTile(2, y, grid[y][1].whatTile);
                drawTile(1, y, zero);
                hasMove = true;
            }
            if (grid[y][3].whatTile.tileNum == 0) {
                drawTile(3, y, grid[y][2].whatTile);
                drawTile(2, y, zero);
                hasMove = true;
            }
        }
    }
}

void mergeLeft() {
    for (unsigned char y = 0; y < 4; y++) {
         if (grid[y][3].whatTile.tileNum != 0) {
            if (grid[y][3].whatTile.tileNum == grid[y][2].whatTile.tileNum) {
                drawTile(2, y, computeTile(grid[y][3].whatTile));
                drawTile(3, y, zero);
                hasMove = true;
            }
            else if (grid[y][1].whatTile.tileNum == grid[y][3].whatTile.tileNum && grid[y][2].whatTile.tileNum == 0) {
                drawTile(1, y, computeTile(grid[y][3].whatTile));
                drawTile(3, y, zero);
                hasMove = true;
            }
            else if (grid[y][3].whatTile.tileNum == grid[y][0].whatTile.tileNum && grid[y][1].whatTile.tileNum == 0 && grid[y][2].whatTile.tileNum == 0) {
                drawTile(0, y, computeTile(grid[y][3].whatTile));
                drawTile(3, y, zero);
                hasMove = true;
            }
        }
        if (grid[y][2].whatTile.tileNum != 0) {
            if (grid[y][2].whatTile.tileNum == grid[y][1].whatTile.tileNum) {
                drawTile(1, y, computeTile(grid[y][2].whatTile));
                drawTile(2, y, zero);
                hasMove = true;
            }
            else if (grid[y][2].whatTile.tileNum == grid[y][0].whatTile.tileNum && grid[y][1].whatTile.tileNum == 0) {
                drawTile(0, y, computeTile(grid[y][2].whatTile));
                drawTile(2, y, zero);
                hasMove = true;
            }
        }

        if (grid[y][1].whatTile.tileNum != 0) {
            if (grid[y][1].whatTile.tileNum == grid[y][0].whatTile.tileNum) {
                drawTile(0, y, computeTile(grid[y][1].whatTile));
                drawTile(1, y, zero);
                hasMove = true;
            }
        }
         
       
       
    }
}

void moveLeft() {
    mergeLeft();
    for (unsigned char y = 0; y < 4; y++) {
        if (grid[y][1].whatTile.tileNum != 0) {
            if (grid[y][0].whatTile.tileNum == 0) {
                drawTile(0, y, grid[y][1].whatTile);
                drawTile(1, y, zero);
                hasMove = true;
            }
        }
        if (grid[y][2].whatTile.tileNum != 0) {
            if (grid[y][1].whatTile.tileNum == 0) {
                drawTile(1, y, grid[y][2].whatTile);
                drawTile(2, y, zero);
                hasMove = true;
            }
            if (grid[y][0].whatTile.tileNum == 0) {
                drawTile(0, y, grid[y][1].whatTile);
                drawTile(1, y, zero);
                hasMove = true;
            }
        }
         if (grid[y][3].whatTile.tileNum != 0) {
            if (grid[y][2].whatTile.tileNum == 0) {
                drawTile(2, y, grid[y][3].whatTile);
                drawTile(3, y, zero);
                hasMove = true;
            }
            if (grid[y][1].whatTile.tileNum == 0) {
                drawTile(1, y, grid[y][2].whatTile);
                drawTile(2, y, zero);
                hasMove = true;
            }
            if (grid[y][0].whatTile.tileNum == 0) {
                drawTile(0, y, grid[y][1].whatTile);
                drawTile(1, y, zero);
                hasMove = true;
            }
        }

    
    }
}

void mergeUp() {
    for (unsigned char x = 0; x < 4; x++) {
         if (grid[3][x].whatTile.tileNum != 0) {
            if (grid[3][x].whatTile.tileNum == grid[2][x].whatTile.tileNum) {
                drawTile(x, 2, computeTile(grid[3][x].whatTile));
                drawTile(x, 3, zero);
                hasMove = true;
            }
            else if (grid[3][x].whatTile.tileNum == grid[1][x].whatTile.tileNum && grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 1, computeTile(grid[3][x].whatTile));
                drawTile(x, 3, zero);
                hasMove = true;
            }
            else if (grid[3][x].whatTile.tileNum == grid[0][x].whatTile.tileNum && grid[1][x].whatTile.tileNum == 0 && grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 0, computeTile(grid[3][x].whatTile));
                drawTile(x, 3, zero);
                hasMove = true;
            }
        }

         if (grid[2][x].whatTile.tileNum != 0) {
            if (grid[2][x].whatTile.tileNum == grid[1][x].whatTile.tileNum) {
                drawTile(x, 1, computeTile(grid[2][x].whatTile));
                drawTile(x, 2, zero);
                hasMove = true;
            }
            else if (grid[2][x].whatTile.tileNum == grid[0][x].whatTile.tileNum && grid[1][x].whatTile.tileNum == 0) {
                drawTile(x, 0, computeTile(grid[2][x].whatTile));
                drawTile(x, 2, zero);
                hasMove = true;
            }
        }
        if (grid[1][x].whatTile.tileNum != 0) {
            if (grid[1][x].whatTile.tileNum == grid[0][x].whatTile.tileNum) {
                drawTile(x, 0, computeTile(grid[1][x].whatTile));
                drawTile(x, 1, zero);
                hasMove = true;
            }
        }

       
       
    }
}

void moveUp() {
    mergeUp();
    for (unsigned char x = 0; x < 4; x++) {
        if (grid[1][x].whatTile.tileNum != 0) {
            if (grid[0][x].whatTile.tileNum == 0) {
                drawTile(x, 0, grid[1][x].whatTile);
                drawTile(x, 1, zero);
                hasMove = true;
            }
        }
        if (grid[2][x].whatTile.tileNum != 0) {
            if (grid[1][x].whatTile.tileNum == 0) {
                drawTile(x, 1, grid[2][x].whatTile);
                drawTile(x, 2, zero);
                hasMove = true;
            }
            if (grid[0][x].whatTile.tileNum == 0) {
                drawTile(x, 0, grid[1][x].whatTile);
                drawTile(x, 1, zero);
                hasMove = true;
            }
        }
         if (grid[3][x].whatTile.tileNum != 0) {
            if (grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 2, grid[3][x].whatTile);
                drawTile(x, 3, zero);
                hasMove = true;
            }
            if (grid[1][x].whatTile.tileNum == 0) {
                drawTile(x, 1, grid[2][x].whatTile);
                drawTile(x, 2, zero);
                hasMove = true;
            }
            if (grid[0][x].whatTile.tileNum == 0) {
                drawTile(x, 0, grid[1][x].whatTile);
                drawTile(x, 1, zero);
                hasMove = true;
            }
        }

    
    }
}

void mergeDown() {
    for (unsigned char x = 0; x < 4; x++) {
        if (grid[2][x].whatTile.tileNum != 0) {
            if (grid[2][x].whatTile.tileNum == grid[3][x].whatTile.tileNum) {
                drawTile(x, 3, computeTile(grid[2][x].whatTile));
                drawTile(x, 2, zero);
                hasMove = true;
            }
        }
         if (grid[1][x].whatTile.tileNum != 0) {
            if (grid[2][x].whatTile.tileNum == grid[1][x].whatTile.tileNum) {
                drawTile(x, 2, computeTile(grid[1][x].whatTile));
                drawTile(x, 1, zero);
                hasMove = true;
            }
            else if (grid[1][x].whatTile.tileNum == grid[3][x].whatTile.tileNum && grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 3, computeTile(grid[1][x].whatTile));
                drawTile(x, 1, zero);
                hasMove = true;
            }
        }

        if (grid[0][x].whatTile.tileNum != 0) {
            if (grid[0][x].whatTile.tileNum == grid[1][x].whatTile.tileNum) {
                drawTile(x, 1, computeTile(grid[0][x].whatTile));
                drawTile(x, 0, zero);
                hasMove = true;
            }
            else if (grid[2][x].whatTile.tileNum == grid[0][x].whatTile.tileNum && grid[1][x].whatTile.tileNum == 0) {
                drawTile(x, 2, computeTile(grid[0][x].whatTile));
                drawTile(x, 0, zero);
                hasMove = true;
            }
            else if (grid[3][x].whatTile.tileNum == grid[0][x].whatTile.tileNum && grid[1][x].whatTile.tileNum == 0 && grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 3, computeTile(grid[0][x].whatTile));
                drawTile(x, 0, zero);
                hasMove = true;
            }
        }
       
    }
}

void moveDown() {
    mergeDown();
    for (unsigned char x = 0; x < 4; x++) {
        if (grid[2][x].whatTile.tileNum != 0) {
            if (grid[3][x].whatTile.tileNum == 0) {
                drawTile(x, 3, grid[2][x].whatTile);
                drawTile(x, 2, zero);
                hasMove = true;
            }
        }
        if (grid[1][x].whatTile.tileNum != 0) {
            if (grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 2, grid[1][x].whatTile);
                drawTile(x, 1, zero);
                hasMove = true;
            }
            if (grid[3][x].whatTile.tileNum == 0) {
                drawTile(x, 3, grid[2][x].whatTile);
                drawTile(x, 2, zero);
                hasMove = true;
            }
        }
         if (grid[0][x].whatTile.tileNum != 0) {
            if (grid[1][x].whatTile.tileNum == 0) {
                drawTile(x, 1, grid[0][x].whatTile);
                drawTile(x, 0, zero);
                hasMove = true;
            }
            if (grid[2][x].whatTile.tileNum == 0) {
                drawTile(x, 2, grid[1][x].whatTile);
                drawTile(x, 1, zero);
                hasMove = true;
            }
            if (grid[3][x].whatTile.tileNum == 0) {
                drawTile(x, 3, grid[2][x].whatTile);
                drawTile(x, 2, zero);
                hasMove = true;
            }
        }
    }
}

bool winCondition() {
    for (unsigned char y = 0; y < 4; y++) {
        for (unsigned char x = 0; x < 4; x++) {
            if (grid[y][x].whatTile.tileNum == 2048) {
                return true;
            }
        }
    }
    return false;
}

void clearGrid() {
    for (unsigned char y = 0; y < 4; y++) {
        for (unsigned char x = 0; x < 4; x++) {
            grid[y][x].whatTile = zero;
        }
    } 
}

bool isGridFull() {
    for (unsigned char y = 0; y < 4; y++) {
        for (unsigned char x = 0; x < 4; x++) {
            if (grid[y][x].whatTile.tileNum == 0) return false;
        }
    }

    return true;
}


bool loseCondition() {
    for (short y = 0; y < 4; y++) {
        for (short x = 0; x < 4; x++) {
            if (y + 1 < 4) {
                if (grid[y][x].whatTile.tileNum == grid[y+1][x].whatTile.tileNum) return false;
            }
            if (y - 1 >= 0) {
                if (grid[y][x].whatTile.tileNum == grid[y-1][x].whatTile.tileNum) return false;
            }
            if (x - 1 >= 0) {
                if (grid[y][x].whatTile.tileNum == grid[y][x-1].whatTile.tileNum) return false;
            }
            if (x + 1 < 4) {
                if (grid[y][x].whatTile.tileNum == grid[y][x+1].whatTile.tileNum) return false;
            }
        }
    } 


    return true;
}

#endif