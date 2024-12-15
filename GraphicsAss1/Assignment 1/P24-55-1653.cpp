// Includes
#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <windows.h>

// ANY CONSTANTS AND VARIABLES

#define M_PI 3.14159265358979323846

// Window size
const int windowWidth = 800;
const int windowHeight = 600;

// Player position and dimensions
float playerX = 100.0f;
float playerY = 100.0f;
float playerWidth = 50.0f;
float playerHeight = 50.0f;
float eyeRadius = 2.0f;
float eyeOffsetX = 4.0f;
float eyeOffsetY = 3.0f;

// Checks
bool isJumping = false;
bool isDucking = false;
bool gameStarted = false;
bool isClockActive = true;
bool isGameOver = false;
bool isLowerHit = true;
bool isUpperHit = true;

// Player Variables
float jumpVelocity = 5.0f;
float gravity = 0.2f;
float runAnimationPhase = 0.0f; // Track the animation phase for running
const float legSwing = 10.0f; // Amount to swing legs for running effect
int playerHealth = 5;  // Player's health 
int playerScore = 0;    // Player's score

//groundLevel
float groundLevel = 100.0f;  // The Y coordinate for the ground


// Global variables for tree animation
const int numTrees = 7;       // Number of trees

float treePositions[numTrees]; // Array to store X positions of trees
float treeSpeed = 2.0f;        // Speed at which the trees move

// Timer VARIABLES
float timerDuration = 13.0f; //in seconds
float timerRemaining = timerDuration;


// Frame counter for score incrementing
int frameCounter = 0; // Counter for frames to control score increase
const int scoreIncrementInterval = 5; // Increment score every 5 frames


//Clock Variables
const int CLOCK_RADIUS = 30;
const float CLOCK_WIDTH = CLOCK_RADIUS * 2;
const float CLOCK_HEIGHT = CLOCK_RADIUS;

float clockXPosition = windowWidth;

const float clockSpeed = 5.0f;
float clockRenderTimer = 0.0f;
const float clockRenderInterval = 6.0f;

const int fixedHour = 10;
const int fixedMinute = 10;
const int fixedSecond = 30;

//OBSTACLES VARIABLES
float upperWallXPosition = windowWidth;
float upperWallRenderTimer = 0.0f;
const float upperWallRenderInterval = 4.0f;

float lowerWallXPosition = windowWidth + 800;
float lowerWallRenderTimer = 0.0f;
const float lowerWallRenderInterval = 5.0f;

const float WALL_WIDTH = 100.0f;
const float WALL_Height = 55.0f;

const float wallSpeed = 5.0f;

//Arrow VARS
float upperArrowXPosition = windowWidth + 600.0f; // Starting position of the arrow (off-screen)
float upperArrowRenderTimer = 0.0f;
const float upperArrowRenderInterval = 7.0f;
float upperArrowYPosition = groundLevel + 110.0f;

bool isUpperArrowActive = true;     // Whether the arrow is active
bool isLowerArrowActive = true;     // Whether the arrow is active
float arrowSpeed = 5.0f;       // Speed of the arrow

float lowerArrowXPosition = windowWidth + 400.0f; // Starting position of the arrow (off-screen)
float lowerArrowRenderTimer = 0.0f;
const float lowerArrowRenderInterval = 5.0f;
float lowerArrowYPosition = groundLevel + 50;

bool ArrowJump = true;
float lowerArrowJump = 0.6f;
float upperArrowJump = 2.0f;


const float ARROW_WIDTH = 30.0f;
const float ARROW_HEIGHT = 20.0f;
const float ARROW_STEM_WIDTH = 10.0f;
const float ARROW_STEM_LENGTH = 40.0f;

//game vars
float gameSpeed = 1.0f;           // Initial game speed
float gameSpeedIncrement = 0.005f; // How much the speed increases over time

bool isPowerUpActive = true; // Power-up state
float powerUpXPosition = windowWidth + 1000; // Initial position of the power-up
float powerUpYPosition = 120.0f; // Fixed Y position for simplicity
bool isInvincible = false; // Player invincibility state
float invincibilityDuration = 5.0f; // Duration of invincibility in seconds
float invincibilityTimer = 0.0f; // Timer to track invincibility duration 

const float POWERUP_WIDTH = 20.0f;  // Width of the power-up
const float POWERUP_HEIGHT = 20.0f; // Height of the power-up


bool isScorePowerUpActive = true; // Score power-up state
float scorePowerUpXPosition = windowWidth + 1200; // Initial position of the score power-up
float scorePowerUpYPosition = 120.0f; // Fixed Y position for simplicity

const float SCORE_POWERUP_WIDTH = 20.0f;  // Width of the score power-up
const float SCORE_POWERUP_HEIGHT = 20.0f; // Height of the score power-up


//---------------------------------------------------------------------------------------------------------------------------------------------------------------//

//FUNCTIONS

//Controls Functions

void handleKeyPress(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': // Jump
        if (!isJumping) {
            isJumping = true;
        }
        break;
    case 's': // Duck
        isDucking = true;
        break;
    }
}

void handleKeyRelease(unsigned char key, int x, int y) {
    if (key == 's') {
        isDucking = false;
    }
}

void handleSpecialKeyPress(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        if (!isJumping) {
            isJumping = true;
        }
        break;
    case GLUT_KEY_DOWN:
        isDucking = true;
        break;
    }
}

void handleSpecialKeyRelease(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        isDucking = false;
    }
}

void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Check if the click is within the button area
        if (x > (windowWidth / 2 - 100) && x < (windowWidth / 2 + 100) &&
            y >(windowHeight / 2 - 25) && y < (windowHeight / 2 + 25)) {
            gameStarted = true;
        }
    }
}

void Controls_init() {

    glutKeyboardFunc(handleKeyPress);
    glutKeyboardUpFunc(handleKeyRelease);
    glutSpecialFunc(handleSpecialKeyPress);
    glutSpecialUpFunc(handleSpecialKeyRelease);
    glutMouseFunc(handleMouseClick);

}


