#include <GL/glut.h>

#include <stdio.h>
#include "Wire.h"
#include "Light.h"

#define MAX_RESOLUTION 80
#define MIN_RESOLUTION 20
#define RESOLUTION_DELTA 10

#define WIRE_SIZE 500

#define SPHERE_RADIUS 100
#define SPHERE_SLICES 20

#define MIN_PARTICLES_PER_HAIR 1
#define MAX_PARTICLES_PER_HAIR 10

#define REFRESH_RATE 60

/********** User IDs for callbacks ********/


Wire* myWire;

Light* dirLight;

//Hair related variables
float hairEnabled = 0;
int particlesPerHair = MIN_PARTICLES_PER_HAIR;

//Flags to show/hide elements
int showParticles = 1;
int showTexture = 1;
int showWireframe = 1;
int flatShade = 0;
int lightEnabled = 1;
int figure = 0;
bool isPaused = true;

int resolution = MIN_RESOLUTION;

float rotationX = 0.0;
float rotationY = 0.0;
float prevX = 0.0;
float prevY = 0.0;
bool mouseDown = false;

//Gravity force
float gravityForce = 10.0f;
Vector* gravityDirection;
float ks = 20;
float kd = 0.2f;
float particleRadius = 5;

//Texture
GLuint texName = 0;
unsigned char woodtexture[512][512][3];

//Camera variabless
float global_ambient[4] = { 0.0, 0.0, 0.0, 0.0 };
float cameraEye[3] = { 100.0, 50.0, 800.0 };
float cameraLookAt[4] = { 0.0, 0.0, 0.0, 1.0 };
float cameraUp[4] = { 0.0, 1.0, 0.0 };

float viewRotation[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
float lightRotation[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

//Object position
float objPos[3] = { 0.0f, 0.0f, 0.0f };

float fovy = 45.0;
float dNear = 100;
float dFar = 2000;

float intensity = 1.0f;


void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)w / (GLdouble)h, dNear, dFar);
    glViewport(0, 0, w, h);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(lightRotation);
    glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);

    glLoadIdentity();
    gluLookAt(cameraEye[0], cameraEye[1], cameraEye[2], cameraLookAt[0], cameraLookAt[1], cameraLookAt[2], cameraUp[0], cameraUp[1], cameraUp[2]);

    glMultMatrixf(viewRotation);

    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationY, 0, 1, 0);

    myWire->renderTriangles(showWireframe, flatShade);

    if (lightEnabled)
    {
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }

    if (showParticles)
    {
        myWire->renderParticles();
    }

    glLightfv(GL_LIGHT0, GL_DIFFUSE, dirLight->difusse);

    if (showTexture)
    {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_COLOR_MATERIAL);
    }

    if (flatShade)
    {
        glShadeModel(GL_FLAT);
    }
    else
    {
        glShadeModel(GL_SMOOTH);
    }

    //myWire->renderSprings();
    glPushMatrix();
    //glTranslatef(objPos[0], objPos[1], objPos[2]);
    //glutSolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_SLICES);
    glPopMatrix();
    /*
        if(showWireframe)
        {
            gluQuadricDrawStyle(cylinder, GLU_LINE);
        }
        else
        {
            gluQuadricDrawStyle(cylinder, GLU_FILL);
        }
        glPushMatrix();
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 0, -CYLINDER_HEIGHT/2);
            gluSphere(cylinder, CYLINDER_RADIUS, CYLINDER_SLICES, CYLINDER_SLICES);
            gluCylinder(cylinder, CYLINDER_RADIUS, CYLINDER_RADIUS, CYLINDER_HEIGHT, CYLINDER_SLICES, CYLINDER_SLICES);
            glTranslatef(0, 0, CYLINDER_HEIGHT);
            gluSphere(cylinder, CYLINDER_RADIUS, CYLINDER_SLICES, CYLINDER_SLICES);
        glPopMatrix();
    */
    glutSwapBuffers();
}

void init(void)
{
    rotationX = 0;
    rotationY = 0;

    gravityDirection = new Vector(0, -1, 0);

    //Cylinder related
    //cylinder = gluNewQuadric();
    //p0 = new Vector(-120, 0, 0);
    //p1 = new Vector(120, 0, 0);

    //Lighting related
    dirLight = new Light();
    dirLight->setPosition(0, 300, 100);
    dirLight->setAmbient(0, 0, 0);
    dirLight->setDifusse(0.8f, 0.8f, 0.8f);
    dirLight->setSpecular(0.0f, 0.8f, 0.0f);

    glEnable(GL_NORMALIZE);

    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    //initTexture();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glLightfv(GL_LIGHT0, GL_POSITION, dirLight->pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, dirLight->ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, dirLight->specular);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dirLight->difusse);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseDown = true;
        prevX = x - rotationY;
        prevY = y - rotationX;
    }

    else {
        mouseDown = false;
    }
}

void mouseMotion(int x, int y) {
    if (mouseDown) {
        rotationX = y - prevY;
        rotationY = x - prevX;
        glutPostRedisplay();
    }
}

void initWire()
{
    myWire = new Wire(resolution, WIRE_SIZE);
}

void control_cb(int control)
{

    initWire();

    float diff1 = dirLight->difusse[0] + intensity;
    float diff2 = dirLight->difusse[1] + intensity;
    float diff3 = dirLight->difusse[2] + intensity;

    dirLight->setDifusse(diff1, diff2, diff3);


    glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
    if (key == 'x') cameraEye[0] -= 50;
    if (key == 'X') cameraEye[0] += 50;
    if (key == 'y') cameraEye[1] -= 50;
    if (key == 'Y') cameraEye[1] += 50;
    if (key == 'z') cameraEye[2] -= 50;
    if (key == 'Z') cameraEye[2] += 50;
    if (key == 'p') showParticles = !showParticles;
    if (key == 'P') showParticles = !showParticles;
    if (key == 't' || key == 'T') showTexture = !showTexture;
    if (key == 'w' || key == 'W') showWireframe = !showWireframe;
    if (key == 's' || key == 'S') flatShade = !flatShade;
    if (key == 'b' || key == 'B') isPaused = !isPaused;
    if (key == 'r') initWire();
    if (key == '+')
    {
        if (resolution < MAX_RESOLUTION)
        {
            resolution += RESOLUTION_DELTA;
            initWire();
        }
    }
    if (key == '-')
    {
        if (resolution > MIN_RESOLUTION)
        {
            resolution -= RESOLUTION_DELTA;
            initWire();
        }
    }

    glutPostRedisplay();
}

void update(int i)
{
    if (!isPaused)
    {
        myWire->update(SPHERE_RADIUS, gravityForce, *gravityDirection, ks, kd, particleRadius, objPos);
        glutPostRedisplay();
    }

    //Uncoment to use glui
    glutTimerFunc(60, update, 1);
    //GLUI_Master.set_glutTimerFunc(REFRESH_RATE, update, 10);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(800, 600);

    glutCreateWindow("Cloth");

    init();

    //Comment if you dont want to use GLUI
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    //glutTimerFunc(60, update, 10);
    glutMotionFunc(mouseMotion);

    //Uncoment if you want to use GLUI
    /*GLUI_Master.set_glutDisplayFunc(display);
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutKeyboardFunc(key);
    GLUI_Master.set_glutMouseFunc(mouse);
    GLUI_Master.set_glutIdleFunc(NULL);
    GLUI_Master.set_glutTimerFunc(REFRESH_RATE, update, 10);
    */
    glutMotionFunc(mouseMotion);

    initWire();

    //glui();
    update(1);
    glutMainLoop();

    return(EXIT_SUCCESS);
}