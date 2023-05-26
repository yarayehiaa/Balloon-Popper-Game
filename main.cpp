#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <stdio.h>
#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define MAX_BALLOONS 20
#define MIN_BALLOON_SIZE 20   // Minimum balloon size
#define MAX_BALLOON_SIZE 25

const int BATCH_SIZE = 5;
const int SPAWN_DELAY = 5000;
int phyWidth= 500;
int phyHeight= 500;
int logWidth=100;
int logHeight=100;
int MAX_SPEED=6;
int MIN_SPEED=3;
const int basketHeight = 20;
float basketWidth = 80.0;  // Width of the basket
float basketX = phyWidth / 2 - basketWidth / 2; // X-coordinate of the basket
int basketY = phyHeight - basketHeight;
float balloons[MAX_BALLOONS][5];
int score = 0;
char scoreString[11]; // +7 for "Score: " and null terminator
bool gameRunning = true;
const int GAME_DURATION = 20;
float totalTime=0;

// Function to get the elapsed time in seconds



void drawCircle(int x, int y, int radius) {
    int numSegments = 100;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= numSegments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}




void intToString(int number, const char* buffer, int bufferSize) {
    snprintf(const_cast<char*>(buffer), bufferSize, "Score: %d", number);
}

void gametimer(int value) {
    if (value == 0) {
        // Time's up, end the game
        gameRunning = false;
        glutPostRedisplay(); // Trigger display function to switch to game over screen
    }
}
void restartGame() {
    score = 0;
    gameRunning = true;
    glutTimerFunc(GAME_DURATION * 1000, gametimer, 0); // Start the timer again
        }
void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            basketX -= 10.0;
             // Move the basket left by 10 units
            break;
        case GLUT_KEY_RIGHT:
            basketX += 10.0;  // Move the basket right by 10 units
            break;

        case GLUT_KEY_F1:
            printf("yes");
            restartGame();
    }
}


void drawBalloons() {
    for (int i = 0; i < 20; i++) {
        if (balloons[i][3] != 0) {
            // Balloon has not been popped
            if (balloons[i][3] == 1) {
                // Red balloon
                glColor3f(1.0, 0.0, 0.0); // Set color to red
            } else if (balloons[i][3] == 2) {
                // Yellow balloon
                glColor3f(1.0, 1.0, 0.0); // Set color to yellow
            }

            // Draw balloon
            glBegin(GL_POLYGON);
            for (int j = 0; j < 360; j++) {
                float theta = 2.0 * 3.1415926 * float(j) / float(360);
                float x = balloons[i][0] + balloons[i][2] * cos(theta);
                float y = balloons[i][1] + balloons[i][2] * sin(theta);
                glVertex2f(x, y);
            }
            glEnd();
        }
    }
}

void updateBalloons() {
    totalTime += 1.0/46.0;

    for (int i = 0; i < 20; i++) {
             balloons[i][1]+=balloons[i][4]*2;
        if (balloons[i][1] >= 500 && gameRunning) //reached the top
            {
                score-=1;
                balloons[i][4] *= -1;

            }

       if (balloons[i][1] <= 15 && balloons[i][0] + balloons[i][2] >= basketX && balloons[i][0] <= basketX + basketWidth && balloons[i][2] != 0&& gameRunning ) {
            balloons[i][2] = 0;
            score += 1;


        }
    }
}



void initBalloons(int value) {
    for (int i = 0; i < BATCH_SIZE; i++) {
        int balloonSize=rand() % (MAX_BALLOON_SIZE - MIN_BALLOON_SIZE + 1);
        balloons[i][0] = rand() % (phyWidth - balloonSize); // X-coordinate
        balloons[i][1] = 0; // Y-coordinate (start at the bottom)
        balloons[i][2] = MIN_BALLOON_SIZE +balloonSize; // Size
        balloons[i][3] = rand() % 2 + 1; // Color (1 for red, 2 for yellow)
        balloons[i][4] = rand() % (MAX_SPEED - MIN_SPEED + 1) + MIN_SPEED;


    }
    glutPostRedisplay(); // Trigger a redraw

    // Reschedule the timer for the next batch
    glutTimerFunc(SPAWN_DELAY, initBalloons, 0);
}

void printSome(const char *str,int x,int y) {
    glColor3f (1.0, 1.0, 1.0);
    glRasterPos2d(x,y);
    for (int i=0;i<strlen(str);i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    }
    glFlush();
}


void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    initBalloons(0);
}
void drawBasket(){


    glColor3f(0.0, 0.0, 1.0);  // Set the color to blue
    glBegin(GL_QUADS);
    glVertex2f(basketX, 0.0);
    glVertex2f(basketX + basketWidth, 0.0);
    glVertex2f(basketX + basketWidth+10, 20.0);
    glVertex2f(basketX-10, 20.0);
    glEnd();

}

