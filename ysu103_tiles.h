//DEMO 1 https://www.youtube.com/watch?v=2Reih4ck2CU
//DEMO 2 https://youtu.be/osiF3AG_7-I

#ifndef TILES_H
#define TILES_H

struct tile {
    unsigned char b;
    unsigned char g;
    unsigned short r;

    //0, 2, 4, 8, ... 2048
    //0 being empty
    unsigned int tileNum;
};
struct tileLocation {
    unsigned char colStart;
    unsigned char colEnd;
    unsigned char rowStart;
    unsigned char rowEnd;
    

    tile whatTile;
};

const tile zero = {0xFF, 0xFF, 0xFF, 0};
const tile two = { 0xDA, 0xE4, 0xEE, 2};
const tile four = {0xB6, 0xD8, 0xEB, 4};
const tile eight = {0x79, 0xB1, 0xF2, 8};
const tile sixteen = {0x63, 0x95, 0xF5, 16};
const tile thirtyTwo = {0x60, 0x7C, 0xF6, 32};
const tile sixtyFour = {0x3B, 0x5E, 0xF6, 64};
const tile TwentyEight = {0x73, 0xCF, 0xED, 128};
const tile FiftySix = {0x62, 0xCC, 0xED, 256};
const tile Twelve = {0x50, 0xC8, 0xED, 512};
const tile TwentyFour = {0x3F, 0xC5, 0xED, 1024};
const tile FourtyEight {0x2D, 0xC2, 0xED, 2048};

/*

a b c d 
e f g h
i j k l
m n o p


*/

tileLocation aTile = {0, 32, 0, 31, zero};
tileLocation bTile = {34, 64, 0, 31, zero};
tileLocation cTile = {66, 97, 0, 31, zero};
tileLocation dTile = {99, 129, 0, 31, zero};

tileLocation eTile = {0, 32, 33, 63, zero};
tileLocation fTile = {34, 64, 33, 63, zero};
tileLocation gTile = {66, 97, 33, 63, zero};
tileLocation hTile = {99, 129, 33, 63, zero};

tileLocation iTile = {0, 32, 65, 95, zero};
tileLocation jTile = {34, 64, 65, 95, zero};
tileLocation kTile = {66, 97, 65, 95, zero};
tileLocation lTile = {99, 129, 65, 95, zero};

tileLocation mTile = {0, 32, 97, 128, zero};
tileLocation nTile = {34, 64, 97, 128, zero};
tileLocation oTile = {66, 97, 97, 128, zero};
tileLocation pTile = {99, 129, 97, 128, zero};


//game logic done in this matrix
tileLocation grid[4][4] = {
    { aTile, bTile, cTile, dTile },
    { eTile, fTile, gTile, hTile },
    { iTile, jTile, kTile, lTile },
    { mTile, nTile, oTile, pTile }
};

//5x8 font
unsigned char numbers[10][5] {
    {0b01111110, 0b10000001, 0b10000001, 0b10000001, 0b01111110}, //0
    {0x80, 0x82, 0xFF, 0x80, 0x80}, // 1
    {0xE2, 0x91, 0x91, 0x91, 0x8E}, // 2
    {0x42, 0x81, 0x91, 0x91, 0x6E}, // 3
    {0x18, 0x14, 0x12, 0x11 ,0xFF}, // 4
    {0x8F, 0x89, 0x89, 0x89, 0x71}, // 5 
    {0x7E, 0x89, 0x89, 0x89, 0x72}, // 6
    {0x01, 0xC1, 0x31, 0x0D, 0x03}, // 7
    {0x76, 0x89, 0x89, 0x89, 0x76}, // 8 
    {0x4E, 0x89, 0x89, 0x89, 0x7E} //9
};


#endif