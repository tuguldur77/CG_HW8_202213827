#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstdio>
#include <GL/glew.h>
#include <GL/glut.h>
#include "load_mesh.h"
#include "frame_timer.h"

enum RenderMode {
    IMMEDIATE_MODE,
    VERTEX_ARRAYS
};
RenderMode gRenderMode = IMMEDIATE_MODE;

GLuint gVAO;
GLuint gPositionVBO, gNormalVBO, gIndexVBO;

void setup_vertex_arrays() {
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);

    // --- Position VBO ---
    glGenBuffers(1, &gPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, gPositions.size() * sizeof(Vector3), gPositions.data(), GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0); // ← Must be inside VAO

    // --- Normal VBO ---
    glGenBuffers(1, &gNormalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gNormalVBO);
    glBufferData(GL_ARRAY_BUFFER, gNormals.size() * sizeof(Vector3), gNormals.data(), GL_STATIC_DRAW);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, 0); // ← Must also be inside VAO

    // --- Index Buffer ---
    glGenBuffers(1, &gIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gTriangles.size() * sizeof(Triangle), gTriangles.data(), GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO
}


void init_opengl() {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);      // Hidden surface elimination
    glDisable(GL_CULL_FACE);      // Disable back-face culling
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);       // Required for scaled model (scale = 10)
    glShadeModel(GL_SMOOTH);

    // Global ambient light (Ia = 0.2, 0.2, 0.2)
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // Light 0 (Directional light) — set position later in display()
    GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    // Material: ka = kd = (1,1,1), ks = (0,0,0), p = 0
    GLfloat matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat shininess = 0.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    start_timing();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);

    // ✅ Light direction setup in camera space
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f }; // Directional
    float len = sqrtf(3.0f);
    for (int i = 0; i < 3; ++i) light_position[i] /= len;
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    if (gRenderMode == IMMEDIATE_MODE) {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < gTriangles.size(); ++i) {
            unsigned int k0 = gTriangles[i].indices[0];
            unsigned int k1 = gTriangles[i].indices[1];
            unsigned int k2 = gTriangles[i].indices[2];

            glNormal3f(gNormals[k0].x, gNormals[k0].y, gNormals[k0].z);
            glVertex3f(gPositions[k0].x, gPositions[k0].y, gPositions[k0].z);

            glNormal3f(gNormals[k1].x, gNormals[k1].y, gNormals[k1].z);
            glVertex3f(gPositions[k1].x, gPositions[k1].y, gPositions[k1].z);

            glNormal3f(gNormals[k2].x, gNormals[k2].y, gNormals[k2].z);
            glVertex3f(gPositions[k2].x, gPositions[k2].y, gPositions[k2].z);
        }
        glEnd();
    }
    else {
        glBindVertexArray(gVAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gTriangles.size() * 3), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    float elapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += elapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;

    char title[128];
    sprintf(title, "OpenGL Bunny: %.2f FPS [%s]", fps,
        gRenderMode == IMMEDIATE_MODE ? "Immediate Mode" : "Vertex Arrays");
    glutSetWindowTitle(title);

    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        gRenderMode = IMMEDIATE_MODE;
        gTotalFrames = 0;
        gTotalTimeElapsed = 0;
        printf("Switched to Q1: Immediate Mode\n");
        break;
    case '2':
        gRenderMode = VERTEX_ARRAYS;
        gTotalFrames = 0;
        gTotalTimeElapsed = 0;
        printf("Switched to Q2: Vertex Arrays Mode\n");
        break;
    case 27:
        exit(0);
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL Bunny");

    glewInit();
    init_opengl();
    load_mesh("bunny.obj");
    setup_vertex_arrays();  // Must be after mesh is loaded
    init_timer();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
