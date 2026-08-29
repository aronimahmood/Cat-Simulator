#include <windows.h>
#include <GL/glut.h>//EH
#include<GL/glu.h>
#include <cmath>
#include <mmsystem.h>  // For playing sounds
#pragma comment(lib, "winmm.lib")  // Link the library


// Sparkle effect variables*eh
const int NUM_SPARKLES = 50;
float sparkleX[NUM_SPARKLES], sparkleY[NUM_SPARKLES];
float sparkleSize[NUM_SPARKLES];
float sparkleAlpha[NUM_SPARKLES];
bool sparkleActive[NUM_SPARKLES];
float sparkleSpeed = 0.8f;
//eh


float djColor1[3] = {0.8f, 0.2f, 0.2f};  // Red//EH***
float djColor2[3] = {0.2f, 0.2f, 0.8f};  // Blue/EH***
float ballX = -0.5f; // Ball's initial x position
float ballY = -0.3f; // Ball's initial y position
float ballRadius = 0.1f;
float ballSpeedX = 0.0f; // Speed of the ball along the x-axis
float ballSpeedY = 0.0f; // Speed of the ball along the y-axis
float gravity = -0.0005f; // Gravity effect on the ball
float bounceFactor = 0.7f; // Reduces speed after bouncing
float snoreTime = 0.0f; // Controls the snoring animation
float breathScale = 1.0f; // Controls the breathing motion
float breathTime = 0.0f;  // Time variable for breathing effect
bool isBlinking = false;  // Track if the cat is currently blinking
float blinkTimer = 0.0f;  // Timer for when the next blink should happen
float blinkDuration = 0.0f; // Timer for the duration of the blink
float checkerAngle = 0.0f;  // Angle for circular motion
const float checkerRadius = 0.02f; // Radius of circular movement
const float checkerRotationSpeed = 0.0015f; // Speed of rotation

bool isEating = false;      // Track eating animation state
float foodX = 0.0f;         // Food initial X position
float foodY = 0.8f;         // Food starting Y position
float foodSpeedY = -0.005f; // Falling speed of the food
bool foodDropped = false;   // Whether food is currently dropping
float eatingTimer = 0.0f;   // Timer to reset eating animation
int windowWidth=800;//ehhh
int windowHeight=800;//ehhhh

// Dance animation variables********eh
bool isDancing = false;
float danceStep = 0.0f;
float danceBellyBob = 0.0f;
float danceTailSway = 0.0f;


float pupilScale = 1.0f; // Scale for pupil size
bool isBouncing = false; // Ball bouncing state
bool isButtonPressed = false; // Track button press state
bool isSleeping = false; // Track sleep state



const int numDots = 70;
float dotX[numDots], dotY[numDots], dotSpeedY[numDots];

void initializeDots() {
    for (int i = 0; i < numDots; ++i) {
        dotX[i] = ((rand() % 2000) - 1000) / 1000.0f; // Random X position in [-1.0, 1.0]
        dotY[i] = ((rand() % 2000) - 1000) / 1000.0f; // Random Y position in [-1.0, 1.0]
        dotSpeedY[i] = 0.0001f + (rand() % 3) / 100000.0f; // Random downward speed
    }
}

void initializeSparkles() {
    for(int i=0; i<NUM_SPARKLES; i++) {
        sparkleActive[i] = false;
        sparkleAlpha[i] = 0.0f;
    }
}//eh****

