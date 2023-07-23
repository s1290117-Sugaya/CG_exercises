#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _PI 3.14159f

enum { PERSP, ORTHO };
static int g_proj_mode = PERSP;

// window dimension
static int g_width = 800;
static int g_height = 800;

static char filename[13] = "platonic.off";

static void saveAsOFF(GLfloat vertices[][3], int faces[][3], int numOfVertices, int numOfFaces){
    FILE *fp = fopen(filename, "w");
	
    fprintf(fp, "%d %d 0\n", numOfVertices, numOfFaces);
    for(int i=0; i<numOfVertices; i++){
	fprintf(fp, "%f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
    }
    for(int i=0; i<numOfFaces; i++){
	fprintf(fp, "%d %d %d\n", faces[i][0], faces[i][1], faces[i][2]);
    }

    fclose(fp);
}

static void drawIcosahedron(void) {
    GLfloat phi = (1.f + sqrtf(5.f)) * .5f;
    GLfloat a = 1.f;
    GLfloat b = 1.f / phi;

    GLfloat vertices[12][3] = { {0.f,b,-a}, {b,a,0.f}, {-b,a,0.f},
    {0.f,b,a}, {0.f,-b,a}, {-a,0.f,b},
    {0.f,-b,-a}, {a,0.f,-b}, {a,0.f,b},
    {-a,0.f,-b},{b,-a,0.f},{-b,-a,0.f}
    };

    GLfloat color[20][3] = { {0.0f,0.0f,0.6f}, {0.0f,0.0f,0.8f}, {0.0f,0.0f,1.0f},
    {0.f, 0.2f, 1.f}, {0.f,0.4f,1.f}, {0.f,0.6f,1.f}, {0.f,0.8f,1.f}, {0.f,1.f,1.f},
    {0.2f,1.f,0.8f}, {0.4f,1.f,0.6f}, {0.6f,1.f,0.4f}, {0.8f,1.f,0.2f},
    {1.f,1.f,0.f}, {1.f,0.8f,0.f}, {1.f,0.6f,0.f}, {1.f,0.4f,0.f}, {1.f,0.2f,0.f},
    {1.f,0.f,0.f}, {0.8f,0.f,0.f}, {0.6f,0.f,0.f}
    };

    int faces[20][3] = { {2,1,0}, {1,2,3}, {5,4,3}, {4,8,3},
    {7,6,0}, {6,9,0}, {11,10,4}, {10,11,6},
    {9,5,2}, {5,9,11}, {8,7,1}, {7,8,10},
    {2,5,3}, {8,1,3}, {9,2,0}, {1,7,0},
    {11,9,6}, {7,10,6}, {5,11,4}, {10,8,4}
    };

    int i;

    for (i = 0; i < 20; ++i) {
        GLfloat* c = color[i];
        int* f = faces[i];
        int v0 = f[0], v1 = f[1], v2 = f[2];
        glColor3f(c[0], c[1], c[2]);
        glBegin(GL_TRIANGLES);
        glVertex3f(vertices[v0][0], vertices[v0][1], vertices[v0][2]);
        glVertex3f(vertices[v1][0], vertices[v1][1], vertices[v1][2]);
        glVertex3f(vertices[v2][0], vertices[v2][1], vertices[v2][2]);
        glEnd();
    }

    saveAsOFF(vertices, faces, 12, 20);
}

static void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (g_proj_mode == PERSP) {
        gluPerspective(45.0, (GLdouble)g_width / (GLdouble)g_height, 0.1, 20.0);
    }
    else {
        glOrtho(-2.0, 2.0, -2.0, 2.0, -10.0, 10.0);
    }

    // Modelview transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


    glPushMatrix();

    drawIcosahedron();

    glPopMatrix();

    glutSwapBuffers();
}

static void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    g_width = w;
    g_height = h;
}

static void keyboard(unsigned char k, int x, int y) {
    switch (k) {
    case 27:
        exit(EXIT_SUCCESS);
        break;

    case 'p':
        g_proj_mode = (1 - g_proj_mode);
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

static void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Icosahedron");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

