#define _CRT_SECURE_NO_WARNINGS
//  Includes
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <math.h>
#include <random>
#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <chrono>
#include <thread>
#include <iostream>
#include <dos.h> //for delay
#include <conio.h> //for getch()
#include <string.h>
#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Winmm.lib")
#include <mmsystem.h>
#include <gl/glut.h>

using namespace std;
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void Key(int key, int x, int y);
//void KeyUp(unsigned char key, int x, int y);
//void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
//-----------------

//  Global Variables
float seconds = 3;
int selectedBar = 0; // used to determine which bar has the mouse currently over it
int bar1Y = 0;       // holds the Y translation value of bar 1
int bar2Y = 0;       // holds the Y translation value of bar 2
int bar3Y = 0;       // holds the Y translation value of bar 3
int playerY = 0;  // holds the value of the Y coordinate of the ball
int playerX = 400;
float playerRed = 1;
float playerGreen = 1;
float playerBlue = 1;
int angle = 0;
double motionTime = 1;
int score = 0;
int obstacleCount = 3;
bool coinFlag = false;

int gameOverY = 2500;

int coinXRight = 660;
float coinYRight;
bool rightCoinFlag = false;

int randomArray[1500];
int delay[1000];


int life = 3;
int coins = 0;

int coinXMid = 380;
float coinYMid;
bool midCoinFlag = false;

int coinXLeft = 115;
float coinYLeft; //220 added
bool leftCoinFlag = false;

float movingBarrier = 0;

int obstacleXMid = 400;
float obstacleYMid;
bool midObstacleFlag = false;

int obstacleXRight = 660;
float obstacleYRight;
bool rightObstacleFlag = false;

int obstacleXLeft = 115;
float obstacleYLeft;
bool leftObstacleFlag = false;

//-----------------

void main(int argc, char** argr) {
    srand(time(0));
    glutInit(&argc, argr);
    obstacleYMid = (rand() % 1500) + 1000;
    obstacleYRight = (rand() % 1500) + 1000;
    obstacleYLeft = (rand() % 1500) + 1000;
    if (obstacleYMid - obstacleYLeft < 300)
        obstacleYMid += 500;
    if (obstacleYLeft - obstacleYMid < 300)
        obstacleYMid += 500;

    coinYRight = -900;
    coinYMid = -900;
    coinYLeft = -900;

    for (int i = 0; i < 1000; i++)
    {
        randomArray[i] = (rand() % 1000) + 1;
        cout << randomArray[i];
        cout << " :array random number";
        cout << "\n";
    }

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Game");
    glutTimerFunc(0, Timer, 0);
    glutDisplayFunc(Display);
    glutSpecialFunc(Key);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);

    glutMainLoop();
}

// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void gameOver()
{
    PlaySound(TEXT("./SoundEffects/game over.wav"), NULL, SND_ASYNC | SND_FILENAME);
    gameOverY = 400;
    seconds = 0;
}

void redAlert()
{
    cout << life;
    cout << "life";
    if (life == 2)
    {
        playerGreen = 0.6;
        playerBlue = 0.6;
        playerRed = 1;
    }
    else
    {
        if (life == 1)
        {
            playerGreen = 0.2;
            playerBlue = 0.2;
            playerRed = 1;
        }
        else
        {
            if (life == 0)
            {
                playerGreen = 0;
                playerBlue = 0;
                playerRed = 0.4;
            }

        }
    }
    cout << playerRed, ": PLAYER RED";
    cout << "\n";
    cout << playerGreen, ": PLAYER GREEN";
    cout << "\n";
    cout << playerBlue, ": PLAYER BLUE";
    cout << "\n";

    if (life == 0)
    {
        gameOver();
    }

}

void greenAlert()
{
    PlaySound(TEXT("./SoundEffects/New life gained.wav"), NULL, SND_ASYNC | SND_FILENAME);
    coins = 0;
    life++;
    motionTime *= 1.5;
    if (life == 2)
    {
        playerGreen = 0.6;
        playerBlue = 0.6;
        playerRed = 1;
    }
    else
    {
        if (life >= 3)
        {
            playerGreen = 1;
            playerBlue = 1;
            playerRed = 1;
        }
    }
}