void pushAway() {
    lowerArrowXPosition += 200;
    upperArrowXPosition += 200;
    upperWallXPosition += 300;
    lowerWallXPosition += 300;

    for (int i = 0; i < numTrees; i++) {
        treePositions[i] += 300;
    }
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------//

//DRAWING FUNCTIONS

void renderPlayer() {
    // Set color and draw torso (Rectangle)
    glColor3f(0.0f, 0.0f, 1.0f);  // Body color
    glBegin(GL_QUADS);
    glVertex2f(playerX + 15, playerY);                    // Bottom-left
    glVertex2f(playerX + 35, playerY);                    // Bottom-right
    glVertex2f(playerX + 35, playerY + playerHeight);     // Top-right (adjust height when ducking)
    glVertex2f(playerX + 15, playerY + playerHeight);     // Top-left (adjust height when ducking)
    glEnd();

    // Draw head (Circle)
    glColor3f(1.0f, 0.8f, 0.6f);  // Skin tone for the head
    float radius = 12.0f;
    float centerX = playerX + playerWidth / 2.0f;  // Center the head on top of the body
    float centerY = playerY + playerHeight + 10.0f; // Position head slightly above the body
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY);  // Center of the circle
    for (int i = 0; i <= 20; i++) {  // Draw the circle with triangles
        float theta = 2.0f * M_PI * float(i) / 20.0f;  // Get angle for the vertex
        float dx = radius * cosf(theta);  // X component
        float dy = radius * sinf(theta);  // Y component
        glVertex2f(centerX + dx, centerY + dy);
    }
    glEnd();

    // Draw eyes and mouth (same as before)
    glColor3f(0.0f, 0.0f, 0.0f);  // Black for eyes


    // Left eye
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX - eyeOffsetX, centerY + eyeOffsetY);
    for (int i = 0; i <= 20; i++) {
        float theta = 2.0f * M_PI * float(i) / 20.0f;
        float dx = eyeRadius * cosf(theta);
        float dy = eyeRadius * sinf(theta);
        glVertex2f(centerX - eyeOffsetX + dx, centerY + eyeOffsetY + dy);
    }
    glEnd();

    // Right eye
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX + eyeOffsetX, centerY + eyeOffsetY);
    for (int i = 0; i <= 20; i++) {
        float theta = 2.0f * M_PI * float(i) / 20.0f;
        float dx = eyeRadius * cosf(theta);
        float dy = eyeRadius * sinf(theta);
        glVertex2f(centerX + eyeOffsetX + dx, centerY + eyeOffsetY + dy);
    }
    glEnd();

    // Draw mouth
    glColor3f(1.0f, 0.0f, 0.0f);  // Red for mouth
    float mouthWidth = 6.0f;
    float mouthHeight = -2.0f;
    glBegin(GL_LINES);
    glVertex2f(centerX - mouthWidth / 2.0f, centerY - eyeOffsetY);  // Left side of the mouth
    glVertex2f(centerX + mouthWidth / 2.0f, centerY - eyeOffsetY);  // Right side of the mouth
    glEnd();

    // Draw shoulders and arms
    glColor3f(0.0f, 0.0f, 1.0f);  // Skin tone for arms and shoulders

    // Left shoulder (small rectangle near the torso)
    glBegin(GL_QUADS);
    glVertex2f(playerX + 5, playerY + playerHeight); // Top-left of shoulder
    glVertex2f(playerX + 15, playerY + playerHeight); // Top-right of shoulder
    glVertex2f(playerX + 15, playerY + playerHeight - 10); // Bottom-right of shoulder
    glVertex2f(playerX + 5, playerY + playerHeight - 10); // Bottom-left of shoulder
    glEnd();

    //glVertex2f(playerX + 15, playerY + playerHeight);     // Top-left (adjust height when ducking)


    // Right shoulder (small rectangle near the torso)
    glBegin(GL_QUADS);
    glVertex2f(playerX + 35, playerY + playerHeight); // Top-left of shoulder
    glVertex2f(playerX + 45, playerY + playerHeight); // Top-right of shoulder
    glVertex2f(playerX + 45, playerY + playerHeight - 10); // Bottom-right of shoulder
    glVertex2f(playerX + 35, playerY + playerHeight - 10); // Bottom-left of shoulder
    glEnd();

    glColor3f(1.0f, 0.8f, 0.6f);  // Skin tone for arms and shoulders

    // Left arm (rectangle extending from shoulder)
    glBegin(GL_QUADS);
    glVertex2f(playerX + 5, playerY + playerHeight - 10); // top left 
    glVertex2f(playerX + 15, playerY + playerHeight - 10); // top right
    glVertex2f(playerX + 15, playerY + playerHeight * 0.60f - 10); // bottom right
    glVertex2f(playerX + 5, playerY + playerHeight * 0.60f - 10); // bottom left
    glEnd();

    // Right arm (rectangle extending from shoulder)
    glBegin(GL_QUADS);
    glVertex2f(playerX + 35, playerY + playerHeight - 10); // Shoulder point
    glVertex2f(playerX + 45, playerY + playerHeight - 10); // Elbow
    glVertex2f(playerX + 45, playerY + playerHeight * 0.60f - 10); // Elbow extension
    glVertex2f(playerX + 35, playerY + playerHeight * 0.60f - 10); // Hand
    glEnd();

    // Leg animation and drawing (using polygons)
    float legAdjustment = legSwing * sin(runAnimationPhase);

    // Left leg
    glBegin(GL_POLYGON);
    glVertex2f(playerX + 15, playerY);                            // Bottom-left of the leg
    glVertex2f(playerX + 15 + legAdjustment, playerY - 30);      // Top-left of the leg
    glVertex2f(playerX + 22.5 + legAdjustment, playerY - 30); // Top-right of the leg
    glVertex2f(playerX + 22.5, playerY);                       // Bottom-right of the leg
    glEnd();

    // Right leg
    glBegin(GL_POLYGON);
    glVertex2f(playerX + 27.5, playerY);                            // Bottom-left of the leg
    glVertex2f(playerX + 27.5 + legAdjustment, playerY - 30);      // Top-left of the leg
    glVertex2f(playerX + 35 + legAdjustment, playerY - 30); // Top-right of the leg
    glVertex2f(playerX + 35, playerY);                       // Bottom-right of the leg
    glEnd();
}

// Function to initialize tree positions
void initTrees() {
    for (int i = 0; i < numTrees; i++) {
        treePositions[i] = i * 150.0f + 50; // Initial spacing of trees
    }
}
void renderTrees() {
    // Green color for the leaves
    glColor3f(0.2f, 0.8f, 0.2f);

    for (int i = 0; i < numTrees; i++) {
        float treeX = treePositions[i];
        float treeY = groundLevel + 150;  // Base of the tree
        float treeWidth = 10.0f;
        float treeHeight = 80.0f;

        // Draw tree trunk (brown rectangle)
        glColor3f(0.55f, 0.27f, 0.07f);  // Brown color for the trunk
        glBegin(GL_QUADS);
        glVertex2f(treeX - (treeWidth / 2), treeY);                    // Bottom-left corner of the trunk
        glVertex2f(treeX + (treeWidth / 2), treeY);                    // Bottom-right corner of the trunk
        glVertex2f(treeX + (treeWidth / 2), treeY + treeHeight / 2);   // Top-right corner of the trunk
        glVertex2f(treeX - (treeWidth / 2), treeY + treeHeight / 2);   // Top-left corner of the trunk
        glEnd();

        // Draw tree leaves (green triangle)
        glColor3f(0.0f, 0.5f, 0.0f);  // Green color for the leaves
        glBegin(GL_TRIANGLES);
        glVertex2f(treeX - (treeWidth + 5), treeY + treeHeight / 2);         // Bottom-left corner of the leaves
        glVertex2f(treeX + (treeWidth + 5), treeY + treeHeight / 2);         // Bottom-right corner of the leaves
        glVertex2f(treeX, treeY + treeHeight);                          // Top corner of the leaves (adjusted for height)
        glEnd();

    }
}

