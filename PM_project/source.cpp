#pragma comment(lib, "winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include "Loop.h"
#include "Light.h"
#include "Cannon.h"
#include "Collision.h"
#include <FreeImage.h>

enum GAME_STATE { HOME, GUIDE, PLAY, CLEAR, FAIL, BOARD };
GAME_STATE state = HOME;

Loop loop;
Cannon cannon;
Collision collision;
Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);
Material mtl1, mtl2, mtl3;
vector<int> score;

clock_t start_t = clock();
clock_t end_t;

bool gamestatus = true;

static GLuint textureIDs[6];
GLubyte* textureDatas[6];
int imageWidths[6], imageHeights[6];

FIBITMAP* createBitMap(char const* filename) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
    if (format == -1) {
        cout << "Could not find image: " << filename << " - Aborting." << endl;
        exit(-1);
    }
    else if (format == FIF_UNKNOWN) {
        cout << "Couldn't determine file format - attempting to get from file extension..." << endl;
        format = FreeImage_GetFIFFromFilename(filename);
        if (!FreeImage_FIFSupportsReading(format)) {
            cout << "Detected image format cannot be read!" << endl;
            exit(-1);
        }
    }
    FIBITMAP* bitmap = FreeImage_Load(format, filename);
    int bitsPerPixel = FreeImage_GetBPP(bitmap);
    FIBITMAP* bitmap32;
    if (bitsPerPixel == 32) bitmap32 = bitmap;
    else bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    return bitmap32;
}

