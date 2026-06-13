#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

float playerX = 0.0f;
float playerY = -0.75f;
float playerWidth = 0.18f;
float playerHeight = 0.28f;

float enemyX = 0.0f;
float enemyY = 0.9f;
float enemyWidth = 0.18f;
float enemyHeight = 0.28f;

float enemySpeed = 0.015f;
int score = 0;
bool gameOver = false;

void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
        glVertex2f(x - width / 2, y - height / 2);
        glVertex2f(x + width / 2, y - height / 2);
        glVertex2f(x + width / 2, y + height / 2);
        glVertex2f(x - width / 2, y + height / 2);
    glEnd();
}

void drawRoad() {
    glColor3f(0.15f, 0.15f, 0.15f);
    drawRectangle(0.0f, 0.0f, 0.9f, 2.0f);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawRectangle(-0.45f, 0.0f, 0.02f, 2.0f);
    drawRectangle(0.45f, 0.0f, 0.02f, 2.0f);

    glColor3f(1.0f, 1.0f, 0.0f);
    drawRectangle(0.0f, 0.7f, 0.03f, 0.25f);
    drawRectangle(0.0f, 0.25f, 0.03f, 0.25f);
    drawRectangle(0.0f, -0.2f, 0.03f, 0.25f);
    drawRectangle(0.0f, -0.65f, 0.03f, 0.25f);
}

void drawCar(float x, float y, bool isPlayer) {
    if (isPlayer) {
        glColor3f(0.1f, 0.4f, 1.0f);
    } else {
        glColor3f(1.0f, 0.1f, 0.1f);
    }

    drawRectangle(x, y, 0.18f, 0.28f);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawRectangle(x - 0.07f, y + 0.08f, 0.04f, 0.08f);
    drawRectangle(x + 0.07f, y + 0.08f, 0.04f, 0.08f);
    drawRectangle(x - 0.07f, y - 0.08f, 0.04f, 0.08f);
    drawRectangle(x + 0.07f, y - 0.08f, 0.04f, 0.08f);

    glColor3f(0.6f, 0.9f, 1.0f);
    drawRectangle(x, y + 0.04f, 0.10f, 0.07f);
}

bool checkCollision() {
    return playerX - playerWidth / 2 < enemyX + enemyWidth / 2 &&
           playerX + playerWidth / 2 > enemyX - enemyWidth / 2 &&
           playerY - playerHeight / 2 < enemyY + enemyHeight / 2 &&
           playerY + playerHeight / 2 > enemyY - enemyHeight / 2;
}

void resetEnemy() {
    int lane = rand() % 3;

    if (lane == 0) {
        enemyX = -0.28f;
    } else if (lane == 1) {
        enemyX = 0.0f;
    } else {
        enemyX = 0.28f;
    }

    enemyY = 1.1f;
}

void updateGame(int value) {
    if (!gameOver) {
        enemyY -= enemySpeed;

        if (enemyY < -1.1f) {
            score++;
            enemySpeed += 0.001f;
            resetEnemy();
        }

        if (checkCollision()) {
            gameOver = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateGame, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawRoad();
    drawCar(playerX, playerY, true);
    drawCar(enemyX, enemyY, false);

    glColor3f(1.0f, 1.0f, 1.0f);

    std::stringstream ss;
    ss << "Score: " << score;
    drawText(-0.95f, 0.9f, ss.str());

    if (gameOver) {
        glColor3f(1.0f, 0.0f, 0.0f);
        drawText(-0.25f, 0.1f, "GAME OVER");

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(-0.35f, -0.05f, "Press R to Restart");
    }

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A') {
        playerX -= 0.08f;
    }

    if (key == 'd' || key == 'D') {
        playerX += 0.08f;
    }

    if (key == 'r' || key == 'R') {
        gameOver = false;
        score = 0;
        enemySpeed = 0.015f;
        playerX = 0.0f;
        resetEnemy();
    }

    if (key == 27) {
        exit(0);
    }

    if (playerX < -0.28f) {
        playerX = -0.28f;
    }

    if (playerX > 0.28f) {
        playerX = 0.28f;
    }

    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        playerX -= 0.08f;
    }

    if (key == GLUT_KEY_RIGHT) {
        playerX += 0.08f;
    }

    if (playerX < -0.28f) {
        playerX = -0.28f;
    }

    if (playerX > 0.28f) {
        playerX = 0.28f;
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.45f, 0.0f, 1.0f);
    srand(time(0));
    resetEnemy();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Car Dodge Game");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, updateGame, 0);

    glutMainLoop();

    return 0;
}