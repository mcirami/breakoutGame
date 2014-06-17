//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// paddel size
#define PADDLEHEIGHT 12
#define PADDLEWIDTH 68

// brick size
#define BRICKHEIGHT 10
#define BRICKWIDTH 35

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);
void initText(GWindow window, int lives, int points, int bricks);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);
    
    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

     // number of bricks initially
    int bricks = COLS * ROWS;

    // number of points initially
    int points = 0;
    
    // number of lives initially
    int lives = LIVES;

    // Sets speed of ball
    double velocityX = drand48() * 5;
    double velocityY = 5;
    
    // Wait for click to start game or continue after life lost
    begin:
    waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {   
        // Moves paddle along with mouse
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - PADDLEWIDTH / 2;
                double y = (getHeight(window) - PADDLEHEIGHT) / 1.1;
                setLocation(paddle, x, y);
            }
        }
        
        // Makes ball bounce off side of window
        move(ball, velocityX, 0);
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = -velocityX;
        }
        else if (getX(ball) <= 0)
        {
            velocityX = -velocityX;
        }
        
        pause(10);      
        
        move(ball, 0, velocityY);
        
        // Makes ball disappear if hits bottom of window and subtracts a life     
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            --lives;
            if (lives > 0)
            {
                removeGWindow(window, ball);
                freeGObject(ball);
                ball = initBall(window);
                goto begin;
            }
            // Ends game displays text
            else if (lives == 0)
            {
                velocityY = 0;
                velocityX = 0;
                initText(window, lives, points, bricks);
            }

        }
        else if (getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        
    GObject object = detectCollision(window, ball);
        
        // Makes ball bounce off paddle
        if (object == paddle)
        {
            velocityY = -velocityY;       
        }
        
        // makes ball bounce off bricks and adds to score
        else if ((object != NULL) && (strcmp(getType(object), "GRect") == 0))
        {
            velocityY = -velocityY;
            ++points;
            removeGWindow(window, object);
            freeGObject(object);
            updateScoreboard(window, label, points);

                if (points == bricks)
                { 
                    initText(window, lives, points, bricks);
                    velocityY = 0;
                    velocityX = 0;
                    break;
                }
        }
        
        pause(10);

    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    
    // Sets where bricks start
    double x = (getWidth(window) - BRICKWIDTH) / 175;
    double y = (getHeight(window) - BRICKHEIGHT) / 10;
    
    // Goes through loop to set all bricks
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x, y, BRICKWIDTH, BRICKHEIGHT);
            x = x + BRICKWIDTH + 5;
            
            // Sets color of bricks for each row
            switch (i)
            {
                case 0:
                    setFilled(brick, true);
                    setColor(brick, "RED");
                    add(window, brick);
                    break;
                case 1:
                    setFilled(brick, true);
                    setColor(brick, "BLUE");
                    add(window, brick);
                    break;
                case 2:
                    setFilled(brick, true);
                    setColor(brick, "GREEN");
                    add(window, brick);
                    break;
                case 3:
                    setFilled(brick, true);
                    setColor(brick, "ORANGE");
                    add(window, brick);
                    break;
                case 4:
                    setFilled(brick, true);
                    setColor(brick, "YELLOW");
                    add(window, brick);
                    break;
            }
        }
        y = y + BRICKHEIGHT + 5;
        x = ((getWidth(window) - BRICKWIDTH) / 175);
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // Sets ball in center of window
    double x = (getWidth(window) - RADIUS) / 2;
    double y = (getHeight(window) - RADIUS) / 2;
    
    // Sets color of ball and ads to window
    GOval ball = newGOval(x, y, RADIUS, RADIUS);
    setFilled(ball, true);
    setColor(ball, "BLUE");
    add(window, ball);
    
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // Create paddle
    
    
    // Set location of paddle
    double x = (getWidth(window) - PADDLEWIDTH) / 2;
    double y = (getHeight(window) - PADDLEHEIGHT) / 1.1;
    GRect paddle = newGRect(x, y, PADDLEWIDTH, PADDLEHEIGHT);
    
    // Set color of paddle
    setFilled(paddle, true);
    setColor(paddle, "BLUE");
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // Set score location
    GLabel label = newGLabel("0");
    setFont(label, "SasSerif-35");
    setColor(label, "Green");
    double x = ((getWidth(window) - getWidth(label)) / 2);
    double y = ((getHeight(window) - getHeight(label))/ 2);
    setLocation(label, x, y);
    add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

// Prints text if user loses or wins
void initText(GWindow window, int lives, int points, int bricks)
{
    // Shows if user wins
    if (points == bricks)
    {
        GLabel text = newGLabel("YOU WON!");
        setFont(text, "SasSerif-45");
        setColor(text, "Blue");
        double x = ((getWidth(window) - getWidth(text)) / 2);
        double y = ((getHeight(window) - getHeight(text))/ 3);
        setLocation(text, x, y);
        add(window, text);
    }
    
    // Shows when user is out of lives
    else if (lives == 0)
    {
        GLabel text = newGLabel("GAME OVER!");
        setFont(text, "SasSerif-45");
        setColor(text, "Red");
        double x = ((getWidth(window) - getWidth(text)) / 2);
        double y = ((getHeight(window) - getHeight(text))/ 3);
        setLocation(text, x, y);
        add(window, text);
    }   
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }
    
    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }
    
    // no collision
    return NULL;
}
