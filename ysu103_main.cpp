//DEMO 1 https://www.youtube.com/watch?v=2Reih4ck2CU
//DEMO 2 https://youtu.be/osiF3AG_7-I

#include <avr/io.h>
#include "spiavr.h"  
#include "ysu103_ST7735.h"
#include "ysu103_4D7S.h"
#include "ysu103_tiles.h"
#include "ysu103_game_logic.h"
#include "timerISR.h"
#include "periph.h"

#define NUM_TASKS 4


unsigned long GCD_PERIOD = 1;
unsigned long SCORE_PERIOD = 1;
unsigned long BT_PERIOD = 50;
unsigned long JOY_PERIOD = 1;
unsigned long GAME_PERIOD = 1;

typedef struct _task{
	signed char state; 		
	unsigned long period; 		
	unsigned long elapsedTime; 	
	int (*TickFct)(int); 		
} task;

task tasks[NUM_TASKS];

void TimerISR() {
	for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   
		if ( tasks[i].elapsedTime == tasks[i].period ) {           
			tasks[i].state = tasks[i].TickFct(tasks[i].state); 
			tasks[i].elapsedTime = 0;                          
		}
		tasks[i].elapsedTime += GCD_PERIOD;                        
	}
    seed++;
}

bool gameStart = false;

enum Score_State {S_Start, D1, D2, D3, D4};
int TckFct_Score(int state) {
    switch (state) {
        case S_Start:
            state = D1;
            break;
        case D1:
            state = D2;
            break;
        case D2:
            state = D3;
            break;
        case D3:
            state = D4;
            break;
        case D4:
            state = D1;
            break;
    }
    switch (state) {
        case D1:
            if(score >= 1000) {
                PORTB &= 0xFE; 
                shiftRegister(nums[(score / 1000) % 10]);
                PORTC |= 0b010000;
                PORTC |= 0b100000;
                PORTB |= 0x02;
            }
            break;
        case D2:
            if (score >= 100) {
                PORTB &= 0b111101;
                shiftRegister(nums[(score / 100) % 10]);
                PORTB |= 0x01;
                PORTC |= 0b010000;
                PORTC |= 0b100000;
            }
            break;
        case D3:
            if (score >= 10) {
                PORTC = 0b011111;
                shiftRegister(nums[(score / 10) % 10]);
                PORTC |= 0b010000;
                PORTB |= 0x02;
                PORTB |= 0x01;
            }
            break;
        case D4:
            PORTC = 0b101111;
            shiftRegister(nums[score % 10]);
            PORTC |= 0b100000;
            PORTB |= 0x02;
            PORTB |= 0x01;  
            break;   
    }
    return state;
}

enum Bt_State {B_Start, off, release, on};
int TckFct_Bt(int state) {
    switch (state) {
        case B_Start:
            state = off;
            break;
        case off:
            if (PINC & 0x04) {
                state = release;
            }
            break;
        case release:
            if (!(PINC & 0x04)) {
                state = on;
            }
            break;
        case on:
            state = off;
            break;
    }
    switch (state) {
        case on:
            if (!gameStart)        
                gameStart = true; 
            else if (gameStart)
                gameStart = false;
            break;
    }
    return state;
}

enum Joy_State {J_Start, J_rest, up , down, left, right, J_stop};
int TckFct_Joy(int state) {
    switch (state) {
        case J_Start:
            state = J_rest;
            break;
        case J_rest:
            if (ADC_read(0) > 650) {
                state = up;
            }
            else if (ADC_read(0) < 400) {
                state = down;
            }
            else if (ADC_read(1) > 650) {
                state = right;
            }
            else if (ADC_read(1) < 400) {
                state = left;
            }
            break;
        case up: 
            if (ADC_read(0) <= 544) {
                state = J_rest;
            }
            break;
        case down: 
            if (ADC_read(0) <= 544) {
                state = J_rest;
            }
            break;
        case right: 
            if (ADC_read(1) <= 544) {
                state = J_rest;
            }
            break; 
        case left: 
            if (ADC_read(1) <= 544) {
                state = J_rest;
            }
            break;
    }
    switch (state) {
        case right:
            moveRight();
            if (hasMove) {
                spawnRandTile();
                hasMove = false;
            }
            break;
        case J_rest:
            break;
        case left:
            moveLeft();
            if (hasMove) {
                spawnRandTile();
                hasMove = false;
            }
            break;
        case down:
            moveDown();
            if (hasMove) {
                spawnRandTile();
                hasMove = false;
            }
            break;
        case up:
            moveUp();
            if (hasMove) {
                spawnRandTile();
                hasMove = false;
            }
            break;
    }
    return state;
}
enum Game_State {G_Start, gameON, gameOff, gameStop};
int TckFct_Game(int state) {
    switch (state) {
        case G_Start:
            state = gameOff;
            score = 0;
            ST7735_init();
            set_screen();
            clearGrid();
            titleScreen();
            startBtScreen();
            
            break;
        case gameOff:

            if (gameStart) {
                state = gameON;

                set_screen();
                setGrid();


                randTileStartGame();


            }
            break;


        case gameON:
            if (winCondition()) {
                set_screen();
                winScreen();
                state = gameStop;
            }
            else if (isGridFull() && loseCondition()) {
                set_screen();
                loseScreen();
                state = gameStop;
            }
            else if (!gameStart)
                state = G_Start;
            break;
        case gameStop: 
            if (!gameStart)
                state = G_Start;
            break;
    }
    switch (state) {
        case gameON:
            break;
    }


    return state;
}

int main(void) {
    ADC_init();
    SPI_INIT();

    DDRC = 0b110000;
    PORTC = 0x00;

    DDRB = 0xFF;
    PORTB = 0x00;

    DDRD = 0xFF;
    PORTD = 0x80;

    TimerSet(GCD_PERIOD);
    TimerOn();


    tasks[0].state = S_Start;
    tasks[0].period = SCORE_PERIOD;
    tasks[0].elapsedTime = SCORE_PERIOD;
    tasks[0].TickFct = &TckFct_Score;
    
    tasks[1].state = B_Start;
    tasks[1].period = BT_PERIOD;
    tasks[1].elapsedTime = BT_PERIOD;
    tasks[1].TickFct = &TckFct_Bt;
    
    tasks[2].state = J_Start;
    tasks[2].period = JOY_PERIOD;
    tasks[2].elapsedTime = JOY_PERIOD;
    tasks[2].TickFct = &TckFct_Joy;

    tasks[3].state = G_Start;
    tasks[3].period = GAME_PERIOD;
    tasks[3].elapsedTime = GAME_PERIOD;
    tasks[3].TickFct = &TckFct_Game;


    while (1) {
    }

    return 0;
}