//this is the method used to print text in OpenGL
//there are three parameters,
//the first two are the coordinates where the text is display,
//the third coordinate is the string containing the text to display
void print(int x, int y, char* string)
{
    int len, i;

    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    //get the length of the string to display
    len = (int)strlen(string);

    //loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}


// draw triangle
void drawTriangle(int x, int y, int w, int h) {
    glBegin(GL_POLYGON);
    glVertex2f(x, h + y);
    glVertex2f(x - (w / 2), y);
    glVertex2f(x + (w / 2), y);
    glEnd();
}



// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}




// Keyboard handler function
//   key: the key pressed on the keyboard
//   x  : the X coordinate of the mouse cursor at the moment of pressing the key
//   y  : the Y coordinate of the mouse cursor at the moment of pressing the key
void Key(int key, int x, int y) {
    // check the selected bar, to decide which variable to control: bar1Y, bar2Y or bar3Y
    if (key == GLUT_KEY_LEFT)
        if (playerX == 400)
            playerX = 120;
        else
        {
            if (playerX == 675)
                playerX = 400;
            else
            {
                //produce can't do sound
            }
        }
    if (key == GLUT_KEY_RIGHT)
        if (playerX == 400)
            playerX = 675;
        else
        {
            if (playerX == 120)
                playerX = 400;
            else
            {
                //produce can't do sound
            }
        }



    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();
}

// KeyboardUp handler function
//   similar to the Keyboard handler function except that it is called only when the key is released


// Mouse handler function
//   button: the mouse button clicked: left, right or middle
//   state:  the state of the button: clicked (down) or released (up)
//   x  : the X coordinate of the mouse cursor at the moment of clicking the mouse button
//   y  : the Y coordinate of the mouse cursor at the moment of clicking the mouse button


void Timer(int value) {
    score++;
    angle++;
    srand(time(0));
    // set the ball's Y coordinate to a random number between 10 and 780 (since the window's height is 800)
    if (movingBarrier < -100)
        movingBarrier = 0;
    movingBarrier -= motionTime;


    if (obstacleYLeft > -100)
        obstacleYLeft -= motionTime;
    else
    {
        srand(time(0));
        int random = randomArray[rand() % 1000];
        //cout << random;
        //cout << ": OBSTACLE LEFT";
        //cout << "\n";
        if (random > 600)
        {
            leftObstacleFlag = false;
            obstacleYLeft = randomArray[randomArray[randomArray[rand() % 1000]]] + 1000;
            obstacleCount++;
            if (obstacleCount == 6)
            {
                coinFlag = true;
                obstacleCount = 0;
            }

        }
    }

    if (obstacleYMid > -100)
        obstacleYMid -= motionTime;
    else
    {
        srand(time(0));
        int random = randomArray[randomArray[rand() % 1000]];

        if (random > 600)
        {
            midObstacleFlag = false;
            obstacleYMid = randomArray[randomArray[rand() % 1000]] + 1000;
            if (obstacleYMid - obstacleYLeft < 300)
                obstacleYMid += 300;
            obstacleCount++;
            if (obstacleCount == 6)
            {
                coinFlag = true;
                obstacleCount = 0;
            }
        }

    }
    srand(time(NULL));
    if (obstacleYRight > -100)
        obstacleYRight -= motionTime;
    else {
        srand(time(0));
        int random = randomArray[randomArray[randomArray[rand() % 1000]]];
        //cout << random;
        //cout << ": OBSTACLE RIGHT";
        //cout << "\n";
        if (random > 600)
        {
            rightObstacleFlag = false;
            obstacleYRight = randomArray[rand() % 1000] + 1000;
            if ((obstacleYRight - obstacleYMid) < 300 && (obstacleYRight - obstacleYMid) > 0)
                obstacleYRight += 300;
            if ((obstacleYMid - obstacleYRight) < 300 && (obstacleYMid - obstacleYRight) > 0)
                obstacleYMid += 300;

            obstacleCount++;
            if (obstacleCount == 6)
            {
                coinFlag = true;
                obstacleCount = 0;
            }
        }
    }
    srand(time(0));

    if (coinYLeft > -700)
        coinYLeft -= motionTime;
    else
    {
        if (randomArray[rand() % 1000] % 3 == 0 && coinFlag == true)
        {
            coinFlag = false;
            int random = randomArray[randomArray[randomArray[randomArray[rand() % 1000]]]];

            leftCoinFlag = false;
            coinYLeft = 1000 + randomArray[rand() % 1000];
            if (abs(coinYLeft - obstacleYLeft) <= 200)
            {
                coinYLeft += 200;
                if (abs(coinYLeft - obstacleYLeft) <= 200)
                    coinYLeft += 200;
            }
        }

    }
    if (coinYMid > -700)
        coinYMid -= motionTime;
    else
    {
        if (randomArray[rand() % 1000] % 3 == 1 && coinFlag == true)
        {
            coinFlag = false;
            coinYMid = 1120 + randomArray[randomArray[randomArray[rand() % 1000]]];
            midCoinFlag = false;
            if (abs(coinYMid - obstacleYMid) >= 150 && (abs(coinYMid - obstacleYMid) <= 400))
            {
                coinYMid += 200;
                if (abs(coinYMid - obstacleYMid) >= 150 && (abs(coinYMid - obstacleYMid) <= 400))
                    coinYMid += 200;
            }
        }
    }


    if (coinYRight > -700)
        coinYRight -= motionTime;
    else
    {
        if (randomArray[rand() % 1000] % 3 == 2 && coinFlag == true)
        {
            coinFlag = false;
            coinYRight = randomArray[randomArray[randomArray[randomArray[rand() % 1000]]]] + 1300;
            rightCoinFlag = false;
            if (abs(coinYRight - obstacleYRight) >= 300 && (abs(coinYRight - obstacleYRight) <= 500))
            {
                coinYRight += 200;
                if (abs(coinYRight - obstacleYRight) >= 300 && (abs(coinYRight - obstacleYRight) <= 500))
                    coinYRight += 200;
            }
        }

    }

    //checking collisions
    if (playerX == 675 && obstacleYRight < 350 && obstacleYRight >200 && rightObstacleFlag == false) //120,400,675
    {
        PlaySound(TEXT("./SoundEffects/Obstacle.wav"), NULL, SND_ASYNC | SND_FILENAME);
        rightObstacleFlag = true;
        life--;
        redAlert();
    }
    if (playerX == 400 && obstacleYMid < 350 && obstacleYMid >200 && midObstacleFlag == false) //120,400,675
    {
        PlaySound(TEXT("./SoundEffects/Obstacle.wav"), NULL, SND_ASYNC | SND_FILENAME);
        midObstacleFlag = true;
        life--;

        redAlert();
    }
    if (playerX == 120 && obstacleYLeft < 350 && obstacleYLeft > 200 && leftObstacleFlag == false) //120,400,675
    {
        PlaySound(TEXT("./SoundEffects/Obstacle.wav"), NULL, SND_ASYNC | SND_FILENAME);
        leftObstacleFlag = true;
        life--;
        redAlert();

    }
    if (playerX == 120 && coinYLeft < 350 && coinYLeft > 200 && leftCoinFlag == false) //120,400,675
    {
        PlaySound(TEXT("./SoundEffects/Coin SoundEffect.wav"), NULL, SND_ASYNC | SND_FILENAME);
        score += 300;
        leftCoinFlag = true;
        coins++;
        coinYLeft = -1000;
        if (coins == 5)
        {
            greenAlert();
        }
    }
    if (playerX == 400 && coinYMid < 350 && coinYMid > -100 && midCoinFlag == false) //120,400,675
    {
        PlaySound(TEXT("./SoundEffects/Coin SoundEffect.wav"), NULL, SND_ASYNC | SND_FILENAME);
        midCoinFlag = true;
        score += 300;
        coins++;
        coinYMid = -1000;
        if (coins == 5)
        {
            greenAlert();
        }
    }
    if (playerX == 675 && coinYRight < 150 && coinYRight > -100 && rightCoinFlag == false) //120,400,675
    {
        score += 300;
        PlaySound(TEXT("./SoundEffects/Coin SoundEffect.wav"), NULL, SND_ASYNC | SND_FILENAME);
        rightCoinFlag = true;
        coins++;
        coinYRight = -1000;

        if (coins == 5)
        {
            greenAlert();
        }
    }





    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();

    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(seconds, Timer, 0);
}


void Display() {
    glClear(GL_COLOR_BUFFER_BIT);


    // if the mouse is over the 1st bar, color it yellow, otherwise black
    if (selectedBar == 1)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 0);
    // draw the 1st vertical rectangle, which is fixed and won't move
    drawRect(10, 120, 20, 560); // rectangle 1

    /**
    * note:
    *   read the next comments from down to up, since we are dealing with stacks to handle transformations (LIFO).
    *   the comments are numbered to help you read them in the correct order
    *   3 nested stacks are used to handle the 3 parts of the arm, which look like that:
    *
    *
    *    Draw Rect 1
    *
    *    Stack 3 Push <-------------------.
    *      Translate Bar 1                |
    *      Draw Rect 2                    |
    *                                     |
    *      Stack 2 Push <-------------.   |
    *        Translate Bar 2          |   |
    *        Draw Rect 3 and 4        |   |
    *                                 |   |
    *        Stack 1 Push <-------.   |   |
    *          Translate Bar 3    |   |   |
    *          Draw Rect 5 and 6  |   |   |
    *                             |   |   |
    *        Stack 1 Pop  <------'    |   |
    *      Stack 2 Pop  <------------'    |
    *    Stack 3 Pop  <------------------'
    *
    * order of execution:
    *  1. Draw Rect 5 and 6
    *  2. Translate Bar 3
    *  3. Draw Rect 3 and 4
    *  4. Translate Bar 2
    *  5. Draw Rect 2
    *  6. Translate Bar 1
    */

    //===== 3. create a new stack to control the 1st part of the arm (rectangles 2 and 3)
    //         since this is a stack, rectangles 4, 5 and 6 will be already drawn and will translate with rectangles 2 and 3
    glPushMatrix();
    glTranslatef(0, bar1Y, 0);
    glColor3f(0.5, 0.5, 0.5);
    drawRect(30, 390, 150, 20); // rectangle 2

    // if the mouse is over the 2nd bar, color it yellow, otherwise black
    if (selectedBar == 2)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 0);
    drawRect(180, 300, 20, 200); // rectangle 3

    //===== 2. create a new stack to control the 2nd part of the arm (rectangles 4 and 5)
    //         since this is a stack, rectangle 6 will be already drawn and will translate with rectangles 4 and 5
    glPushMatrix();
    glTranslatef(0, bar2Y, 0);
    glColor3f(0.5, 0.5, 0.5);
    drawRect(200, 390, 100, 20); // rectangle 4

    // if the mouse is over the 3rd bar, color it yellow, otherwise black
    if (selectedBar == 3)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 0);
    drawRect(300, 370, 20, 60); // rectangle 5

    //===== 1. create a new stack to control the 3rd part of the arm (rectangle 6)
    glPushMatrix();
    glTranslatef(0, bar3Y, 0);
    glColor3f(0.5, 0.5, 0.5);
    drawRect(320, 390, 60, 20);// rectangle 6


        //===== lane barrier left

    //===== background
    glPushMatrix();
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(0, 0, 1000, 1000);// rectangle 6



    //===== lane barrier right
    glPushMatrix();
    glTranslatef(0, bar3Y, 0);
    glColor3f(1, 1, 1);
    drawRect(530, 0, 10, 1000);// rectangle 6


    glPushMatrix(); //lane moving barrier 1
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier, 10, 50);

    glPushMatrix(); //lane moving barrier 2
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 100, 10, 50);

    glPushMatrix(); //lane moving barrier 3
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 200, 10, 50);

    glPushMatrix(); //lane moving barrier 4
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 300, 10, 50);

    glPushMatrix(); //lane moving barrier 5
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 400, 10, 50);

    glPushMatrix(); //lane moving barrier 6
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 500, 10, 50);

    glPushMatrix(); //lane moving barrier 7
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 600, 10, 50);

    glPushMatrix(); //lane moving barrier 8
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 700, 10, 50);

    glPushMatrix(); //lane moving barrier 0
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 800, 10, 50);

    glPushMatrix(); //lane moving barrier 0
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(530, movingBarrier + 900, 10, 50);


    glPushMatrix(); //lane barrier left
    glTranslatef(0, bar3Y, 0);
    glColor3f(1, 1, 1);
    drawRect(250, 0, 10, 1000);// rectangle 6



    glPushMatrix(); //lane moving barrier 1
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier, 10, 50);

    glPushMatrix(); //lane moving barrier 2
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 100, 10, 50);

    glPushMatrix(); //lane moving barrier 3
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 200, 10, 50);

    glPushMatrix(); //lane moving barrier 4
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 300, 10, 50);

    glPushMatrix(); //lane moving barrier 5
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 400, 10, 50);

    glPushMatrix(); //lane moving barrier 6
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 500, 10, 50);

    glPushMatrix(); //lane moving barrier 7
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 600, 10, 50);

    glPushMatrix(); //lane moving barrier 8
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 700, 10, 50);

    glPushMatrix(); //lane moving barrier 9
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 800, 10, 50);

    glPushMatrix(); //lane moving barrier 10
    glTranslatef(0, bar3Y, 0);
    glColor3f(0, 0, 0);
    drawRect(250, movingBarrier + 900, 10, 50);



    glPushMatrix(); //player body
    glTranslatef(-12, 230, 0);
    glColor3f(playerRed, playerGreen, playerBlue);
    drawRect(playerX, 0, 20, 70);

    glPushMatrix(); //player head
    glColor3f(playerRed, playerGreen, playerBlue);
    glTranslatef(10, -220, 0);
    drawCircle(playerX, 300, 20);


    glPushMatrix(); //player right hand
    glTranslatef(7, 260, 0);
    glColor3f(playerRed, playerGreen, playerBlue);
    drawRect(playerX, 0, 30, 10);

    glPushMatrix(); //player left hand
    glTranslatef(-48, 0, 0);
    glColor3f(playerRed, playerGreen, playerBlue);
    drawRect(playerX, 0, 30, 10);

    glPushMatrix(); //player left leg
    glTranslatef(25, -70, 0);
    glColor3f(playerRed, playerGreen, playerBlue);
    drawRect(playerX, 0, 8, 40);

    glPushMatrix(); //player left leg
    glTranslatef(20, 0, 0);
    glColor3f(playerRed, playerGreen, playerBlue);
    drawRect(playerX, 0, 8, 40);

    glPushMatrix(); //COINRight
    glColor3f(1, 1, 0);
    drawCircle(coinXRight, coinYRight, 20);

    //glPushMatrix(); //obstacleMid
    //glColor3f(1, 0, 0);
    //glTranslatef(10, -220, 0);
    //drawTriangle(obstacleXMid, obstacleYMid,220,80);

    //glPushMatrix(); //obstacleLeft
    //glColor3f(1, 0, 0);
    //drawTriangle(obstacleXLeft, obstacleYLeft, 220,80);

    //glPushMatrix(); //obstacleRight
    //glColor3f(1, 0, 0);
    //drawTriangle(obstacleXRight, obstacleYRight, 220, 80);

    glPushMatrix(); //COINLEFT
    glColor3f(1, 1, 0);
    drawCircle(coinXLeft, coinYLeft, 20);

    glPushMatrix(); //COINMID
    glColor3f(1, 1, 0);
    drawCircle(coinXMid, coinYMid, 20);

    glPushMatrix();
    glColor3f(1, 1, 1);
    char* p0s[20];
    sprintf((char*)p0s, "Lives: %d", life);
    print(50, 750, (char*)p0s);

    glPushMatrix();
    glColor3f(1, 1, 1);
    char* p1s[20];
    sprintf((char*)p1s, "Coins: %d", coins);
    print(50, 725, (char*)p1s);

    glPushMatrix();
    glColor3f(1, 1, 1);
    char* p4s[20];
    sprintf((char*)p4s, "Score: %d", score / 20);
    print(50, 700, (char*)p4s);


    glPushMatrix();
    glColor3f(1, 0, 0);
    char* p2s[20];
    sprintf((char*)p2s, "Game Over");
    print(350, gameOverY, (char*)p2s);



    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(obstacleXLeft, obstacleYLeft, 0);
    glPushMatrix();
    glColor3f(1, 0, 0);
    glRotatef(angle, 0, 0, 1);
    drawRect(-30, -5, 60, 10);
    glPopMatrix();
    glTranslatef(obstacleXLeft, obstacleYLeft, 0);
    glPushMatrix();
    glRotatef(angle + 90, 0, 0, 1);
    drawRect(-30, -5, 60, 10);
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(obstacleXMid, obstacleYMid, 0);
    glPushMatrix();
    glColor3f(1, 0, 0);
    glRotatef(angle, 0, 0, 1);
    drawRect(-30, -5, 60, 10);
    glPopMatrix();
    //  glTranslatef(obstacleXMid, obstacleYMid, 0);
    glPushMatrix();
    glRotatef(angle + 90, 0, 0, 1);
    drawRect(-30, -5, 60, 10);
    glPopMatrix();
    glPopMatrix();



    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(obstacleXRight, obstacleYRight, 0);
    glPushMatrix();
    glColor3f(1, 0, 0);
    glRotatef(angle, 0, 0, 1);
    drawRect(-30, -5, 60, 10);
    glPopMatrix();
    //  glTranslatef(obstacleXMid, obstacleYMid, 0);
    glPushMatrix();
    glRotatef(angle + 90, 0, 0, 1);
    drawRect(-30, -5, 60, 10);
    glPopMatrix();
    glPopMatrix();



    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();


    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();


    glFlush();
}