void drawCircle(float cx, float cy, float r, int numSegments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * i / numSegments;
        float x = r * cos(angle);
        float y = r * sin(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}
void drawDJBackground() {
    // Simple checkerboard pattern
    float squareSize = 0.2f;
    bool colorSwitch = true;

    for(float y = -1.0f; y < 1.0f; y += squareSize) {
        for(float x = -1.0f; x < 1.0f; x += squareSize) {
            if(colorSwitch) {
                drawRectangle(x, y, x+squareSize, y+squareSize,
                            djColor1[0], djColor1[1], djColor1[2]);
            } else {
                drawRectangle(x, y, x+squareSize, y+squareSize,
                            djColor2[0], djColor2[1], djColor2[2]);
            }
            colorSwitch = !colorSwitch;
        }
        colorSwitch = !colorSwitch;
    }
}
void drawPolkaDots() {
    float dotRed = 1.0f;
    float dotGreen = 0.8f;
    float dotBlue = 0.4f;

    if (isSleeping) {
        // Change to darker blue if the cat is sleeping
        dotRed = 0.15f;
        dotGreen = 0.15f;
        dotBlue = 0.4f;
    }

    for (int i = 0; i < numDots; ++i) {
        drawCircle(dotX[i], dotY[i], 0.02f, 20, dotRed, dotGreen, dotBlue);

        dotY[i] -= dotSpeedY[i]; // Move dots down

        // Reset dot position if it goes off-screen
        if (dotY[i] < -1.0f) {
            dotY[i] = 1.0f;
            dotX[i] = ((rand() % 2000) - 1000) / 1000.0f; // Reset X position
        }
    }
}

void drawCatTail() {
    float tailX = 0.35f+ danceTailSway*0.5f; // Add tail sway***EHH ***// Starting X position (right side)
    float tailY = -0.55f; // Starting Y position (floor level)
    float tailSegmentRadius = 0.08f; // Radius for  tail segments
    int numSegments = 5; // Number of  segments

    for (int i = 0; i < numSegments; i++) {
        drawCircle(tailX, tailY, tailSegmentRadius, 20, 0.9f, 0.8f, 0.7f);
       tailX += 0.07f + 0.01f * sin(danceTailSway * 10); // Subtle wave*******eh***
        tailY += 0.02f * (i % 2 == 0 ? 1 : -1); // Small up-down zigzag effect

        drawCircle(tailX, tailY - 0.06f, tailSegmentRadius * 0.7f, 20, 0.9f, 0.8f, 0.7f);
    }
}
void drawCat() {
drawCatTail(); //****eh
// Breathing animation effect (subtle scaling)
float breathEffect = 0.01f * sin(breathTime);

// Body****eh
drawCircle(0.0f, -0.4f + danceBellyBob, 0.4f + breathEffect, 50, 0.9f, 0.8f, 0.7f);
// Chest/Underbelly (slightly smaller white circle)
drawCircle(0.0f, -0.35f + danceBellyBob, 0.3f + breathEffect, 50, 0.976, 0.941, 0.910);


// Head
drawCircle(0.0f, 0.2f, 0.3f, 50, 0.9f, 0.8f, 0.7f);



    // Ears
    glColor3f(0.9f, 0.8f, 0.7f); // Outer ear color 0.9f, 0.8f, 0.7f
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.2f, 0.4f); // Left ear
    glVertex2f(-0.1f, 0.6f);
    glVertex2f(0.0f, 0.4f);
    glVertex2f(0.2f, 0.4f); // Right ear
    glVertex2f(0.1f, 0.6f);
    glVertex2f(0.0f, 0.4f);
    glEnd();

// Inner ears (lighter color inside the ear)
glColor3f(1.0, 0.905, 0.914); // Light peach color for inner ear
glBegin(GL_TRIANGLES);
glVertex2f(-0.15f, 0.45f); // Left ear inner part
glVertex2f(-0.1f, 0.57f);
glVertex2f(-0.05f, 0.49f);
glVertex2f(0.15f, 0.45f); // Right ear inner part
glVertex2f(0.1f, 0.57f);
glVertex2f(0.05f, 0.49f);
glEnd();

// Lines on top of the cat's head
glColor3f(0.37f, 0.18f, 0.16f);
drawLine(-0.03f, 0.5f, -0.03f, 0.45f, 0.37f, 0.18f, 0.16f); // Left line
drawLine(0.0f, 0.5f, 0.0f, 0.45f, 0.37f, 0.18f, 0.16f);   // Middle line
drawLine(0.03f, 0.5f, 0.03f, 0.45f, 0.37f, 0.18f, 0.16f);   // Right line

   if (isSleeping || isBlinking) {
    // Closed eyes (either sleeping or blinking)
    drawLine(-0.12f, 0.25f, -0.08f, 0.25f, 0.37f, 0.18f, 0.16f); // Left closed eye
    drawLine(0.08f, 0.25f, 0.12f, 0.25f, 0.37f, 0.18f, 0.16f);   // Right closed eye
} else {
    // Open eyes
    drawCircle(-0.1f, 0.25f, 0.05f, 20, 0.37f, 0.18f, 0.16f); // Left eye
    drawCircle(0.1f, 0.25f, 0.05f, 20, 0.37f, 0.18f, 0.16f);  // Right eye
    drawCircle(-0.1f, 0.25f, 0.02f * pupilScale, 20, 1.0f, 1.0f, 1.0f); // Left pupil
    drawCircle(0.1f, 0.25f, 0.02f * pupilScale, 20, 1.0f, 1.0f, 1.0f);  // Right pupil
}

    // Nose
    glColor3f(1.0f, 0.5f, 0.5f); // Pink nose
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.02f, 0.2f);
    glVertex2f(0.02f, 0.2f);
    glVertex2f(0.0f, 0.18f);
    glEnd();

   // Mouth
   //****eh