void renderBackground() {
    // Render the road (the first road)
    glColor3f(0.4f, 0.4f, 0.4f);  // Dark gray color for the road
    glBegin(GL_QUADS);
    glVertex2f(0, 0);               // Bottom-left corner
    glVertex2f(windowWidth, 0);     // Bottom-right corner
    glVertex2f(windowWidth, groundLevel + 150); // Top-right corner
    glVertex2f(0, groundLevel + 150);          // Top-left corner
    glEnd();

    renderTrees();


}

void renderUpperObstacles() {
    //float obstacleHeight = WallHeight; // Wall height is 75% of player's height
    upperWallXPosition -= wallSpeed * gameSpeed;

    if (upperWallXPosition > -WALL_WIDTH) {
        // Set the color for the wall obstacle
        glColor3f(0.7f, 0.7f, 0.7f); // Light gray color for the wall

        // Draw the rectangular wall using GL_QUADS
        glBegin(GL_QUADS);
        glVertex2f(upperWallXPosition, groundLevel + WALL_Height);                          // Bottom-left
        glVertex2f(upperWallXPosition + WALL_WIDTH, groundLevel + WALL_Height);           // Bottom-right
        glVertex2f(upperWallXPosition + WALL_WIDTH, groundLevel + WALL_Height + 40); // Top-right
        glVertex2f(upperWallXPosition, groundLevel + WALL_Height + 40);          // Top-left
        glEnd();

        // Set the color for the "X" on the wall
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for the "X"

        // Draw the "X" using GL_LINES
        glBegin(GL_LINES);
        // Diagonal from top-left to bottom-right
        glVertex2f(upperWallXPosition, groundLevel + WALL_Height + 40);          // Top-left
        glVertex2f(upperWallXPosition + WALL_WIDTH, groundLevel + WALL_Height);           // Bottom-right

        // Diagonal from bottom-left to top-right
        glVertex2f(upperWallXPosition, groundLevel + WALL_Height);                          // Bottom-left
        glVertex2f(upperWallXPosition + WALL_WIDTH, groundLevel + WALL_Height + 40); // Top-right
        glEnd();
    }

}

void renderLowerObstacle() {
    lowerWallXPosition -= wallSpeed * gameSpeed;

    if (lowerWallXPosition > -WALL_WIDTH) {
        // Set the color for the lower wall obstacle
        glColor3f(0.7f, 0.7f, 0.7f); // Light gray color for the wall

        // Draw the rectangular wall using GL_QUADS
        glBegin(GL_QUADS);
        glVertex2f(lowerWallXPosition, groundLevel - 20);                          // Bottom-left
        glVertex2f(lowerWallXPosition + WALL_WIDTH, groundLevel - 20);           // Bottom-right
        glVertex2f(lowerWallXPosition + WALL_WIDTH, groundLevel + 20); // Top-right
        glVertex2f(lowerWallXPosition, groundLevel + 20);          // Top-left
        glEnd();

        // Set the color for the "X" on the wall
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for the "X"

        // Draw the "X" using GL_LINES
        glBegin(GL_LINES);
        // Diagonal from top-left to bottom-right
        glVertex2f(lowerWallXPosition, groundLevel + 20);          // Top-left
        glVertex2f(lowerWallXPosition + WALL_WIDTH, groundLevel - 20);           // Bottom-right

        // Diagonal from bottom-left to top-right
        glVertex2f(lowerWallXPosition, groundLevel - 20);                          // Bottom-left
        glVertex2f(lowerWallXPosition + WALL_WIDTH, groundLevel + 20); // Top-right
        glEnd();
    }
}


//void renderClock() {
//    // Move the clock to the left over time
//    clockXPosition -= clockSpeed * gameSpeed; // Move speed; adjust as needed
//
//    // Only render the clock if it's within the visible screen
//    if (clockXPosition > -CLOCK_RADIUS && isClockActive) {
//        // Draw the clock face with a black border
//        glColor3f(1.0f, 1.0f, 0.7f); // Light yellow color for the clock face
//        glBegin(GL_TRIANGLE_FAN);
//        glVertex2f(clockXPosition, groundLevel); // Center of the clock
//        for (int angle = 0; angle <= 360; angle += 6) { // 60 points for the clock face
//            float radians = angle * M_PI / 180.0f; // Convert angle to radians
//            float x = clockXPosition + CLOCK_RADIUS * cos(radians);
//            float y = groundLevel + CLOCK_RADIUS * sin(radians);
//            glVertex2f(x, y);
//        }
//        glEnd();
//
//        // Draw the clock border
//        glColor3f(0.0f, 0.0f, 0.0f); // Black color for the border
//        glBegin(GL_LINE_LOOP);
//        for (int angle = 0; angle < 360; angle += 6) { // 60 points for the border
//            float radians = angle * M_PI / 180.0f; // Convert angle to radians
//            float x = clockXPosition + CLOCK_RADIUS * cos(radians);
//            float y = groundLevel + CLOCK_RADIUS * sin(radians);
//            glVertex2f(x, y);
//        }
//        glEnd();
//
//        // Calculate angles for hands
//        float hourAngle = (fixedHour % 12 + fixedMinute / 60.0f) * 30; // 30 degrees per hour
//        float minuteAngle = (fixedMinute + fixedSecond / 60.0f) * 6; // 6 degrees per minute
//        float secondAngle = fixedSecond * 6; // 6 degrees per second
//
//        // Draw the hour hand
//        glColor3f(0.0f, 0.0f, 0.0f); // Black color for the hour hand
//        glBegin(GL_LINES);
//        glVertex2f(clockXPosition, groundLevel); // Center of the clock
//        glVertex2f(clockXPosition + (CLOCK_RADIUS - 10) * cos(hourAngle * M_PI / 180.0f),
//            groundLevel + (CLOCK_RADIUS - 10) * sin(hourAngle * M_PI / 180.0f));
//        glEnd();
//
//        // Draw the minute hand
//        glColor3f(0.0f, 1.0f, 0.0f); // Black color for the minute hand
//        glBegin(GL_LINES);
//        glVertex2f(clockXPosition, groundLevel); // Center of the clock
//        glVertex2f(clockXPosition + (CLOCK_RADIUS - 15) * cos(minuteAngle * M_PI / 180.0f),
//            groundLevel + (CLOCK_RADIUS - 15) * sin(minuteAngle * M_PI / 180.0f));
//        glEnd();
//
//        // Draw the second hand
//        glColor3f(1.0f, 0.0f, 0.0f); // Red color for the second hand
//        glBegin(GL_LINES);
//        glVertex2f(clockXPosition, groundLevel); // Center of the clock
//        glVertex2f(clockXPosition + (CLOCK_RADIUS - 20) * cos(secondAngle * M_PI / 180.0f),
//            groundLevel + (CLOCK_RADIUS - 20) * sin(secondAngle * M_PI / 180.0f));
//        glEnd();
//    }
//}

