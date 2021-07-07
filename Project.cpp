//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Vase.cpp
//  See Lab03 (II) for details
//  ========================================================================

#include <iostream>
#include <cmath>
#include <GL/freeglut.h> // will have to change this at home
#include <math.h>
#include "loadTGA.h"

using namespace std;


const int N = 50;  // Total number of vertices on the base curve

float vertexX_init[N];
float vertexY_init[N];
float vertexZ_init[N];

float angle = 25, cam_hgt = 50.0;  //Rotation angle, camera height

float theta = 10 * (M_PI/180);

float xPos = -10; // Initial xPosition of the ball
float xVelocity = 1; // the given velociy for the ball in x Direction
float yPos = 5; // initial height (y) position for the ball
float yVelocity = 1; // the given velociy for the ball in y Direction
float yAcceleration = -0.1; // the change in velocity due to gravity in y direction
//int counter = 0;
float look_x = 0, look_z= 0, eye_x = 25, eye_z = 41, eye_y = 10;
GLuint txId[6];
GLuint txtId[2];
//Below equations are for initial conditions of the system.

//Max height is given by yMax = (yVelocity / yAcceleration) + yPos
// at this point the Y stops growing so velocity is == to 0

// max height X position is given by abs((yVelocity / yAcceleration) * xVelocity)
// The amount of steps to the top of the curve times the step size of the velocity.

//end position is given by 2* the max X height position

bool goingRight = true;
float thetaRing = 90;
float xPosCannon;
float yPosCannon;
int lightHeightBall;

float cannonHolderX[14] = {0,0,5,5,5,5,2.5,2.5,2.5,2.5,0,0,0,0};
float cannonHolderY[14] = {0,0,0,0,2.5,2.5,2.5,2.5,1.875,1.875,1.875,1.875,0,0};
float cannonHolderZ[14] = {0,0.625,0,0.625,0,0.625,0,0.625,0,0.625,0,0.625,0,0.625};

float ringHolderX[8] = {0,0,0.5,0.5,0.5,0.5,0,0};
float ringHolderY[8] = {0,0,0,0,7.5,7.5,7.5,7.5};
float ringHolderZ[8] = {0,0.5,0,0.5,0,0.5,0,0.5};

//-------------------------------------------------------------------
//-------------------------------------------------------------------

// Computes the normal vector of a triangle/quad
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}


