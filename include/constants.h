#pragma once
const static int TILE_WIDTH = 150;
const static int TILE_HEIGHT = 150;

constexpr auto ANIMATION_SPEED = 0.2f;

const static int STATUS_HEIGHT = 80;

const static int DIR_UP = 0;
const static int DIR_RIGHT = 1;
const static int DIR_DOWN = 2;
const static int DIR_LEFT = 3;
const static int DIR_NONE = 4; // no rotation
const static int MAX_DIRECTION = DIR_LEFT; // valid file input


const static int BG_IMAGE = 14;
const static int SOLVED_IMAGE = 15;
const static int GAMEOVER_IMAGE = 16;
const static int WELCOME_IMAGE = 17; // to change

const static int WATER_SOUND = 0; 
const static int BG_SOUND = 1;
const static int CLICK_SOUND = 2; 

const static bool LOOP = true;
const static bool NO_LOOP = false;

const static bool FLOW = true;

const static int WIN_HEIGHT = 1000;
const static int WIN_WIDTH = 1400;

const static int WIN_BORDER_HEIGHT = 50;
const static int WIN_BORDER_WIDTH = 100;


const static int TJUNCTION = 0;
const static int CROSS = 1;
const static int STRAIGHT = 2;
const static int TURN = 3;
const static int TAP = 4;
const static int SINK = 5;
const static int EMPTY = 6;
const static int MAX_TILE_TYPE = EMPTY; // valid file input

const static char* OPEN_FILE_ERR = "Cannot open game file";
const static char* EXTRA_DATA_ERR = "Corrupted input file (extra data) at line ";
const static char* MISSING_DATA_ERR = "Missing or unexpected data in file at line ";
const static char* INCOMPLETE_ERR = "Incomplete map at line ";