void initialize() {
    mtl1.setEmission(0.1f, 0.1f, 0.1f, 1.0f);
    mtl1.setAmbient(0.0215f, 0.1745f, 0.0215f, 1.0f);
    mtl1.setDiffuse(0.07568f, 0.61424f, 0.07568f, 1.0f);
    mtl1.setSpecular(0.633f, 0.727811f, 0.633f, 1.0f);
    mtl1.setShininess(0.6f);

    mtl2.setEmission(0.1f, 0.1f, 0.1f, 1.0f);
    mtl2.setAmbient(0.1745f, 0.01175f, 0.01175f, 1.0f);
    mtl2.setDiffuse(0.61424f, 0.04136f, 0.04136f, 1.0f);
    mtl2.setSpecular(0.727811f, 0.626959f, 0.626959f, 1.0f);
    mtl2.setShininess(50.0f);

    mtl3.setEmission(0.1f, 0.1f, 0.1f, 1.0f);
    mtl3.setAmbient(0.0f, 0.1f, 0.06f, 1.0f);
    mtl3.setDiffuse(0.01f, 0.01f, 0.01f, 1.0f);
    mtl3.setSpecular(0.5f, 0.5f, 0.5f, 1.0f);
    mtl3.setShininess(50.0f);

    light.setAmbient(0.5f, 0.5f, 0.5f, 1.0f);
    light.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
    light.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);

    loop.createLoop(mtl1);

    FIBITMAP* bitmap1_32 = createBitMap("Home.PNG");
    imageWidths[0] = FreeImage_GetWidth(bitmap1_32);
    imageHeights[0] = FreeImage_GetHeight(bitmap1_32);
    textureDatas[0] = FreeImage_GetBits(bitmap1_32);
    glGenTextures(1, &textureIDs[0]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[0], imageHeights[0], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[0]);

    FIBITMAP* bitmap2_32 = createBitMap("Rules.PNG");
    imageWidths[1] = FreeImage_GetWidth(bitmap2_32);
    imageHeights[1] = FreeImage_GetHeight(bitmap2_32);
    textureDatas[1] = FreeImage_GetBits(bitmap2_32);
    glGenTextures(1, &textureIDs[1]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[1], imageHeights[1], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[1]);

    FIBITMAP* bitmap3_32 = createBitMap("Cannon.PNG");
    imageWidths[2] = FreeImage_GetWidth(bitmap3_32);
    imageHeights[2] = FreeImage_GetHeight(bitmap3_32);
    textureDatas[2] = FreeImage_GetBits(bitmap3_32);
    glGenTextures(1, &textureIDs[2]);
    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidths[2], imageHeights[2], 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureDatas[2]);

    PlaySound(TEXT("Intro.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
}

void displayCharacters(void* font, string str, float x, float y) {
    glRasterPos2f(x, y);
    for (int i = 0; i < str.size(); i++)
        glutBitmapCharacter(font, str[i]);
}

void keyboardDown(unsigned char key, int x, int y) {
    if (state == PLAY) {
        switch (key) {
        case 32:
            if (cannon.getSphereonCannon().size() == 0) break;
            if (cannon.getLaunchedSphere().size() == 0) {
                cannon.launchSphere();
                PlaySound(TEXT("Launch.wav"), NULL, SND_ASYNC);
            }
            break;
        case 'q':
            cannon.CCWrotate();
            break;
        case 'w':
            cannon.CWrotate();
            break;
        }
    }
    else if (state == HOME) {
        if (key == 27) exit(0);
        else if (key == 13) {
            state = PLAY;
            PlaySound(TEXT("Play.wav"), NULL, SND_ASYNC);
            score.push_back(0);
        }
        else if (key == 'g' || key == 'G') state = GUIDE;
        else if (key == 's' || key == 'S') state = BOARD;
    }
    else if (state == FAIL || state == CLEAR) {
        if (key == 27) exit(0);
        else if (key == 'h' || key == 'H') {
            state = HOME;
            PlaySound(TEXT("Intro.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
            loop.getSphereString().clear();
            cannon.setCannonAngle(0);
            cannon.getSphereonCannon().clear();
        }
    }
    else if (state == BOARD) {
        if (key == 'h' || key == 'H') state = HOME;
    }
    else if (state == GUIDE) {
        if (key == 27) exit(0);
        else if (key == 13) {
            state = PLAY;
            PlaySound(TEXT("Play.wav"), NULL, SND_ASYNC);
            score.push_back(0);
        }
        else if (key == 'h' || key == 'H') state = HOME;
    }
    glutPostRedisplay();
}

void specialKeyDown(int key, int x, int y) {
    if (state == PLAY) {
        int angle = cannon.getCannonAngle();
        if ((angle > -90 && angle < 90) || (angle > 270 && angle < 360) || (angle > -360 && angle < -270)) {
            switch (key) {
            case GLUT_KEY_RIGHT:
                cannon.CWrotate();
                break;
            case GLUT_KEY_LEFT:
                cannon.CCWrotate();
                break;
            }
        }
        else if ((angle > 90 && angle < 270) || (angle > -270 && angle < -90)) {
            switch (key) {
            case GLUT_KEY_RIGHT:
                cannon.CCWrotate();
                break;
            case GLUT_KEY_LEFT:
                cannon.CWrotate();
                break;
            }
        }
        if ((angle > -180 && angle < 0) || (angle > 180 && angle < 360)) {
            switch (key) {
            case GLUT_KEY_DOWN:
                cannon.CWrotate();
                break;
            case GLUT_KEY_UP:
                cannon.CCWrotate();
                break;
            }
        }
        else if ((angle > -360 && angle < -180) || (angle > 0 && angle < 180)) {
            switch (key) {
            case GLUT_KEY_DOWN:
                cannon.CCWrotate();
                break;
            case GLUT_KEY_UP:
                cannon.CWrotate();
                break;
            }
        }
    }
}

void idle() {
    if (state != PLAY) return;

    Material mtl;
    int temp = rand() % 3; // variable to pick random Material
    int temp2 = rand() % 3;

    bool checkstatus = true;

    end_t = clock();

    if ((float)(end_t - start_t) > 1000 / 60.0f) {
        loop.moveSphere();
        cannon.flySphere();
        if (loop.checkPreprogress()) {
            if (temp == 0) {
                loop.addSphere(mtl1, 1);
            }
            else if (temp == 1) {
                loop.addSphere(mtl2, 2);
            }
            else {
                loop.addSphere(mtl3, 3);
            }
            loop.moveSphere();
        }
        else {
            if (temp == 0) {
                loop.addSphere(mtl1, 1);
                cannon.addSphereOnCannon(mtl1, 1);
            }
            else if (temp == 1) {
                loop.addSphere(mtl2, 2);
                cannon.addSphereOnCannon(mtl2, 2);
            }
            else {
                loop.addSphere(mtl3, 3);
                cannon.addSphereOnCannon(mtl3, 3);
            }
        }
    }

    for (int i = 0; i < loop.getSphereString().size(); i++) {
        if (loop.getSphereString().begin()->getLoopPointIdx() == loop.getEndLoopPointIdx()) {
            state = FAIL;
            PlaySound(TEXT("Fail.wav"), NULL, SND_ASYNC);
            score.pop_back();
            break;
        }

        if (cannon.getLaunchedSphere().size() == 0) break;

        if (collision.isCollision(loop.getSphereString()[i], cannon.getLaunchedSphere()[0])) {
            if (collision.isDelete(cannon.getLaunchedSphere()[0], i, loop.getSphereString()) == -1) {
                loop.insertSphere(cannon.getLaunchedSphere()[0], i);
                PlaySound(TEXT("Insert.wav"), NULL, SND_ASYNC);
                if (loop.getSphereString().begin()->getLoopPointIdx() > loop.getEndLoopPointIdx()) {
                    state = FAIL;
                    PlaySound(TEXT("Fail.wav"), NULL, SND_ASYNC);
                    score.pop_back();
                }
                cannon.delLaunchedSphere(0);
                checkstatus = false;
            }
            else {
                int bef = loop.getSphereString().size();
                loop.delSphere(collision.isDelete(cannon.getLaunchedSphere()[0], i, loop.getSphereString()));
                PlaySound(TEXT("Delete.wav"), NULL, SND_ASYNC);
                int aft = loop.getSphereString().size();
                int diff = bef - aft;
                int plus = 5 * diff * (diff + 1);
                score.back() += plus;
                if (loop.getSphereString().size() == 0) {
                    state = CLEAR;
                    PlaySound(TEXT("Clear.wav"), NULL, SND_ASYNC);
                }
                cannon.delLaunchedSphere(0);
                checkstatus = false;
            }
        }
        if (!checkstatus) break;
    }
    glutPostRedisplay();
}
void homedisplay() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-7 * boundaryX / 8, -boundaryY / 3);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-7 * boundaryX / 8, 2 * boundaryY / 3);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(7 * boundaryX / 8, 2 * boundaryY / 3);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(7 * boundaryX / 8, -boundaryY / 3);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    displayCharacters(GLUT_BITMAP_HELVETICA_18, "Press Enter to start game", -120.0f, -150.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "g : Game Rules", -250.0f, -250.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "s : Scoreboard", 150.0f, -250.0f);
}
void guidedisplay() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-7 * boundaryX / 8, -boundaryY / 3);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-7 * boundaryX / 8, 2 * boundaryY / 3);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(7 * boundaryX / 8, 2 * boundaryY / 3);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(7 * boundaryX / 8, -boundaryY / 3);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "Press Enter to start game", -120.0f, -180.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "h : Home", -250.0f, -250.0f);
}
void faildisplay() {
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "THE GAME IS OVER", -110.0f, 50.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "Try harder next time!", -90.0f, -50.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "h : Home", -250.0f, -250.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "esc : exit", 150.0f, -250.0f);
}
void cleardisplay() {
    string str = to_string(score.back());
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "GAME CLEAR!", -80.0f, 50.0f);
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "YOUR SCORE:", -100.0f, -50.0f);
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, str, 70.0f, -50.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "h : Home", -250.0f, -250.0f);
}
void boarddisplay() {
    displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "SCOREBOARD", -80.0f, 240.0f);
    displayCharacters(GLUT_BITMAP_HELVETICA_18, "h : Home", -250.0f, -250.0f);
    if (score.size() == 0) {
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Nobody cleared yet", -80.0f, 0.0f);
    }
    else {
        glLineWidth(3);
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(-240, 150);
        glVertex2f(240, 150);
        glEnd();
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(-80, 200);
        glVertex2f(-80, -225);
        glEnd();
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(80, 200);
        glVertex2f(80, -225);
        glEnd();
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Player", -180.0f, 180.0f);
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Score", -20.0f, 180.0f);
        displayCharacters(GLUT_BITMAP_HELVETICA_18, "Rank", 120.0f, 180.0f);
        vector<int> rank;
        for (int i = 0; i < score.size(); i++) {
            rank.push_back(1);
            for (int j = 0; j < score.size(); j++) {
                if (score[i] < score[j]) rank[i]++;
            }
        }
        for (int i = 0; i < score.size(); i++) {
            displayCharacters(GLUT_BITMAP_HELVETICA_18, to_string(i + 1), -160.0f, 120 - 20 * i);
            displayCharacters(GLUT_BITMAP_HELVETICA_18, to_string(score[i]), -10.0f, 120 - 20 * i);
            displayCharacters(GLUT_BITMAP_HELVETICA_18, to_string(rank[i]), 130.0f, 120 - 20 * i);
        }
    }

}
void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (state == PLAY) {
        //Texturing
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glPushMatrix();
        glTranslatef(cannon.getCannonCenter()[0], cannon.getCannonCenter()[1], cannon.getCannonCenter()[2]);
        glRotatef((GLfloat)cannon.getCannonAngle(), 0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-60, -60);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-60, 60);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(60, 60);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(60, -60);
        glEnd();
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        //Typing
        string str = to_string(score.back());
        displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, "Current score:", 100.0f, 275.0f);
        displayCharacters(GLUT_BITMAP_TIMES_ROMAN_24, str, 250.0f, 275.0f);

        // Draw 3D
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(light.getID());
        light.draw();

        // Implement : Project related
        cannon.draw();
        loop.draw();

        glDisable(light.getID());
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
    }
    else if (state == HOME) {
        homedisplay();
        loop.initialize(mtl1);
        cannon.initialize();
    }
    else if (state == GUIDE) guidedisplay();
    else if (state == FAIL) faildisplay();
    else if (state == CLEAR) cleardisplay();
    else if (state == BOARD) boarddisplay();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    // init GLUT and create Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(350, 50);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Puzzle Loop");
    initialize();

    srand(time(0));
    // register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardDown);
    glutSpecialFunc(specialKeyDown);
    glutIdleFunc(idle);


    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}