// Function to render bitmap string at given coordinates
void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y); // Set the position for the text
    while (*string) {
        glutBitmapCharacter(font, *string); // Render each character
        string++;
    }
}

void renderHealthBar(float barX, float barY) {
    // Size and gap for the health bar
    float barWidth = 100.0f; // Width of the outer rectangle
    float barHeight = 15.0f; // Height of the outer rectangle
    float gap = 2.0f;        // Gap between inner rectangles
    float quadWidth = (barWidth - 4 * gap) / 5; // Width of each life rectangle
    // Save the current line width
    GLfloat originalLineWidth;
    glGetFloatv(GL_LINE_WIDTH, &originalLineWidth);
    // Draw the outer rectangle using GL_LINES
    glColor3f(0.0f, 0.0f, 0.0f); // White color for the outer rectangle
    glLineWidth(3.0f); // Adjust this value as needed

    glBegin(GL_LINES);

    // Top line
    glVertex2f(barX - 1, barY);
    glVertex2f(barX + barWidth, barY);

    // Bottom line
    glVertex2f(barX - 1, barY - barHeight);
    glVertex2f(barX + barWidth, barY - barHeight);

    // Left line
    glVertex2f(barX - 1, barY);
    glVertex2f(barX - 1, barY - barHeight);

    // Right line
    glVertex2f(barX + barWidth, barY);
    glVertex2f(barX + barWidth, barY - barHeight);
    glEnd();
    // Restore the original line width
    glLineWidth(originalLineWidth);


    // Draw the filled life rectangles inside the outer rectangle using GL_QUADS
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for filled lives
    for (int i = 0; i < playerHealth; i++) {
        glBegin(GL_QUADS);
        glVertex2f(barX - 0.5 + i * (quadWidth + gap), barY);                  // Top-left
        glVertex2f(barX - 0.5 + i * (quadWidth + gap) + quadWidth, barY);      // Top-right
        glVertex2f(barX - 0.5 + i * (quadWidth + gap) + quadWidth, barY - barHeight + 1); // Bottom-right
        glVertex2f(barX - 0.5 + i * (quadWidth + gap), barY - barHeight + 1);      // Bottom-left
        glEnd();
    }
}





// Function to render the start game button
void renderStartButton() {
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for the button
    glBegin(GL_QUADS);
    glVertex2f(windowWidth / 2 - 100, windowHeight / 2 - 25); // Bottom-left
    glVertex2f(windowWidth / 2 + 100, windowHeight / 2 - 25); // Bottom-right
    glVertex2f(windowWidth / 2 + 100, windowHeight / 2 + 25); // Top-right
    glVertex2f(windowWidth / 2 - 100, windowHeight / 2 + 25); // Top-left
    glEnd();

    // Render button text "START GAME"
    glColor3f(1.0f, 1.0f, 1.0f); // White color for text
    float textX = windowWidth / 2 - 70; // Adjust X position for centering
    float textY = windowHeight / 2 - 10; // Adjust Y position for centering
    renderBitmapString(textX, textY, GLUT_BITMAP_HELVETICA_18, "START GAME"); // Render the text
}

void renderLowerArrow() {
    //updateLowerArrow();
    // Move the arrow to the left over time, like the clock
    lowerArrowXPosition -= arrowSpeed * gameSpeed;

    // Only render the arrow if it's within the visible screen and active
    if (lowerArrowXPosition > -ARROW_WIDTH && isLowerArrowActive) {
        // Draw the arrow body
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the arrow
        glBegin(GL_TRIANGLES);
        // Define vertices for the arrow shape (pointing upwards now)
        glVertex2f(lowerArrowXPosition, lowerArrowYPosition + ARROW_HEIGHT);                   // Top point of the arrow
        glVertex2f(lowerArrowXPosition - ARROW_WIDTH / 2, lowerArrowYPosition);                // Bottom-left of the arrow
        glVertex2f(lowerArrowXPosition + ARROW_WIDTH / 2, lowerArrowYPosition);                // Bottom-right of the arrow
        glEnd();

        // Draw the arrow stem (optional, pointing upwards)
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the stem
        glBegin(GL_QUADS);
        glVertex2f(lowerArrowXPosition - ARROW_STEM_WIDTH / 2, lowerArrowYPosition);              // Bottom-left of the stem
        glVertex2f(lowerArrowXPosition + ARROW_STEM_WIDTH / 2, lowerArrowYPosition);              // Bottom-right of the stem
        glVertex2f(lowerArrowXPosition + ARROW_STEM_WIDTH / 2, lowerArrowYPosition - ARROW_STEM_LENGTH); // Top-right of the stem
        glVertex2f(lowerArrowXPosition - ARROW_STEM_WIDTH / 2, lowerArrowYPosition - ARROW_STEM_LENGTH); // Top-left of the stem
        glEnd();
    }
}

void renderUpperArrow() {
    // Move the arrow to the left over time, like the clock
    upperArrowXPosition -= arrowSpeed * gameSpeed;

    // Only render the arrow if it's within the visible screen and active
    if (upperArrowXPosition > -ARROW_WIDTH && isUpperArrowActive) {
        // Draw the arrow body
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the arrow
        glBegin(GL_TRIANGLES);
        // Define vertices for the arrow shape (pointing upwards now)
        glVertex2f(upperArrowXPosition, ARROW_HEIGHT + upperArrowYPosition);                   // Top point of the arrow
        glVertex2f(upperArrowXPosition - ARROW_WIDTH / 2, upperArrowYPosition );                // Bottom-left of the arrow
        glVertex2f(upperArrowXPosition + ARROW_WIDTH / 2, upperArrowYPosition );                // Bottom-right of the arrow
        glEnd();

        // Draw the arrow stem (optional, pointing upwards)
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the stem
        glBegin(GL_QUADS);
        glVertex2f(upperArrowXPosition - ARROW_STEM_WIDTH / 2, upperArrowYPosition);              // Bottom-left of the stem
        glVertex2f(upperArrowXPosition + ARROW_STEM_WIDTH / 2, upperArrowYPosition);              // Bottom-right of the stem
        glVertex2f(upperArrowXPosition + ARROW_STEM_WIDTH / 2, upperArrowYPosition - ARROW_STEM_LENGTH); // Top-right of the stem
        glVertex2f(upperArrowXPosition - ARROW_STEM_WIDTH / 2, upperArrowYPosition - ARROW_STEM_LENGTH); // Top-left of the stem
        glEnd();
    }
}




