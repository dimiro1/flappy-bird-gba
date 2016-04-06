#include "mylib.h"
#include <stdlib.h>
#include "text.h"
#include "bird.h"
#include "startScreen.h"
#include "pipeNeckBottom.h"
#include "pipeNeckTop.h"
#include "pipeBody.h"

#define NUMOBJS 7

#define MAXHEIGHT 149

#define PIPE_MARGIN 1

typedef struct {
    int showing;
    int col;
    int topHeight;
    int gapHeight;
} PIPE;

typedef struct {
    int row;
    int col;
} BIRD;

// State enum definition
enum GBAState {
    START,
    START_NO_DRAW,
    PLAY,
    PLAY_FLY,
    GAME_OVER
};

const int birdWidth = BIRD_WIDTH;
const int birdHeight = BIRD_HEIGHT;
const int pipeBodyWidth = PIPEBODY_WIDTH;
const int pipeBodyHeight = PIPEBODY_HEIGHT;
const int pipeNeckWidth = PIPENECKBOTTOM_WIDTH;
const int pipeNeckHeight = PIPENECKBOTTOM_HEIGHT;
const int pipeMargin = 1;
const int delayTime = 10000;

const int numPipes = 3;

int detectCollision(BIRD *bird, PIPE *pipe);

void drawPipe(PIPE *pipe);

int score;

int main() {

    REG_DISPCTL = MODE3 | BG2_ENABLE;

    enum GBAState state = START;
    int startDownLastFrame = 0;

    while (1) {
        waitForVblank();
        switch (state) {
            case START:
                drawImage3(0, 0, STARTSCREEN_WIDTH, STARTSCREEN_HEIGHT, startScreen);
                drawImage3(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 4, BIRD_WIDTH, BIRD_HEIGHT, bird);
                drawString(30, (SCREEN_WIDTH - calcStringWidth("Flappy Bird")) / 2, "Flappy Bird", MAGENTA);
                drawString(50, (SCREEN_WIDTH - calcStringWidth("Press START to start")) / 2, "Press START to start",
                           WHITE);
                PIPE testPipe = {1, 70, 70, 40};
                drawPipe(&testPipe);
                state = START_NO_DRAW;
                break;
            case START_NO_DRAW:
                if (KEY_DOWN_NOW(BUTTON_START) && !startDownLastFrame) {
                    state = PLAY;
                }
                break;
            case PLAY:
                fillScreen(CYAN);
                drawImage3(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 4, BIRD_WIDTH, BIRD_HEIGHT, bird);
                break;

        }

        if (KEY_DOWN_NOW(BUTTON_START)) {
            startDownLastFrame = 1;
        } else {
            startDownLastFrame = 0;
        }
    }


}

int detectCollision(BIRD *bird, PIPE *pipe) {
    if (bird->row > (pipe->topHeight - 1) && (bird->row + birdHeight) < pipe->topHeight + pipe->gapHeight) {
        return 0;
    } else {
        if ((bird->col + birdWidth) < pipe->col || bird->col > pipe->col + pipeNeckWidth) {
            return 0;
        } else {
            return 1;
        }
    }
}

void drawPipe(PIPE *pipe) {
    if (!pipe->showing) {
        return;
    }
    for (int i = 0; i < pipe->topHeight - pipeNeckHeight; ++i) {
        drawImage3(i, pipe->col + pipeMargin, pipeBodyWidth, pipeBodyHeight, pipeBody);
    }
    for (int i = 0; i < pipeBodyHeight; ++i) {
        drawImage3(pipe->topHeight - pipeNeckHeight, pipe->col, pipeNeckWidth, pipeNeckHeight, pipeNeckTop);
    }
    for (int i = 0; i < pipeBodyHeight; ++i) {
        drawImage3(i + pipe->topHeight + pipe->gapHeight, pipe->col, pipeNeckWidth, pipeNeckHeight, pipeNeckBottom);
    }
    for (int i = 0; i < SCREEN_HEIGHT - (pipe->topHeight + pipe->gapHeight + pipeNeckHeight); ++i) {
        drawImage3(i + pipe->topHeight + pipe->gapHeight + pipeNeckHeight, pipe->col + pipeMargin, pipeBodyWidth,
                   pipeBodyHeight, pipeBody);
    }
}