void loadTexture()
{
    glGenTextures(5, txId);     // Create 2 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("skyrender0.tga"); // SIDE
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("skyrender1.tga"); // SIDE
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);  //Use this texture
    loadTGA("skyrender2.tga"); // Roof one
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[3]);  //Use this texture
    loadTGA("skyrender3.tga"); // SIDE
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[4]);  //Use this texture
    loadTGA("skyrender4.tga"); // SIDE
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[5]);  //Use this texture
    loadTGA("skyrender5.tga"); // ROOF
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glGenTextures(2, txtId);

    glBindTexture(GL_TEXTURE_2D, txtId[0]);  //Use this texture
    loadTGA("pole.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txtId[1]);  //Use this texture
    loadTGA("wood.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);




    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}


void skyBoxDrawer()
{
    glEnable(GL_TEXTURE_2D); // need this
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0); glVertex3f(-150, 100, -150);
        glTexCoord2f(0.0, 0.0); glVertex3f(-150, -1.25, -150);
        glTexCoord2f(1.0, 0); glVertex3f(150, -1.25, -150);
        glTexCoord2f(1.0, 1.0); glVertex3f(150, 100, -150);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, txId[4]);
    glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0); glVertex3f(-150, 100, 150);
        glTexCoord2f(0.0, 0.0); glVertex3f(-150, -1.25, 150);
        glTexCoord2f(1.0, 0); glVertex3f(150, -1.25, 150);
        glTexCoord2f(1.0, 1.0); glVertex3f(150, 100, 150);

        glEnd();
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);

        glTexCoord2f(0, 1); glVertex3f(-150, 100, -150);
        glTexCoord2f(0.0, 0.0); glVertex3f(-150, -1.25, -150);
        glTexCoord2f(1, 0); glVertex3f(-150, -1.25, 150);
        glTexCoord2f(1.0, 1.0); glVertex3f(-150, 100, 150);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0); glVertex3f(150, 100, -150);
        glTexCoord2f(0.0, 0.0); glVertex3f(150, -1.25, -150);
        glTexCoord2f(1.0, 0); glVertex3f(150, -1.25, 150);
        glTexCoord2f(1.0, 1.0); glVertex3f(150, 100, 150);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, txId[5]);
    glBegin(GL_QUADS);
        //Floor
        glTexCoord2f(0.0, 1.0); glVertex3f(150, -1.25, 150);
        glTexCoord2f(0.0, 0.0); glVertex3f(150, -1.25, -150);
        glTexCoord2f(1.0, 0); glVertex3f(-150, -1.25, -150);
        glTexCoord2f(1.0, 1.0); glVertex3f(-150, -1.25, 150);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);

        //Roof
        glTexCoord2f(0.0, 1.0); glVertex3f(150, 100, 150);
        glTexCoord2f(0.0, 0.0); glVertex3f(150, 100, -150);
        glTexCoord2f(1.0, 0); glVertex3f(-150, 100, -150);
        glTexCoord2f(1.0, 1.0); glVertex3f(-150, 100, 150);


    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void cannonHolder()
{
    int i;
    glPushMatrix();
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txtId[1]);
        glBegin(GL_QUADS);

            //glNormal()
            glTexCoord2f(0.0, 1.0); glVertex3f(0, 0, 0.625);
            glTexCoord2f(0.0, 0.0); glVertex3f(5, 0, 0.625);
            glTexCoord2f(1.0, 0); glVertex3f(5, 1.875, 0.625);
            glTexCoord2f(1.0, 1.0); glVertex3f(0, 1.875, 0.625);

        glEnd();
        glBindTexture(GL_TEXTURE_2D, txtId[1]);
        glBegin(GL_QUADS);

            glTexCoord2f(0.0, 1.0); glVertex3f(2.5, 1.875, 0.625);
            glTexCoord2f(0.0, 0.0); glVertex3f(5, 1.875, 0.625);
            glTexCoord2f(1.0, 0); glVertex3f(5, 2.5, 0.625);
            glTexCoord2f(1.0, 1.0); glVertex3f(2.5, 2.5, 0.625);

         glEnd();

         glBindTexture(GL_TEXTURE_2D, txtId[1]);
        glBegin(GL_QUADS);

            glTexCoord2f(0.0, 1.0); glVertex3f(0, 0, 0);
            glTexCoord2f(0.0, 0.0); glVertex3f(5, 0, 0);
            glTexCoord2f(1.0, 0); glVertex3f(5, 1.875,0 );
            glTexCoord2f(1.0, 1.0); glVertex3f(0, 1.875, 0);

        glEnd();
        glBindTexture(GL_TEXTURE_2D, txtId[1]);
        glBegin(GL_QUADS);

            glTexCoord2f(0.0, 1.0); glVertex3f(2.5, 1.875, 0);
            glTexCoord2f(0.0, 0.0); glVertex3f(5, 1.875, 0);
            glTexCoord2f(1.0, 0); glVertex3f(5, 2.5, 0);
            glTexCoord2f(1.0, 1.0); glVertex3f(2.5, 2.5, 0);

         glEnd();

        glBegin(GL_QUAD_STRIP);

            for(i = 0; i < 15; i++)
            {
                glVertex3f(cannonHolderX[i], cannonHolderY[i], cannonHolderZ[i]);

            }


        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void initialise(void)
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
    float mat[4] = { 1.0, 0.75, 0.5, 1.0 };
    loadTexture();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glEnable(GL_SMOOTH);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);

    glClearColor (1.0, 1.0, 1.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
}