void renderFrame() {
    // Save the current line width
    GLfloat originalLineWidth;
    glGetFloatv(GL_LINE_WIDTH, &originalLineWidth);

    // Set the color for the frame
    glColor3f(0.2f, 0.2f, 0.5f); // A blueish color for the frame

    // Set the line width to make the lines thicker
    glLineWidth(30.0f); // Adjust this value as needed

    // Draw the upper boundary of the frame
    glBegin(GL_LINES);
    glVertex2f(0, windowHeight); // Top-left corner
    glVertex2f(windowWidth, windowHeight); // Top-right corner
    glEnd();

    // Draw the lower boundary of the frame
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f); // Bottom-left corner
    glVertex2f(windowWidth, 0.0f); // Bottom-right corner
    glVertex2f(windowWidth, 15.0f); // Top-right corner
    glVertex2f(0.0f, 15.0f); // Top-left corner
    glEnd();


    // Restore the original line width
    glLineWidth(originalLineWidth);
}

void renderGamerNameQuad() {
    // Set the color for the gamer name quad (customizable)
    glColor3f(0.9f, 0.1f, 0.4f); // A reddish color for the quad

    // Start drawing a quad (trapezoid shape)
    glBegin(GL_QUADS);

    // Adjusted coordinates for a smaller trapezoid
    glVertex2f(-30.0f, windowHeight); // Top-left corner
    glVertex2f(270.0f, windowHeight); // Top-right corner
    glVertex2f(195.0f, windowHeight - 50); // Bottom-right corner
    glVertex2f(45.0f, windowHeight - 50); // Bottom-left corner

    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // White color for the timer
    float timerX = 60.0f; // X position
    float timerY = windowHeight - 40.0f; // Y position (close to top)


    // Create a string buffer to format the timer display
    char timerString[50];
    sprintf(timerString, "Timer: %.1f", timerRemaining); // Format remaining time

    // Render the timer text
    renderBitmapString(timerX, timerY, GLUT_BITMAP_HELVETICA_18, timerString); // Render the timer text

    // Set the color for the side shape (customizable)
    glColor3f(0.1f, 0.5f, 0.8f); // A bluish color for the side shape

    // Start drawing the side shape (quad-like shape)
    glBegin(GL_QUADS);

    // Define the vertices of the side shape
    glVertex2f(270.0f, windowHeight); // Top-left corner
    glVertex2f(474.0f, windowHeight); // Top-right corner
    glVertex2f(399.0f, windowHeight - 50); // Bottom-right corner
    glVertex2f(195.0f, windowHeight - 50); // Bottom-left corner

    glEnd();

    // Now, draw multiple inclined quads inside the side shape
    glColor3f(0.9f, 0.9f, 0.1f); // Color for inner quads (yellowish)

    // Number of inner quads to draw
    int numQuads = 5;

    // Loop through and draw inclined quads
    for (int i = 0; i < numQuads; ++i) {
        // Calculate the position and size of each quad
        float xStartTop = 244.5f + i * 40.0f; // Top-left corner (shift left for tilt)
        float xEndTop = xStartTop + 35.0f;    // Top-right corner
        float xStartBottom = xStartTop - 30.0f; // Bottom-left corner, slightly to the right
        float xEndBottom = xEndTop - 30.0f;    // Bottom-right corner, slightly to the right

        float yTop = windowHeight - 19.0f;            // Top position remains the same
        float yBottom = windowHeight - 39.0f; // Bottom position (30 units lower)

        // Draw each inclined quad
        glBegin(GL_QUADS);
        glVertex2f(xStartTop, yTop);     // Top-left corner
        glVertex2f(xEndTop, yTop);       // Top-right corner
        glVertex2f(xEndBottom, yBottom); // Bottom-right corner
        glVertex2f(xStartBottom, yBottom); // Bottom-left corner
        glEnd();
    }
}

void renderLowerBound() {
    // Set the color for the gamer name quad (customizable)
    glColor3f(0.9f, 0.1f, 0.4f); // A reddish color for the quad

    // Start drawing a quad (trapezoid shape) in the bottom-right corner
    glBegin(GL_QUADS);
    // Adjusted coordinates for the trapezoid in the bottom-right corner
    glVertex2f(windowWidth - 75 , 50.0f); // Top-right corner
    glVertex2f(windowWidth - 225.0f, 50.0f); // Top-left corner
    glVertex2f(windowWidth - 300.0f, 15.0f); // Bottom-left corner
    glVertex2f(windowWidth, 15.0f); // Bottom-right corner
    glEnd();
    
    // Set the color for the side shape (customizable)
    glColor3f(0.1f, 0.5f, 0.8f); // A bluish color for the side shape

    // Start drawing the side shape (quad-like shape) in the bottom-right corner
    glBegin(GL_QUADS);
    // Define the vertices of the side shape
    glVertex2f(windowWidth - 225.0f, 50.0f); // Top-right corner
    glVertex2f(windowWidth - 429.0f, 50.0f); // Top-left corner
    glVertex2f(windowWidth - 504.0f, 15.0f); // Bottom-left corner
    glVertex2f(windowWidth - 300.0f, 15.0f); // Bottom-right corner
    glEnd();

    // Now, draw multiple inclined quads inside the side shape
    glColor3f(0.9f, 0.9f, 0.1f); // Color for inner quads (yellowish)

    // Number of inner quads to draw
    int numQuads = 5;

    // Loop through and draw inclined quads
// Loop through and draw inclined quads
    for (int i = 0; i < numQuads; ++i) {
        // Calculate the position and size of each quad
        float xStartTop = windowWidth - 289.5f - i * 40.0f; // Top-left corner
        float xEndTop = xStartTop + 35.0f; // Top-right corner (shift right for right tilt)

        float xStartBottom = xStartTop - 38.0f; // Bottom-left corner (move left for tilt)
        float xEndBottom = xEndTop - 38.0f; // Bottom-right corner (move left for tilt)

        // Move the Y positions up by 20 units
        float yTop = 40.0f;    // Y position for the top of the quad (30 + 20)
        float yBottom = 20.0f; // Y position for the bottom of the quad (10 + 20)

        // Draw each inclined quad
        glBegin(GL_QUADS);
        glVertex2f(xStartTop, yTop);     // Top-left corner
        glVertex2f(xEndTop, yTop);       // Top-right corner
        glVertex2f(xEndBottom, yBottom); // Bottom-right corner
        glVertex2f(xStartBottom, yBottom); // Bottom-left corner
        glEnd();
    }


}