if(isDancing) {
        // Surprised ":O" mouth///ehhhh
        drawCircle(0.0f, 0.14f, 0.05f, 20, 1.0f, 0.5f, 0.5f);
        // Wider eyes
        drawCircle(-0.1f, 0.25f, 0.06f, 20, 0.37f, 0.18f, 0.16f);
        drawCircle(0.1f, 0.25f, 0.06f, 20, 0.37f, 0.18f, 0.16f);
        drawCircle(-0.1f, 0.25f, 0.03f, 20, 1.0f, 1.0f, 1.0f);
        drawCircle(0.1f, 0.25f, 0.03f, 20, 1.0f, 1.0f, 1.0f);}

   else if (isEating) {
    drawCircle(0.0f, 0.14f, 0.05f, 20, 1.0f, 0.3f, 0.3f); // Open mouth while eating
}
else if (isSleeping) {
    // Only draw the snoring mouth
    float snoreMouthRadius = 0.015f + 0.005f * sin(breathTime);
    drawCircle(0.0f, 0.14f, snoreMouthRadius, 20, 0.37f, 0.18f, 0.16f); // Darker circular open mouth
    drawLine(0.0f, 0.18f, 0.0f, 0.15f, 0.37f, 0.18f, 0.16f);
} else {
    if (pupilScale > 1.0f) {
        // Open mouth when excited
        drawCircle(0.0f, 0.14f, 0.03f, 20, 1.0f, 0.5f, 0.5f); // Pink open mouth
    } else {
        // Closed mouth
        drawLine(0.0f, 0.18f, -0.03f, 0.15f, 0.37f, 0.18f, 0.16f); // Left side
        drawLine(0.0f, 0.18f, 0.03f, 0.15f, 0.37f, 0.18f, 0.16f);  // Right side
    }
}


    // Whiskers
    drawLine(-0.15f, 0.2f, -0.3f, 0.25f, 0.37f, 0.18f, 0.16f); // Left whisker top
    drawLine(-0.15f, 0.18f, -0.3f, 0.18f, 0.37f, 0.18f, 0.16f); // Left whisker middle
    drawLine(-0.15f, 0.16f, -0.3f, 0.11f, 0.37f, 0.18f, 0.16f); // Left whisker bottom
    drawLine(0.15f, 0.2f, 0.3f, 0.25f, 0.37f, 0.18f, 0.16f);   // Right whisker top
    drawLine(0.15f, 0.18f, 0.3f, 0.18f, 0.37f, 0.18f, 0.16f);   // Right whisker middle
    drawLine(0.15f, 0.16f, 0.3f, 0.11f, 0.37f, 0.18f, 0.16f);   // Right whisker bottom

    //collar
    drawRectangle(-0.2f, -0.02f, 0.2f, -0.05f, 0.910, 0.278, 0.416); // Blue collar
    drawCircle(0.0f, -0.1f, 0.05f, 30, 1.0, 1.0, 0.624); // Yellow bell
drawLine(-0.02f, -0.1f, 0.02f, -0.1f, 0.0f, 0.0f, 0.0f);  // Bell line




    // Legs
    drawRectangle(-0.2f, -0.82f, -0.1f, -0.7f, 0.9f, 0.8f, 0.7f); // Left leg
    drawRectangle(0.1f, -0.82f, 0.2f, -0.7f, 0.9f, 0.8f, 0.7f);  // Right leg
}

void drawBall() {
    drawCircle(ballX, ballY, ballRadius, 20, 0.0f, 0.5176f, 0.6275f);
}