void displayGame() {
    glClear(GL_COLOR_BUFFER_BIT);
     // Draw the sky background
    glBegin(GL_QUADS);
    glColor3f(0.53, 0.81, 0.92); // Light blue color for sky
    glVertex2f(0, 0);
    glVertex2f(phyWidth, 0);
    glColor3f(0.53, 0.81, 0.92); // Light blue color for sky
    glVertex2f(phyWidth, phyHeight);
    glVertex2f(0, phyHeight);
    glEnd();

    // Draw the grass
    glBegin(GL_QUADS);
    glColor3f(0.45, 0.78, 0.35); // Green color for grass
    glVertex2f(0, 0);
    glVertex2f(phyWidth, 0);
    glColor3f(0.45, 0.78, 0.35); // Green color for grass
    glVertex2f(phyWidth, 15);
    glVertex2f(0, 15);
    glEnd();

    // Draw the clouds
    glColor3f(1.0, 1.0, 1.0); // White color for clouds

    // Draw the first cloud
    drawCircle(100, phyHeight - 100, 40);
    drawCircle(140, phyHeight - 100, 40);
    drawCircle(180, phyHeight - 100, 40);

    // Draw the second cloud
    drawCircle(300, phyHeight - 150, 50);
    drawCircle(350, phyHeight - 150, 50);
    drawCircle(400, phyHeight - 150, 50);

    glColor3f(1.0, 0.85, 0.0); // Yellowish-orange color for sun
    drawCircle(phyWidth - 25, phyHeight - 25, 60);

    glColor3f(1.0, 1.0, 0.0); // Yellow color for sun rays

    int numRays = 12; // Number of sun rays
    float radius = 60.0; // Radius of the sun

    for (int i = 0; i < numRays; i++) {
        float angle = i * (360.0 / numRays);
        float startX = phyWidth - 25;
        float startY = phyHeight - 25;
        float endX = startX + (radius + 10) * cosf(angle * M_PI / 180.0);
        float endY = startY + (radius + 10) * sinf(angle * M_PI / 180.0);

        glBegin(GL_TRIANGLES);
        glVertex2f(startX, startY);
        glVertex2f(startX + (radius + 20) * cosf((angle - 10) * M_PI / 180.0), startY + (radius + 20) * sinf((angle - 10) * M_PI / 180.0));
        glVertex2f(endX, endY);
        glEnd();
    }
    glRasterPos2f(10, 10); // Set the position to display the score
    glColor3f(1.0, 1.0, 1.0); // Set the color for the score text
    intToString(score, scoreString, sizeof(scoreString));
    for (size_t i = 0; i < strlen(scoreString); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreString[i]);
    }

    char timeString[10];
    snprintf(timeString, sizeof(timeString), "Time: %.2f seconds", totalTime);

    // Render the time on the screen
    glRasterPos2f(400, 10);  // Set the position to display the time
    glColor3f(1.0f, 1.0f, 1.0f);  // Set the text color to white
    for (size_t i = 0; i < strlen(timeString); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, timeString[i]);
    }

    glColor3f(1.0, 1.0, 1.0);
    drawBalloons();
    drawBasket();
    glFlush();

}
void displayGameOverScreen() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 1.0, 0.0, 1.0); // Set green background color

    // Draw the game over screen

    // Draw the score
    glRasterPos2f(200, 250); // Set the position to display the score
    glColor3f(1.0, 1.0, 1.0); // Set the color for the score text

    intToString(score, scoreString, sizeof(scoreString));
    for (size_t i = 0; i < strlen(scoreString); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreString[i]);
    }
    printSome("Time's up!",200,350);
    printSome("Press F1 to restart the game",140,150);

    glutSwapBuffers();
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            int i;
            y=500-y;



        for (i = 0; i < MAX_BALLOONS; i++) {

            if (balloons[i][2] > 0) {
                float dx = x - balloons[i][0];
                float dy = y - balloons[i][1];
                float distance = sqrt(dx*dx + dy*dy);
                if (distance < balloons[i][2] && balloons[i][4]>0) {
                    balloons[i][2] = 0;
                     // Pop the balloon
                    if (balloons[i][3] == 1) {
                        score += 1;
                        printf("%d \t",score); // Increment score for red balloon
                    } else if (balloons[i][3] == 2) {
                        score += 2;
                        printf("%d \t",score); // Increment score for yellow balloon
                    }
                    printf("%d \t",score);
                    break;
                }
            }
        }
        glutPostRedisplay();
    }
}

void reshape(int x,int y) {
    // Keep the window size fixed
glutReshapeWindow(500, 500);
}
void timer(int value) {
    updateBalloons(); // Call updateBalloons to update balloon positions
    glutPostRedisplay(); // Trigger a redisplay of the scene
    // Set the next timer callback
    glutTimerFunc(16, timer, 0); // Adjust the timer interval as needed (e.g., 16 milliseconds for approximately 60 FPS)
}
void display() {
    if (gameRunning) {
        displayGame();
    } else {
        displayGameOverScreen();
    }
    glFlush();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Balloon Popping Game");
    init();
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(SPAWN_DELAY, initBalloons, 0);
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(GAME_DURATION * 1000, gametimer, 0); // Start the timer
    glutDisplayFunc(display);
    glutMainLoop();
}