void renderTimer() {
    // Set the color and position for the timer text

    glColor3f(1.0f, 1.0f, 1.0f); // White color for the timer
    float timerX = 50.0f; // X position
    float timerY = windowHeight - 40.0f; // Y position (close to top)

    //// Create a string buffer to format the timer display
    //char timerString[50];
    //sprintf(timerString, "Timer: %.1f", timerRemaining); // Format remaining time

    //// Render the timer text
    //renderBitmapString(timerX, timerY, GLUT_BITMAP_HELVETICA_18, timerString); // Render the timer text

    // Render health text
    float healthTextX = windowWidth - 300; // X position for "Health:" text
    float healthTextY = timerY;            // Y position stays the same

    char healthString[50];
    sprintf(healthString, "Health:"); // Format health display
    renderBitmapString(healthTextX, healthTextY, GLUT_BITMAP_HELVETICA_18, healthString); // Render health text

    // Render the health bar beside the health text
    float healthBarX = healthTextX + 65.0f; // Set X position for the health bar beside "Health:"
    float healthBarY = healthTextY + 15.0f;  // Align the health bar with the health text (adjusted Y)
    renderHealthBar(healthBarX, healthBarY); // Render the health bar at the specified position

    glColor3f(1.0f, 1.0f, 1.0f);
    // Render score text
    float scoreTextX = windowWidth - 125;  // X position for score text
    float scoreTextY = timerY;             // Y position stays the same

    char scoreString[50];
    sprintf(scoreString, "Score: %d", playerScore); // Format score display
    renderBitmapString(scoreTextX, scoreTextY, GLUT_BITMAP_HELVETICA_18, scoreString); // Render score text
}


void drawPowerUp() {
    if (isPowerUpActive) {
        // Draw the base (rectangle) for the power-up
        glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 0.0f); // Green color for the power-up
        glVertex2f(powerUpXPosition - POWERUP_WIDTH / 2, powerUpYPosition - POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition + POWERUP_WIDTH / 2, powerUpYPosition - POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition + POWERUP_WIDTH / 2, powerUpYPosition + POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition - POWERUP_WIDTH / 2, powerUpYPosition + POWERUP_HEIGHT / 2);
        glEnd();

        // Draw an effect (triangle) at the top
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for the triangle effect
        glVertex2f(powerUpXPosition, powerUpYPosition + POWERUP_HEIGHT / 2 + 10); // Top point
        glVertex2f(powerUpXPosition - 10.0f, powerUpYPosition + POWERUP_HEIGHT / 2); // Bottom-left
        glVertex2f(powerUpXPosition + 10.0f, powerUpYPosition + POWERUP_HEIGHT / 2); // Bottom-right
        glEnd();

        // Draw lines for an outline effect (using GL_LINES)
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for outlines
        glVertex2f(powerUpXPosition - POWERUP_WIDTH / 2, powerUpYPosition - POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition + POWERUP_WIDTH / 2, powerUpYPosition - POWERUP_HEIGHT / 2);

        glVertex2f(powerUpXPosition + POWERUP_WIDTH / 2, powerUpYPosition - POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition + POWERUP_WIDTH / 2, powerUpYPosition + POWERUP_HEIGHT / 2);

        glVertex2f(powerUpXPosition + POWERUP_WIDTH / 2, powerUpYPosition + POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition - POWERUP_WIDTH / 2, powerUpYPosition + POWERUP_HEIGHT / 2);

        glVertex2f(powerUpXPosition - POWERUP_WIDTH / 2, powerUpYPosition + POWERUP_HEIGHT / 2);
        glVertex2f(powerUpXPosition - POWERUP_WIDTH / 2, powerUpYPosition - POWERUP_HEIGHT / 2);
        glEnd();

        // Draw a point at the center for additional detail (using GL_POINTS)
        glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the center point
        glVertex2f(powerUpXPosition, powerUpYPosition); // Center point
        glEnd();
    }
}


void drawScorePowerUp() {
    if (isScorePowerUpActive) {
        // 1st Primitive: Draw the base (rectangle) for the score power-up
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.5f, 1.0f); // Light blue for the base
        glVertex2f(scorePowerUpXPosition - SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition - SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition + SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition - SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition + SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition - SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2);
        glEnd();

        // 2nd Primitive: Draw a triangle effect on top of the power-up
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the triangle effect
        glVertex2f(scorePowerUpXPosition, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2 + 10); // Top point
        glVertex2f(scorePowerUpXPosition - 10.0f, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2); // Bottom-left
        glVertex2f(scorePowerUpXPosition + 10.0f, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2); // Bottom-right
        glEnd();

        // 3rd Primitive: Draw an additional circle in the center (using GL_TRIANGLE_FAN for approximation)
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f, 1.0f, 0.0f); // Green color for the circle
        glVertex2f(scorePowerUpXPosition, scorePowerUpYPosition); // Center of the circle
        for (int i = 0; i <= 20; i++) {
            float angle = i * (2.0f * 3.14159f / 20); // Dividing the circle into segments
            glVertex2f(scorePowerUpXPosition + cos(angle) * 5.0f, scorePowerUpYPosition + sin(angle) * 5.0f);
        }
        glEnd();

        // 4th Primitive: Draw lines for an outline effect
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for outlines
        glVertex2f(scorePowerUpXPosition - SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition - SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition + SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition - SCORE_POWERUP_HEIGHT / 2);

        glVertex2f(scorePowerUpXPosition + SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition - SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition + SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2);

        glVertex2f(scorePowerUpXPosition + SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition - SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2);

        glVertex2f(scorePowerUpXPosition - SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition + SCORE_POWERUP_HEIGHT / 2);
        glVertex2f(scorePowerUpXPosition - SCORE_POWERUP_WIDTH / 2, scorePowerUpYPosition - SCORE_POWERUP_HEIGHT / 2);
        glEnd();
    }
}




//-----------------------------------------------------------------------------------------------------------------------------//

//CHECK FUNCS