//EH*********
void drawHouseplantTub() {
    // Tree roots system
    glColor3f(0.3f, 0.15f, 0.05f); // Root color
    glBegin(GL_LINES);
        // Main roots
        glVertex2f(-0.68f, -0.4f); glVertex2f(-0.72f, -0.45f);
        glVertex2f(-0.65f, -0.4f); glVertex2f(-0.62f, -0.43f);
        glVertex2f(-0.66f, -0.4f); glVertex2f(-0.66f, -0.46f);
    glEnd();

    // Detailed tree trunk with bark texture
    drawRectangle(-0.7f, -0.35f, -0.6f, -0.25f, 0.35f, 0.2f, 0.1f); // Trunk base
    // Bark details
    drawLine(-0.69f, -0.34f, -0.68f, -0.3f, 0.25f, 0.15f, 0.05f);
    drawLine(-0.65f, -0.33f, -0.64f, -0.28f, 0.25f, 0.15f, 0.05f);
    drawLine(-0.62f, -0.35f, -0.61f, -0.29f, 0.25f, 0.15f, 0.05f);

    // Layered Christmas tree with branches
    glColor3f(0.1f, 0.4f, 0.1f); // Darker green base
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.9f, -0.25f);  // Left base
        glVertex2f(-0.4f, -0.25f);  // Right base
        glVertex2f(-0.65f, 0.3f);   // Top point
    glEnd();

    // Lighter green upper layer
    glColor3f(0.2f, 0.6f, 0.2f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.85f, -0.15f);
        glVertex2f(-0.45f, -0.15f);
        glVertex2f(-0.65f, 0.25f);
    glEnd();

    // Snow effect on branches
    drawCircle(-0.6f, 0.0f, 0.02f, 10, 1.0f, 1.0f, 1.0f);
    drawCircle(-0.7f, -0.1f, 0.015f, 8, 1.0f, 1.0f, 1.0f);
    drawCircle(-0.55f, 0.15f, 0.018f, 9, 1.0f, 1.0f, 1.0f);

    // Detailed gifts with ribbons
    // Red gift with gold ribbon
    drawRectangle(-0.8f, -0.4f, -0.6f, -0.32f, 0.8f, 0.1f, 0.1f);
    drawLine(-0.7f, -0.4f, -0.7f, -0.32f, 1.0f, 0.8f, 0.0f); // Vertical ribbon
    drawLine(-0.8f, -0.36f, -0.6f, -0.36f, 1.0f, 0.8f, 0.0f); // Horizontal ribbon

    // Green gift with red ribbon
    drawRectangle(-0.58f, -0.38f, -0.5f, -0.3f, 0.1f, 0.5f, 0.1f);
    drawLine(-0.54f, -0.38f, -0.54f, -0.3f, 0.9f, 0.1f, 0.1f);
    drawLine(-0.58f, -0.34f, -0.5f, -0.34f, 0.9f, 0.1f, 0.1f);

    // Blue gift with silver ribbon (added on left side)
    drawRectangle(-0.95f, -0.42f, -0.85f, -0.35f, 0.1f, 0.1f, 0.8f);
    drawLine(-0.9f, -0.42f, -0.9f, -0.35f, 0.8f, 0.8f, 0.8f);
    drawLine(-0.95f, -0.39f, -0.85f, -0.39f, 0.8f, 0.8f, 0.8f);

    // Golden star topper with glow
    drawCircle(-0.65f, 0.35f, 0.05f, 7, 1.0f, 0.9f, 0.0f);
    drawCircle(-0.65f, 0.35f, 0.03f, 7, 1.0f, 0.95f, 0.3f);
    drawLine(-0.65f, 0.4f, -0.65f, 0.3f, 1.0f, 0.9f, 0.0f); // Star spike
    drawLine(-0.6f, 0.35f, -0.7f, 0.35f, 1.0f, 0.9f, 0.0f); // Horizontal spike
}//EH*********

void drawFood() {
    if (foodDropped) {
        drawCircle(foodX, foodY, 0.05f, 20, 0.9f, 0.6f, 0.2f); // Brown food pellet
    }
}



  void drawBlanket() {
    // Number of segments for the circle
    int numSegments = 50;
    float radius = 0.5f;
    float centerX = 0.0f;
    float centerY = -0.5f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(centerX, centerY); // Center of the blanket

    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * M_PI * float(i) / float(numSegments);
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Add a wavy effect
        float waveEffect = 0.05f * sin(5.0f * angle); // Declare and calculate waveEffect here

        // Calculate the distance from the center
        float distanceFromCenter = sqrt(x * x + y * y);

        // Simulate shading based on the distance from the center
        // The outer parts will be lighter, and the center will be darker
        float shadeFactor = 1.0f - (distanceFromCenter / radius); // Normalize distance to [0, 1]
        shadeFactor = fmax(shadeFactor, 0.3f); // Ensure shadeFactor is at least 0.3 for lighter colors

        // Apply shading with a lighter base color
        glColor3f(0.2f * shadeFactor, 0.5f * shadeFactor, 0.8f * shadeFactor); // Apply shading

        glVertex2f(centerX + x, centerY + y + waveEffect);
    }
    glEnd();
}






void drawHappyAuraBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.4f, 1.0f); // Purple top left
    glVertex2f(-1.0f, 1.0f);
    glColor3f(1.0f, 0.4f, 0.4f); // Red top right
    glVertex2f(1.0f, 1.0f);
    glColor3f(1.0f, 1.0f, 0.5f); // Yellow bottom right
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.8f, 1.0f, 0.5f); // Greenish-yellow bottom left
    glVertex2f(-1.0f, -1.0f);
    glEnd();
}

// snowy gradient background (default)

   void drawWinterBackground() {

        if (isSleeping) {
        // Set background color to dark blue when the cat sleeps
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f); // Dark blue
    } else {
        // Default warm yellow colors
        glClearColor(1.0f, 0.67f, 0.0f, 1.0f); // Warm yellow
    }

    int numRows = 10, numCols = 10;  // Number of checker rows and columns
    float squareSize = 0.2f;         // Size of each square

    float colors[3][3];

    if (isSleeping) {
        // Dark blue tones for night mode
        colors[0][0] = 0.1f; colors[0][1] = 0.1f; colors[0][2] = 0.3f;  // Darkest blue
        colors[1][0] = 0.15f; colors[1][1] = 0.15f; colors[1][2] = 0.4f; // Medium dark blue
        colors[2][0] = 0.2f; colors[2][1] = 0.2f; colors[2][2] = 0.5f;  // Lightest dark blue
    } else {
        // Default warm yellow colors
        colors[0][0] = 1.0f; colors[0][1] = 0.9f; colors[0][2] = 0.6f;
        colors[1][0] = 1.0f; colors[1][1] = 0.8f; colors[1][2] = 0.4f;
        colors[2][0] = 1.0f; colors[2][1] = 0.7f; colors[2][2] = 0.2f;
    }

    float offsetX = checkerRadius * cos(checkerAngle);
    float offsetY = checkerRadius * sin(checkerAngle);

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            float x1 = -1.0f + col * squareSize + offsetX;
            float y1 = -1.0f + row * squareSize + offsetY;
            float x2 = x1 + squareSize;
            float y2 = y1 + squareSize;

            // Wraparound effect for seamless movement
            if (x1 > 1.0f) x1 -= 2.0f, x2 -= 2.0f;
            if (y1 > 1.0f) y1 -= 2.0f, y2 -= 2.0f;

            // Alternate between 3 shades of yellow
            int colorIndex = (row + col) % 3;
            drawRectangle(x1, y1, x2, y2, colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
        }
    }

    // Update checkerboard angle for circular motion
    checkerAngle += checkerRotationSpeed;
    if (checkerAngle >= 2 * M_PI) checkerAngle = 0.0f;  // Reset after a full circle
}

void drawDimmingOverlay() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Black overlay with 50% opacity
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    glDisable(GL_BLEND);
}




void drawButton() {
    drawRectangle(-0.8f, -0.9f, -0.5f, -0.7f, 0.3f, 0.3f, 0.5f); // Button color (gray)
    glColor3f(0.0f, 0.0f, 0.0f); // Text color (black)
    glRasterPos2f(-0.75f, -0.82f);
    const char* text = "Bounce!";
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }
}

void drawLantern() {
    if (!isSleeping) {
        // Draw light glow effect
        drawCircle(0.0f, 0.75f, 0.15f, 50, 1.0f, 1.0f, 0.5f); // Outer light glow
        drawCircle(0.0f, 0.75f, 0.1f, 50, 1.0f, 1.0f, 0.3f); // Inner light glow
    } else {
        // Dimmed glow when sleeping
        drawCircle(0.0f, 0.75f, 0.15f, 50, 0.1f, 0.1f, 0.1f); // Dimmed outer glow
        drawCircle(0.0f, 0.75f, 0.1f, 50, 0.05f, 0.05f, 0.05f); // Dimmed inner glow
    }

    // Draw the lantern body
    drawRectangle(-0.05f, 0.7f, 0.05f, 0.8f, 0.3f, 0.3f, 0.3f); // Lantern rectangle (gray)

    // Draw the handle line
    drawLine(0.0f, 0.8f, 0.0f, 0.85f, 0.3f, 0.3f, 0.3f); // Thin line above the lantern

    // Draw the top circle of the handle
    drawCircle(0.0f, 0.87f, 0.02f, 20, 0.3f, 0.3f, 0.3f); // Small circle for the handle top
}
void drawSparkles() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i=0; i<NUM_SPARKLES; i++) {
        if(sparkleActive[i]) {
            // Random colorful sparkles
            float r = 0.8f + 0.2f*sin(i);
            float g = 0.5f + 0.5f*cos(i*2);
            float b = 0.7f + 0.3f*sin(i*3);

            glColor4f(r, g, b, sparkleAlpha[i]);
            glBegin(GL_TRIANGLE_FAN);
            for(int j=0; j<20; j++) {
                float angle = 2*M_PI*j/20;
                glVertex2f(
                    sparkleX[i] + sparkleSize[i]*cos(angle),
                    sparkleY[i] + sparkleSize[i]*sin(angle)
                );
            }
            glEnd();
        }
    }
    glDisable(GL_BLEND);
}//eh***