bool collisionDetection()
{
    // Position of the ball in X and Y collides with constant X and Y of the cannon
    // 2D collision given with Xpos and yPos
    // z == 0
    // If collides with cyclinder then change camera
    bool returnVal = false;

    if(xPos <= (xPosCannon - 8) || xPos >= (xPosCannon + 7)) // Could add y position as well?
    {
        returnVal = true;

    }

    return returnVal;
}



void createCannonVertexs()
{
    int rotation = 15;
    int i;
    // initial square back
    vertexX_init[0] = 0;
    vertexY_init[0] = 0;
    vertexX_init[1] = 1.375;
    vertexY_init[1] = 0;
    vertexX_init[2] = 1.375;
    vertexY_init[2] = 1.375;

    //curved tip of barrel
    for(i = 3; i < 9; i++)
    {
        vertexX_init[i] = sin(rotation)*1.375;
        vertexY_init[i] = cos(rotation)*1.375;
        rotation += 15;
    }
    // slant of barrel
    vertexX_init[9] = vertexX_init[8] - 0.02;
    vertexY_init[9] = vertexY_init[8] + 0.75;
    for(i = 10; i < 32; i++)
    {
        if((i-10)%4 == 0)
        { // goes accross
            vertexY_init[i] = vertexY_init[i-1] + 0.75;
            vertexX_init[i] =  vertexX_init[i-1] - 0.02;

        } else if((i-10)%4 == 1)
        { // goes up no y change
            vertexY_init[i] = vertexY_init[i-1];
            vertexX_init[i] =  vertexX_init[i-1] + 0.15;


        } else if((i-10)%4 == 2)
        { // goes accross from 1 - 2
            vertexY_init[i] = vertexY_init[i-1] + 0.75;
            vertexX_init[i] =  vertexX_init[i-1] - 0.02;


        } else
        { //goes down from 2 - 3
            vertexY_init[i] = vertexY_init[i-1];
            vertexX_init[i] =  vertexX_init[i-1] - 0.15;

        }
    }
    xPosCannon = vertexX_init[32];
    yPosCannon = vertexY_init[32];

}



void cannonBarrel()
{
    float vertexX[N], vertexY[N], vertexZ[N];
    float wx[N], wy[N], wz[N];
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    createCannonVertexs();
    for (int i = 0; i < N; i++)
    {
        vertexX[i] = vertexX_init[i];
        vertexY[i] = vertexY_init[i];
        vertexZ[i] = vertexZ_init[i];
    }

    glPushMatrix();
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHTING);
        for(int j = 0; j < 36; j++)
        {
            //theta = theta + (10*(M_PI/180));
            for(int i = 0; i < N; i++)
            {
                wx[i] = vertexX[i] * cos(theta)+ vertexZ[i] * sin(theta);
                wy[i] = vertexY[i];
                wz[i] = -vertexX[i] * sin(theta)+ vertexZ[i] * cos(theta);
            }
            glBegin(GL_TRIANGLE_STRIP);
                for(int i = 0; i < N; i++)
                {

                    glVertex3f(vertexX[i], vertexY[i], vertexZ[i]);
                    glNormal3f(1,0,0); // This doesnt work for some resoan
                        //vertexX[i-1], vertexY[i-1], 0,
                        //vertexX[i],   vertexY[i],   0, // Shadows for the cannon
                        //vertexX[i],   vertexY[i],   0
                    glVertex3f(wx[i], wy[i], wz[i]);
                }
            glEnd();
            for(int i = 0; i < N; i++)
            {
                vertexX[i] = wx[i];
                vertexY[i] = wy[i];
                vertexZ[i] = wz[i];
            }



        }
        glPopMatrix();
        glDisable(GL_NORMALIZE);

    //~ // above code is for the surface thingy

}
//makes the objects bottom the movement location not the center.
// helps a whole bunch with squares and stuff
// not so useful for circles
void myTranslateCube(float xSize, float ySize, float zSize, float xPosition, float yPosition, float zPosition)
{

    glTranslatef(xPosition + 0.5 * xSize, yPosition + 0.5 * ySize, zPosition + 0.5 * zSize); // moves the middle

}