bool checkCollisions(const std::string& objectType) {
    if (objectType == "clock") {
        if (!isClockActive) {
            return false;
        }

        // Player bounding box
        float playerLeft = playerX + 15;
        float playerRight = playerX + 35;
        float playerBottom = playerY - 30;
        float playerTop = playerY + playerHeight + 10;

        // Clock bounding box
        float clockLeft = clockXPosition - CLOCK_WIDTH / 2;
        float clockRight = clockXPosition + CLOCK_WIDTH / 2;
        float clockBottom = groundLevel;
        float clockTop = groundLevel + CLOCK_HEIGHT;

        // Check for collision with clock
        return (playerRight >= clockLeft && playerLeft <= clockRight &&
            playerTop >= clockBottom && playerBottom <= clockTop);
    }
    if (objectType == "upperObstacle") {
        if (!isUpperHit) {
            return false;
        }
        // Player bounding box
        float playerLeft = playerX + 15;
        float playerRight = playerX + 35;
        float playerBottom = playerY - 30;
        float playerTop = playerY + playerHeight + 10;

        float wallLeft = upperWallXPosition;
        float wallRight = upperWallXPosition + WALL_WIDTH;
        float wallBottom = groundLevel + WALL_Height;
        float wallTop = groundLevel + WALL_Height + 40;

        return (playerRight >= wallLeft && playerLeft <= wallRight &&
            playerTop >= wallBottom && playerBottom <= wallTop);
    }
    if (objectType == "lowerObstacle") {
        if (!isLowerHit) {
            return false;
        }
        // Player bounding box
        float playerLeft = playerX + 15;
        float playerRight = playerX + 35;
        float playerBottom = playerY - 30;
        float playerTop = playerY + playerHeight + 10;

        // Lower obstacle collision detection
        float lowerWallLeft = lowerWallXPosition;
        float lowerWallRight = lowerWallXPosition + WALL_WIDTH;
        float lowerWallBottom = groundLevel - 20;
        float lowerWallTop = groundLevel + 20;


        return(playerRight >= lowerWallLeft && playerLeft <= lowerWallRight &&
            playerTop >= lowerWallBottom && playerBottom <= lowerWallTop);

    }
    if (objectType == "upperArrow") {
        if (!isUpperArrowActive) {
            return false;
        }

        // Player bounding box
        float playerLeft = playerX + 15;
        float playerRight = playerX + 35;
        float playerBottom = playerY - 30;
        float playerTop = playerY + playerHeight + 10;

        // Arrow bounding box
        float arrowLeft = upperArrowXPosition - ARROW_WIDTH / 2;
        float arrowRight = upperArrowXPosition + ARROW_WIDTH / 2;
        float arrowBottom = upperArrowYPosition - ARROW_STEM_LENGTH;
        float arrowTop = upperArrowYPosition + ARROW_HEIGHT + ARROW_STEM_LENGTH;

        // Check for collision with arrow
        return (playerRight >= arrowLeft && playerLeft <= arrowRight &&
            playerTop >= arrowBottom && playerBottom <= arrowTop);
    }
    if (objectType == "lowerArrow") {
        if (!isLowerArrowActive) {
            return false;
        }

        // Player bounding box
        float playerLeft = playerX + 15;
        float playerRight = playerX + 35;
        float playerBottom = playerY - 30;
        float playerTop = playerY + playerHeight + 10;

        // Arrow bounding box
        float arrowLeft = lowerArrowXPosition - ARROW_WIDTH / 2;
        float arrowRight = lowerArrowXPosition + ARROW_WIDTH / 2;
        float arrowBottom = lowerArrowYPosition - ARROW_STEM_LENGTH;
        float arrowTop = lowerArrowYPosition + ARROW_HEIGHT + ARROW_STEM_LENGTH;

        // Check for collision with arrow
        return (playerRight >= arrowLeft && playerLeft <= arrowRight &&
            playerTop >= arrowBottom && playerBottom <= arrowTop);
    }
    return false; // Default to no collision if objectType doesn't match
}

bool checkScorePowerUpCollision(float playerX, float playerY, float playerRadius) {
    // Check if the score power-up is active
    if (!isScorePowerUpActive) {
        return false; // No collision if score power-up is not active
    }

    // Define the score power-up bounding box
    float scorePowerUpLeft = scorePowerUpXPosition - (SCORE_POWERUP_WIDTH / 2);
    float scorePowerUpRight = scorePowerUpXPosition + (SCORE_POWERUP_WIDTH / 2);
    float scorePowerUpBottom = scorePowerUpYPosition - (SCORE_POWERUP_HEIGHT / 2);
    float scorePowerUpTop = scorePowerUpYPosition + (SCORE_POWERUP_HEIGHT / 2);

    // Player bounding box
    float playerLeft = playerX;
    float playerRight = playerX + playerRadius;
    float playerBottom = groundLevel;
    float playerTop = groundLevel + playerHeight;

    // Check for collision with the score power-up
    return (playerRight > scorePowerUpLeft && playerLeft < scorePowerUpRight &&
        playerTop > scorePowerUpBottom && playerBottom < scorePowerUpTop);
}


//-----------------------------------------------------------------------------------------------------------------------------//

//UPDATES FUNCTIONS

void updateClock() {
    if (isGameOver) {
        return;
    }
    // Update clock rendering timer
    clockRenderTimer += 0.016f; // Increment by time passed
    if (clockRenderTimer >= clockRenderInterval) {
        isClockActive = true;
        clockRenderTimer = 0; // Reset the timer
        clockXPosition = windowWidth; // Reset clock position to right edge
    }
}

void updateUpperObstacles() {
    if (isGameOver) {
        return;
    }

    upperWallRenderTimer += 0.016f;
    if (upperWallRenderTimer >= upperWallRenderInterval) {
        isUpperHit = true;
        upperWallRenderTimer = 0;
        upperWallXPosition = windowWidth;
    }

    if (checkCollisions("upperObstacle")) {
        isUpperHit = false;
        if (isInvincible) {
            isInvincible = false;
            return;
        }
        else {
            playerHealth -= 1;
            return;
        }
        pushAway();
        //updateUpperObstacles();
    }
}

void updateLowerObstacles() {
    if (isGameOver) {
        return;
    }

    lowerWallRenderTimer += 0.016f;
    if (lowerWallRenderTimer >= lowerWallRenderInterval) {
        isLowerHit = true;
        lowerWallRenderTimer = 0;
        lowerWallXPosition = windowWidth + 600;
    }

    if (checkCollisions("lowerObstacle")) {
        isLowerHit = false;
        if (isInvincible) {
            isInvincible = false;
            return;
        }
        else {
            playerHealth -= 1;
            return;
        }
        pushAway();
        //updateLowerObstacles();
    }
}

void updateLowerArrow() {
    if (isGameOver) {
        return;
    }
    if (lowerArrowJump) {
        lowerArrowYPosition += lowerArrowJump;
        lowerArrowJump -= gravity;

        if (lowerArrowYPosition <= groundLevel + 50) {
            lowerArrowYPosition = groundLevel + 50;
            lowerArrowJump = 2.0f;
        }
    }

    lowerArrowRenderTimer += 0.016f;
    if (lowerArrowRenderTimer >= lowerArrowRenderInterval) {
        isLowerArrowActive = true;
        lowerArrowRenderTimer = 0;
        lowerArrowXPosition = windowWidth + 400;
    }
}



void updateUpperArrow() {
    if (isGameOver) {
        return;
    }

    if (upperArrowJump) {
        upperArrowYPosition += upperArrowJump;
        upperArrowJump -= gravity;

        if (upperArrowYPosition <= groundLevel + 110) {
            upperArrowYPosition = groundLevel + 110;
            upperArrowJump = 2.0f;
        }
    }

    upperArrowRenderTimer += 0.016f;
    if (upperArrowRenderTimer >= upperArrowRenderInterval) {
        isUpperArrowActive = true;
        upperArrowRenderTimer = 0;
        upperArrowXPosition = windowWidth + 600;
    }
}

void updateScorePowerUp() {
    if (isScorePowerUpActive) {
        // Move the score power-up to the left
        scorePowerUpXPosition -= 5.0f;

        // Check if it goes off-screen (can reset position if needed)
        if (scorePowerUpXPosition < -SCORE_POWERUP_WIDTH) {
            scorePowerUpXPosition = windowWidth + 1000; // Reset to the right
        }
    }

    // Check for collision with the player
    if (checkScorePowerUpCollision(playerX, playerY, playerWidth)) {
        isScorePowerUpActive = false; // Deactivate score power-up
        playerScore += 1000; // Increase score by 1000
    }
}