void updateBall(int value) {
       ballY += ballSpeedY;
    ballX += ballSpeedX;
    ballSpeedY += gravity;

    // Dance animation updates**********eh****
    if (isDancing) {
        danceBellyBob = sin(danceStep) * 0.03f; // Subtle belly bob
        danceTailSway = sin(danceStep * 1.5f) * 0.1f; // Tail sway
        danceStep += 0.08f; // Animation speed
    }

    if(isDancing) {
    for(int i=0; i<NUM_SPARKLES; i++) {
        if(!sparkleActive[i]) {
            // Activate new sparkles
            sparkleX[i] = (rand()%2000-1000)/1000.0f; // Random X (-1 to 1)
            sparkleY[i] = (rand()%1000)/1000.0f;      // Random Y (0 to 1)
            sparkleSize[i] = 0.02f + (rand()%10)/100.0f;
            sparkleAlpha[i] = 1.0f;
            sparkleActive[i] = true;
        } else {
            // Update existing sparkles
            sparkleY[i] += 0.001f;
            sparkleAlpha[i] -= 0.01f;

            if(sparkleAlpha[i] <= 0.0f) {
                sparkleActive[i] = false;
            }
        }
    }
}

    if (ballY - ballRadius <= -1.0f) {
        ballY = -1.0f + ballRadius;
        ballSpeedY = -ballSpeedY * bounceFactor;
    }

    if (ballX - ballRadius <= -1.0f || ballX + ballRadius >= 1.0f) {
        ballSpeedX = -ballSpeedX * bounceFactor;
    }

    // Gradually reset pupil scale to normal
    if (pupilScale > 1.0f) {
        pupilScale -= 0.02f;
        if (pupilScale < 1.0f) pupilScale = 1.0f;
    }

    // Reset the happy expression if the ball has stopped bouncing
    if (fabs(ballSpeedY) < 0.0001f) {
        isButtonPressed = false;
    }

    // Breathing animation
breathTime += 0.02f;


    // Handle blinking logic
    blinkTimer -= 0.015f;
    if (blinkTimer <= 0.0f) {
        isBlinking = true;
        blinkDuration = 0.15f; // Duration for the blink effect
        blinkTimer = 2.0f + (rand() % 300) / 100.0f; // Randomize next blink time (2-5 seconds)
    }

    if (isBlinking) {
        blinkDuration -= 0.015f;
        if (blinkDuration <= 0.0f) {
            isBlinking = false;
        }
    }

     // Snoring animation
    if (isSleeping) {
        snoreTime += 0.1f; // Adjust speed for snoring effect
    } else {
        snoreTime = 0.0f; // Reset snore animation when awake
    }

    //Eating

    // Food drop logic
    if (foodDropped) {
        foodY += foodSpeedY; // Food falls
        if (foodY <= -0.3f) { // When food reaches the cat's mouth level
            foodDropped = false;
            isEating = true; // Trigger eating animation
            eatingTimer = 1.5f; // Eating animation duration
        }
    }

    // Eating animation countdown
    if (isEating) {
        eatingTimer -= 0.016f; // Approximate frame time step
        if (eatingTimer <= 0.0f) {
            isEating = false; // End eating animation
        }
    }


    glutPostRedisplay();
    glutTimerFunc(15, updateBall, 0); // Call updateBall every 16 milliseconds
}

void keyboard(unsigned char key, int x, int y) {

        if (key == 'd' || key == 'D') {
        isDancing = !isDancing;
        if (isDancing) {
            // Start playing dance music
            PlaySound(TEXT("Dance.wav"), NULL, SND_ASYNC | SND_LOOP);
            isSleeping = false;
            isEating = false;
        } else {
            // Stop dance music
            PlaySound(NULL, NULL, 0);
            // Reset dance positions
            danceBellyBob = 0.0f;
            danceTailSway = 0.0f;
        }
    }
   //sleeping
    if (key == 's' || key == 'S') {
        isSleeping = !isSleeping;

glutPostRedisplay();
    }

    if (key == 'f' || key == 'F') {
        if (!foodDropped && !isEating) { // Drop food only if not already dropping or eating
            foodDropped = true;
            foodX = 0.0f;
            foodY = 0.8f;
        }
    }
}