void cubeMaker(float xSize, float ySize, float zSize, float xPosition, float yPosition, float zPosition)
{
    glPushMatrix();
        myTranslateCube(xSize, ySize, zSize, xPosition, yPosition, zPosition);
        glScalef(xSize, ySize, zSize);
        //glNormal3f(0, 0, 1);
        glutSolidCube(1);

    glPopMatrix();

}

//creates it at the center of the cylinder
void cylinderMaker(float radius, float height, float xPosition, float yPosition, float zPosition)
{
    glPushMatrix();
        glTranslatef(xPosition, yPosition, zPosition);
        glutSolidCylinder(radius, height,50,1);

    glPopMatrix();

}


void basicCannon()
{

    //is made up of rectangles and a cylinder

    //4 cubes to make up the holder
    glPushMatrix();
        glColor3f(0,1,1);
        glTranslatef(0,0,-1.375);
        cannonHolder();
    glPopMatrix();
    glPushMatrix();
        glColor3f(0,1,1);
        glTranslatef(0,0,1.375);
        cannonHolder();
    glPopMatrix();
    //4 cylinders to make up the wheels
    glPushMatrix();
        glColor3f(0,0,1);
        cylinderMaker(0.625,0.625,0.625,0.5,-1.875);
        cylinderMaker(0.625,0.625,3.75,0.5,-1.875);
        cylinderMaker(0.625,0.625,0.625,0.5,1.875);
        cylinderMaker(0.625,0.625,3.75,0.5,1.875);
    glPopMatrix();
    //1 cylinder through the middle of the barrel
    glPushMatrix();
        glColor3f(1,0,1);
        cylinderMaker(.5,3.75,3.125,2.5,-1.5);
    glPopMatrix();
    //last the barrel going through the middle
    glPushMatrix();
        glColor3f(1,0,0);

        glTranslatef(2.3, 1.25,.25);
        glRotatef(315, 0, 0, 1);

        cannonBarrel();
    glPopMatrix();
}

void myTimer(int value)
{
    //Assuming no air resistance only gravity then xVelocity doesnt change
    // but gravity acts on the height of the ball decreacing the vertical acceleraton
    //below is first half
    if(goingRight)
    {
        if(xPos <= 10) // at finished position
        {
            xPos = xPos + xVelocity;
            yVelocity = yVelocity + yAcceleration;
            yPos = yPos + yVelocity;
        }
        else {
            //~ xPos = abs(2 *(1 / 0.1) * 1);
            //~ yPos = 10;
            yVelocity = yVelocity + yAcceleration;
            goingRight = false;

        }
    } else {
        if(xPos > -10) // at finished position
        {
            xPos = xPos - xVelocity;
            yVelocity = yVelocity - yAcceleration;
            yPos = yPos - yVelocity;
        } else {
            xPos = -10; // Initial xPosition of the ball
            xVelocity = 1; // the given velociy for the ball in x Direction
            yPos = 5; // initial height (y) position for the ball
            yVelocity = 1; // the given velociy for the ball in y Direction
            yAcceleration = -0.1;
            thetaRing = 90;
            goingRight = true;

        }

    }
    thetaRing = thetaRing + 180/15;
    if(thetaRing >= 360)
    {
        thetaRing = thetaRing - 360;
    }
    glutPostRedisplay();
    glutTimerFunc(50, myTimer, 0);
}

// generates the moving balls location
void movingBall()
{

    glPushMatrix();

        glTranslatef(xPos, yPos, 0); // initial position is updated each time
        glBindTexture(GL_TEXTURE_2D, txId[0]);
        GLUquadric* q;
        q = gluNewQuadric();
        gluQuadricTexture(q, GL_TRUE);
        gluSphere(q,.5, 25, 10);
    glPopMatrix();

}

// gives the given ball a shadow and takes in the lights position for it
void generateShadowBall(float xPos, float yPos, float zPos)
{
    float shadowMat[16] = { yPos,0,0,0, -xPos,0,-zPos,-1,0,0,yPos,0,  0,0,0,yPos };
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glColor4f(0.2, 0.2, 0.2, 1.0);
        glMultMatrixf(shadowMat);
        movingBall();

    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPushMatrix();
        glColor4f(0,1,1,1);
        movingBall();
    glPopMatrix();

}

