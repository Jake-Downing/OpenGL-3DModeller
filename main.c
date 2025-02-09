#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

int cx = 0, cy = 0, cz = 0;
int cn;

int showMessage = 0;
char message[50] = "";

//-------------------------------------------------------//

struct Polygons {
    int x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
    float r, g, b;
    int state;
    int total;
}; 

struct Polygons P[100];

void addPolygon() {
    P[0].state++;
    if (P[0].state > 4) { P[0].state = 1; } // Reset if more than 4 points
    int st = P[0].state;
    
    if (st == 1) {
        P[0].total++;
        cn = P[0].total;
    }
    
    if (st == 1) { P[cn].x1 = cx; P[cn].y1 = cy; P[cn].z1 = cz; }
    if (st == 1 || st == 2) { P[cn].x2 = cx; P[cn].y2 = cy; P[cn].z2 = cz; }
    if (st == 1 || st == 2 || st == 3) { P[cn].x3 = cx; P[cn].y3 = cy; P[cn].z3 = cz; }
    if (st == 1 || st == 2 || st == 3 || st == 4) { P[cn].x4 = cx; P[cn].y4 = cy; P[cn].z4 = cz; }
}

void savePolygonsToFile() {
    FILE *file = fopen("polygons.txt", "w");
    if (!file) {
        sprintf(message, "Save failed!");
        showMessage = 60;
        return;
    }
    fprintf(file, "%d\n", P[0].total);
    
    for (int i = 1; i <= P[0].total; i++) {
        fprintf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %.2f %.2f %.2f\n",
                P[i].x1, P[i].y1, P[i].z1, P[i].x2, P[i].y2, P[i].z2,
                P[i].x3, P[i].y3, P[i].z3, P[i].x4, P[i].y4, P[i].z4,
                P[i].r, P[i].g, P[i].b);
    }
    fclose(file);
    sprintf(message, "File saved!");
    showMessage = 60;
}

void loadPolygonsFromFile() {
    FILE *file = fopen("polygons.txt", "r");
    if (!file) {
        sprintf(message, "Load failed!");
        showMessage = 60;
        return;
    }
    fscanf(file, "%d", &P[0].total);
    for (int i = 1; i <= P[0].total; i++) {
        fscanf(file, "%d %d %d %d %d %d %d %d %d %d %d %d %f %f %f",
               &P[i].x1, &P[i].y1, &P[i].z1, &P[i].x2, &P[i].y2, &P[i].z2,
               &P[i].x3, &P[i].y3, &P[i].z3, &P[i].x4, &P[i].y4, &P[i].z4,
               &P[i].r, &P[i].g, &P[i].b);
    }
    fclose(file);
    sprintf(message, "File loaded!");
    showMessage = 60;
}

void drawPolygons() {
    for (int i = 1; i <= P[0].total; i++) {
        glBegin(GL_QUADS);
        glColor3f(P[i].r, P[i].g, P[i].b);
        glVertex3f(P[i].x1, P[i].y1, P[i].z1);
        glVertex3f(P[i].x2, P[i].y2, P[i].z2);
        glVertex3f(P[i].x3, P[i].y3, P[i].z3);
        glVertex3f(P[i].x4, P[i].y4, P[i].z4);
        glEnd();
    }
}

void drawCube() {
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(cx, cy, cz);
    glutSolidCube(0.4);
    glPopMatrix();
}

void drawGrid() {
	int i;
    for (i = 0; i < 40; i++) {
        glPushMatrix();
        if (i < 20) {
            glTranslatef(0, 0, i);
        } else {
            glTranslatef(i - 20, 0, 0);
            glRotatef(-90, 0, 1, 0);
        }
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex3f(0, -0.1, 0);
        glVertex3f(19, -0.1, 0);
        glEnd();
        glPopMatrix();
    }
}

void renderText(float x, float y, const char *text) {
    glRasterPos2f(x, y);
    for (const char *c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-13.5, 0, -47);
    glRotatef(40, 1, 1, 0);

    drawGrid();
    drawPolygons();
    drawCube();


    if (showMessage > 0) {
        glColor3f(1, 1, 1);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        renderText(10, 580, message);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        showMessage--;
    }

    glutSwapBuffers();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, 1.0f, 0.1f, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.2, 0.2, 1);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') { cz -= 1; } // Forward
    if (key == 's') { cz += 1; } // Backward
    if (key == 'a') { cx -= 1; } // Left
    if (key == 'd') { cx += 1; } // Right
    if (key == 'q') { cy += 1; } // Up
    if (key == 'e') { cy -= 1; } // Down

    if (key == 32) { addPolygon(); } // Spacebar

    if (key == 'r') { P[cn].r = 1; P[cn].g = 0; P[cn].b = 0; }
    if (key == 'g') { P[cn].r = 0; P[cn].g = 1; P[cn].b = 0; }
    if (key == 'b') { P[cn].r = 0; P[cn].g = 0; P[cn].b = 1; }
    if (key == 'y') { P[cn].r = 1; P[cn].g = 1; P[cn].b = 0; }

    if (key == 'v') { savePolygonsToFile(); } // Save to file
    if (key == 'l') { loadPolygonsFromFile(); glutPostRedisplay(); } // Load from file

    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Modeler");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}