void reshape(int w, int h) {
    if(w == 0) w = 1;
    if(h == 0) h = 1;

    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    float aspect = (float)w / (float)h;
    if(w >= h) {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }

    //gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert mouse coordinates to OpenGL coordinates
        float oglX = (float)x / windowWidth * 2.0f - 1.0f;
        float oglY = 1.0f - (float)y /  windowHeight * 2.0f;

        // Check if the click is within the button area
        if (oglX >= -0.8f && oglX <= -0.5f && oglY >= -0.9f && oglY <= -0.7f) {
            ballSpeedY = 0.05f; // Apply an upward force to the ball
            isButtonPressed = true; // Trigger happy expression
            pupilScale = 1.5f;
        }
    }
}






void drawMiniCat() {
    // Body
    drawCircle(0.6f, 0.6f, 0.1f, 50, 1.0f, 0.7f, 0.8f);
    // Head
    drawCircle(0.6f, 0.75f, 0.08f, 50, 1.0f, 0.7f, 0.8f);
    // Eyes
    drawCircle(0.58f, 0.78f, 0.015f, 20, 0.0f, 0.0f, 0.0f);
    drawCircle(0.62f, 0.78f, 0.015f, 20, 0.0f, 0.0f, 0.0f);
    // Nose
    drawCircle(0.6f, 0.74f, 0.01f, 10, 1.0f, 0.5f, 0.5f);
}


void drawPhotoFrame() {
    float frameTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f;

    // Base frame colors
    float frameRed = isSleeping ? 0.1f : 0.7f;
    float frameGreen = isSleeping ? 0.1f : 0.5f;
    float frameBlue = isSleeping ? 0.3f : 0.3f;

    // Dance mode enhancements
    if(isDancing) {
        // Pulsing frame border
        float pulse = 0.02f * sin(frameTime * 5.0f);
        drawRectangle(0.4f - pulse, 0.45f - pulse, 0.8f + pulse, 0.9f + pulse,
                     fabs(sin(frameTime)), fabs(cos(frameTime*0.7f)), fabs(sin(frameTime*1.3f)));

        // Magical sparkles inside frame
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for(int i=0; i<15; i++) {
            float alpha = 0.5f + 0.5f * sin(frameTime * 3.0f + i);
            // ADD COLOR PARAMETERS TO drawCircle
            drawCircle(0.5f + 0.2f * cos(frameTime + i),
                      0.65f + 0.2f * sin(frameTime * 2 + i),
                      0.008f, 10,
                      1.0f, 0.8f, 0.2f); // RGB values added
        }
        glDisable(GL_BLEND);
    }
    else {
        drawRectangle(0.4f, 0.45f, 0.8f, 0.9f, frameRed, frameGreen, frameBlue);
    }

    // Inner mat
    drawRectangle(0.45f, 0.5f, 0.75f, 0.85f,
                 isSleeping ? 0.2f : 1.0f,
                 isSleeping ? 0.2f : 1.0f,
                 isSleeping ? 0.2f : 1.0f);

    // Animated nail
    drawCircle(0.6f, 0.9f, 0.02f * (1.0f + 0.2f * sin(frameTime * 3.0f)), 20,
              isSleeping ? 0.15f : 0.3f,
              isSleeping ? 0.15f : 0.3f,
              isSleeping ? 0.3f : 0.3f);

    // Mini cat
    if(isDancing) {
        glPushMatrix();
        glTranslatef(0.6f, 0.7f, 0.0f);
        glRotatef(20.0f * sin(frameTime * 3.0f), 0, 0, 1);
        drawMiniCat();
        glPopMatrix();
    } else {
        drawMiniCat();
    }

    // Frame glow effect
    if(isDancing) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // ADD COLOR PARAMETERS TO drawCircle
        drawCircle(0.6f, 0.7f, 0.15f + 0.03f * sin(frameTime * 4.0f), 30,
                  0.8f, 0.8f, 1.0f); // RGB values added
        glDisable(GL_BLEND);
    }
}//Updated*********EH