void updatePlayer() {
    if (isGameOver) {
        return; // Stop updating the player if the game is over
    }

    if (isJumping) {
        playerY += jumpVelocity;
        jumpVelocity -= gravity;
        if (playerY <= groundLevel) {
            playerY = groundLevel;
            jumpVelocity = 5.0f;
            isJumping = false;
        }
    }

    // Handle ducking logic
    if (isDucking) {
        playerHeight = 30.0f;  // Ducking makes the player shorter
    }
    else {
        playerHeight = 50.0f;  // Reset height when not ducking
    }

    // Update run animation phase for leg movement
    runAnimationPhase += 0.1f; // Increment the phase
    if (runAnimationPhase > 2.0f * M_PI) { // Reset after a full cycle
        runAnimationPhase -= 2.0f * M_PI;
    }

    // Increase the score over time if the game is ongoing
    if (gameStarted && !isGameOver) {
        frameCounter++; // Increment frame counter
        if (frameCounter >= scoreIncrementInterval) {
            //playerScore += 1; // Increment score
            frameCounter = 0; // Reset counter
        }

        //// Check for collision with the clock
        //if (checkCollisions("clock")) {
        //    //clockXPosition = windowWidth; // Reset clock position to the right edge
        //    isClockActive = false;
        //    timerRemaining += 5.0f; // Increase timer by 5 seconds
        //    clockXPosition = windowWidth; // Reset clock position to the right edge
        //    updateClock();
        //}

        if (checkCollisions("lowerArrow")) {
            isLowerArrowActive = false; // Deactivate the arrow after collection
            playerScore += 100;          // Increase the score by 500
        }
        if (checkCollisions("upperArrow")) {
            isUpperArrowActive = false; // Deactivate the arrow after collection
            playerScore += 100;          // Increase the score by 500
        }
        

    }
}

void updateTrees() {
    if (isGameOver) {
        return; // Stop updating trees if the game is over
    }

    for (int i = 0; i < numTrees; i++) {
        // Move each tree left by decreasing the X position
        treePositions[i] -= treeSpeed * gameSpeed;

        // Reset the tree's position if it moves off-screen
        if (treePositions[i] < -50) {
            treePositions[i] = windowWidth + 50;
        }
    }
}

bool checkPowerUpCollision(float playerX, float playerY, float playerRadius) {
    // Check if the power-up is active
    if (!isPowerUpActive) {
        return false; // No collision if power-up is not active
    }

    // Define the power-up bounding box
    float powerUpLeft = powerUpXPosition - (POWERUP_WIDTH / 2);
    float powerUpRight = powerUpXPosition + (POWERUP_WIDTH / 2);
    float powerUpBottom = powerUpYPosition - (POWERUP_HEIGHT / 2);
    float powerUpTop = powerUpYPosition + (POWERUP_HEIGHT / 2);

    // Player bounding box
    float playerLeft = playerX;   // Adjusted for player's radius
    float playerRight = playerX + playerRadius;
    float playerBottom = groundLevel;
    float playerTop = groundLevel + playerHeight;

    // Check for collision with the power-up
    return (playerRight > powerUpLeft && playerLeft < powerUpRight &&
        playerTop > powerUpBottom && playerBottom < powerUpTop);
}



void updatePowerUp() {
    if (isPowerUpActive) {
        // Move the power-up to the left
        powerUpXPosition -= 5.0f;

        // Check if it goes off-screen
        //if (powerUpXPosition < -5.0f) {
            // Reset position or spawn a new one if needed
          //  powerUpXPosition = windowWidth; // Reset to the right
        //}
    }

    // Check for collision with the player
    if (checkPowerUpCollision(playerX, playerY, playerWidth)) {
        isPowerUpActive = false; // Deactivate power-up
        isInvincible = true; // Activate invincibility
        invincibilityTimer = 0.0f; // Reset timer
    }
}


void Animations(int value) {
    if (gameStarted && !isGameOver) {
        updatePlayer();            // Update player position only if game has started
        updateTrees();             // Update trees
        //updateClock();
        updateLowerArrow();
        updateUpperArrow();
        updateUpperObstacles();
        updateLowerObstacles();
        updatePowerUp();
        updateScorePowerUp();
        // Update timer
        timerRemaining -= 0.016f; // Decrease timer by approximately 1/60 of a second
        if (timerRemaining <= 0) {
            timerRemaining = 0; // Prevent negative timer
            isGameOver = true;  // Set game over state
        }

        // Increase game speed gradually
        gameSpeed += gameSpeedIncrement; // Gradually increase speed

        if (playerHealth == 0) {
            isGameOver = true;
        }

    }
    glutPostRedisplay();           // Redraw the scene
    glutTimerFunc(16, Animations, 0);  // Call this function again after 16 ms
}

//--------------------------------------------------------------------------------------------------------------------------------//

//Rendering the final Scene

// Function to render the scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    renderBackground();            // Render the background
    renderTimer();                 // Render the timer 
    renderGamerNameQuad();  // Render the lower boundary
    renderFrame();
    renderLowerBound();
    //renderSideShapeRight();
    if (gameStarted) {
        //updatePlayer();            // Update player position
        //updateTrees();             // Update trees
        renderPlayer();            // Render the player
        //renderClock();
        renderLowerArrow();
        renderUpperArrow();
        renderUpperObstacles();
        renderLowerObstacle();
        drawPowerUp();
        drawScorePowerUp();

    }
    else {
        renderStartButton();        // Render the start button
    }

    // Check if the game is over and render the message
    if (isGameOver && timerRemaining == 0) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for game over text
        renderBitmapString(windowWidth / 2 - 60, windowHeight / 2, GLUT_BITMAP_HELVETICA_18, "YOU WON!"); // Display game over message
    }else if (isGameOver){
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for game over text
        renderBitmapString(windowWidth / 2 - 60, windowHeight / 2, GLUT_BITMAP_HELVETICA_18, "GAME OVER!"); // Display game over message
    }

    glutSwapBuffers();             // Swap buffers for double buffering
}

//---------------------------------------------------------------------------------------------------------------------------------------------//

//Main FUNC

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double buffering and RGB color
    glutInitWindowSize(windowWidth, windowHeight);

    //Center the window in the screen
    int screenWidth, screenHeight;

    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int windowX = (screenWidth - windowWidth) / 2;
    int windowY = (screenHeight - windowHeight) / 2;

    glutCreateWindow("2D Infinite Runner Game");
    glutPositionWindow(windowX, windowY);


    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);  // Light blue


    // Set up OpenGL projection
    glOrtho(0.0, windowWidth, 0.0, windowHeight, -1.0, 1.0); // Set orthographic projection



    //Init funcs
    Controls_init();

    // Initialize trees
    initTrees();

    // Register callbacks
    glutDisplayFunc(renderScene);         // Render scene callback
    glutTimerFunc(0, Animations, 0);           // Start timer for updates

    // Start the main loop
    glutMainLoop();
    return 0;
}