//Holds the translations for the spinning ring in a push pull matrix
void movingRing()
{
    //Rotating disk for the ball to go through
    glPushMatrix();
        glRotatef(thetaRing, 0,1,0);
        glTranslatef(0, 9.5, 0);
        //glScalef(1,3,4);
        glutSolidTorus(.5,1.65,200,100);
    glPopMatrix();

}
// Generates the shadow for the spinning ring
void generateShadowRing(float xPos, float yPos, float zPos)
{

    float shadowMat[16] = { yPos,0,0,0, -xPos,0,-zPos,-1,0,0,yPos,0,  0,0,0,yPos };
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glColor4f(0.2, 0.2, 0.2, 1.0);
        glMultMatrixf(shadowMat);
        movingRing();

    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPushMatrix();
        glColor4f(0,1,0,1);
        movingRing();
    glPopMatrix();


}

//creates a pole to hold up the ring
void ringHolder()
{
    int i;
    glPushMatrix();
    glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, txtId[0]);
        glBegin(GL_QUADS);

            glTexCoord2f(0.0, 0.5); glVertex3f(0,0,0.5);
            glTexCoord2f(0.0, 0.0); glVertex3f(0.5,0,0.5);
            glTexCoord2f(0.5, 0); glVertex3f(0.5,7.5,0.5);
            glTexCoord2f(0.5, 0.5); glVertex3f(0,7.5,0.5);

         glEnd();

         glBindTexture(GL_TEXTURE_2D, txtId[0]);

         glBegin(GL_QUADS);

            glTexCoord2f(0.0, 0.5); glVertex3f(0,0,0);
            glTexCoord2f(0.0, 0.0); glVertex3f(0.5,0,0);
            glTexCoord2f(0.5, 0); glVertex3f(0.5,7.5,0);
            glTexCoord2f(0.5, 0.5); glVertex3f(0,7.5,0);

         glEnd();

        glBegin(GL_QUAD_STRIP);

            for(i = 0; i < 9; i++)
            {
                glVertex3f(ringHolderX[i], ringHolderY[i], ringHolderZ[i]);

            }


        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}


//-------------------------------------------------------------------
void display(void)
{

    float lgt_pos[]={5.0f, 50.0f, 100.0f, 1.0f};



    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z,  look_x, 0, look_z,   0, 1, 0);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glRotatef(angle, 0, 1, 0); // --- FX

    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

    // Displays SkyBox
    glPushMatrix();
        glColor3f(1,1,1);
        skyBoxDrawer();
    glPopMatrix();

    // Displays the first cannon
    glPushMatrix();
        glTranslatef(-15.3,0,0);
        basicCannon();
    glPopMatrix();

    // Displays the second cannon
    glPushMatrix();
        glTranslatef(15,0,0);
        glRotatef(180,0,1,0);

        basicCannon();
    glPopMatrix();

    //checks for collision of the ball and the cannon and flips
    //the light sorce to give the impresion that the light sorce for it has gone
    if(collisionDetection())
    {
        lightHeightBall = -100;

    } else {

        lightHeightBall = 100;
    }
    //places the ball with a shadow in sceen
    generateShadowBall(0, lightHeightBall, 0);

    //places the ring with a shadow in sceen
    generateShadowRing(0,100,0);

    //places the ring holder in sceen
    glPushMatrix();
        glTranslatef(0,0,-0.25);
        ringHolder();
    glPopMatrix();

    glutSwapBuffers();
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 1*sin(angle);
        eye_z += 1*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 1*sin(angle);
        eye_z -= 1*cos(angle);
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}



//-------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (750, 750);
    glutInitWindowPosition (600, 100);
    glutCreateWindow ("Project");
    initialise ();
    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(50, myTimer, 0);
    glutMainLoop();

   return 0;
}