void drawShortcutFrame() {
    // Frame border
    glColor3f(0.5f, 0.25f, 0.0f); // Brown wooden color
    glBegin(GL_QUADS);
    glVertex2f(-0.9f, 0.8f);  // Top-left
    glVertex2f(-0.3f, 0.8f);  // Top-right
    glVertex2f(-0.3f, 0.6f);  // Bottom-right
    glVertex2f(-0.9f, 0.6f);  // Bottom-left
    glEnd();

    // Inner background
    glColor3f(0.9f, 0.7f, 0.5f); // Light wood color
    glBegin(GL_QUADS);
    glVertex2f(-0.88f, 0.78f);
    glVertex2f(-0.32f, 0.78f);
    glVertex2f(-0.32f, 0.62f);
    glVertex2f(-0.88f, 0.62f);
    glEnd();

    // Shortcut key text (requires GLUT bitmap fonts)
    glColor3f(0.0f, 0.0f, 0.0f); // Black text
    glRasterPos2f(-0.85f, 0.75f);
    const char* shortcuts[] = {
        "   ","Click Bounce - Bounce",
        "S - Sleep ||"
        " D - Play DJ",
        " "
    };
    for (int i = 0; i < 4; i++) {
        glRasterPos2f(-0.85f, 0.75f - (i * 0.05f));
        for (const char* c = shortcuts[i]; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
    }
}

void drawDJBox() {
    // DJ Box Base
    drawRectangle(-0.3f, -0.6f, 0.3f, -0.4f, 0.2f, 0.2f, 0.2f); // Dark base

    // DJ Box Top Panel
    drawRectangle(-0.28f, -0.4f, 0.28f, -0.38f, 0.3f, 0.3f, 0.3f); // Slightly lighter panel

    // Speakers (left & right)
    drawCircle(-0.22f, -0.5f, 0.05f, 20, 0.0f, 0.0f, 0.0f);
    drawCircle(0.22f, -0.5f, 0.05f, 20, 0.0f, 0.0f, 0.0f);

    // Music Equalizer Lights
    for (int i = 0; i < 5; i++) {
        float x = -0.15f + i * 0.075f;
        float height = (rand() % 5) * 0.02f + 0.02f; // Random height effect
        drawRectangle(x, -0.48f, x + 0.04f, -0.48f + height, 0.8f, 0.2f + i * 0.1f, 0.3f);
    }
}





void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (isDancing) {
        drawDJBackground();
        drawSparkles();
    } else if (isButtonPressed) {
        drawHappyAuraBackground();
    } else {
        drawWinterBackground();
    }

    drawPolkaDots();

    if (isSleeping) {
        drawRectangle(-1.0f, -1.0f, 1.0f, -0.6f, 0.1f, 0.1f, 0.3f); // Dark blue floor
    } else {
        drawRectangle(-1.0f, -1.0f, 1.0f, -0.6f, 0.9f, 0.482f, 0.0f); // Floor
    }

    drawHouseplantTub();

    // Draw cat & other elements
    drawCatTail();
    drawCat();

    if (isSleeping) {
        drawDimmingOverlay();
        drawBlanket();
    }

    // Hide ball when sleeping and Dancing
    if (!isDancing && !isSleeping) {
        drawBall();
    }

    drawFood();

    // Hide button when sleeping and Dancing
    if (!isDancing && !isSleeping) {
        drawButton();
    }

    drawLantern();

    if (isDancing) {
        drawDJBox();
    }

    drawPhotoFrame();
    drawShortcutFrame();

    glutPostRedisplay();
    glutSwapBuffers();
}



void init() {
    glClearColor(1.0f, 0.67f, 0.0f, 1.0f); // Background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 2D Orthographic projection
    glMatrixMode(GL_MODELVIEW);
}

void playWelcomeSound() {
    PlaySound(TEXT("welcome.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void playClosingSound() {
    PlaySound(TEXT("closing.wav"), NULL, SND_FILENAME | SND_SYNC);
}

void onWindowClose() {
    playClosingSound();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800,800);
    glutCreateWindow(" Animated Cat ");
      glutMouseFunc(mouseClick); // Register the mouse click function
      glutKeyboardFunc(keyboard); //keyboard function

      glutReshapeFunc(reshape);



    initializeDots();
    initializeSparkles();//eh

    init();

playWelcomeSound(); // Play welcome sound
    glutDisplayFunc(display);
    glutTimerFunc(16, updateBall, 0); // Start the ball update loop
    //sndPlaySound("Dance.wav",SND_ASYNC);

//     glutCloseFunc(onWindowClose);  // Register closing sound function
    glutMainLoop();
    return 0;
}
