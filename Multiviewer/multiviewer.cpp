//McKenzie Burch
//VIST 271
//Feburary 28, 2018
/* This program displays a multiview of a smooth shaded teapot.  Each viewport is able to change between orthographic and perspective views.  There is also a fullscreen view when the key 'f' or 'F' is pressed, which has the same ability to change views.

   To compile:
 * Linux: g++ -o multiviewer multiviewer.cpp -lGL -lGLU -lglut -lm
 * Mac:   g++ -o multiviewer multiviewer.cpp -framework Carbon -framework OpenGL -framework GLUT -lm
 */

#ifdef __APPLE__
#  include <OpenGL/gl.h>  // definitions for GL graphics routines
#  include <OpenGL/glu.h> // definitions for GL input device handling
#  include <GLUT/glut.h>  // deginitions for the GLUT utility toolkit
#else
#  include <GL/gl.h>      // definitions for GL graphics routines
#  include <GL/glu.h>     // definitions for GL input device handling
#  include <GL/glut.h>    // deginitions for the GLUT utility toolkit
#endif

#include <iostream>       // definitions for standard I/O routines
#include <cmath>          // definitions for math library
#include <cstdlib>
using namespace std;

#define WIDTH  600
#define HEIGHT 300

#define ESC		27	// numeric code for keyboard Esc key

#define NONE		-1	// used to indicate no mouse button pressed

#define DRAWWIDTH	300	// view volume sizes (note: width and
#define DRAWHEIGHT	300	//   height should be in same ratio as window)
#define NEAR		10	// distance of near clipping plane
#define FAR		1000	// distance of far clipping plane
#define DEPTH		-100	// initial z coord. of center of cube

#define WIDTH   600
#define HEIGHT  600

#define ROTFACTOR	0.2     // degrees rotation per pixel of mouse movement
#define XLATEFACTOR	0.5     // units of translation per pixel of mouse movement

#define AMBIENT_FRACTION 0.2
#define DIFFUSE_FRACTION 0.8

enum ViewType { PERSPECTIVE, FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };
ViewType theViewport[4]; // 4 for the multiview + 1 for the full view

char label[7][20] = { "Perspective View", "Front View", "Back View", "Left View", "Right View", "Top View", "Bottom View" };

// color for objects
const static float LightBlue[4] = {0.5, 0.5, 1.0, 1.0};

// Camera position and orientation
static double Pan;
static double Tilt;
static double Approach;

// model orientation
static double ThetaX;
static double ThetaY;

// global variables to track mouse and shift key
static int MouseX;
static int MouseY;
static int Button = NONE;
int q = 0;  //global variable for quadrent
bool fullscreen = false;  //fullscreen boolean

//find quadrent function
int findQuadrent(int x, int y){
    if(fullscreen == true){
        (0 < x < WIDTH && 0 < y < HEIGHT);
        return 4;
    }
    else{
        if(x > 0 && x < WIDTH/2 && y > 0 && y < HEIGHT/2){
                    return 0;
        }
        if(x > WIDTH/2 && x < WIDTH && y > 0 && y < HEIGHT/2){
            return 1;
        }
        if(x > 0 && x < WIDTH/2 && y > HEIGHT/2 && y < HEIGHT){
           return 2;
        }
        if(x > WIDTH/2 && x < WIDTH && y > HEIGHT/2 && y < HEIGHT){
            return 3;
        }
    }
}

//
// Routine to initialize the state of the program to start-up defaults
//
void setInitialState(){
    
    // initial camera viewing controls
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    
    // set up light 0
    glEnable(GL_LIGHT0);	    // always leave light 0 on
    
    // initial camera orientation and position
    Pan = 0;
    Tilt = 0;
    Approach = DEPTH;
    
    // initial model orientation
    ThetaX = 0;
    ThetaY = 0;
    
    theViewport[0] = LEFT;
    theViewport[1] = PERSPECTIVE;
    theViewport[2] = TOP;
    theViewport[3] = FRONT;
    theViewport[4] = RIGHT;
}

/*
  Output text to screen
*/
void output(GLfloat x, GLfloat y, char *text)
{
    char *p;

    glPushMatrix();
        glTranslatef(x, y, 0);
        glScalef(.1, .1, 1);
        for (p = text; *p; p++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}

/*
  Watch mouse motion
*/
void m_Motion(int x, int y)
{
    int delta;
    
    y = -y;
    int dy = y - MouseY;
    int dx = x - MouseX;
    
    switch(Button){
        case GLUT_LEFT_BUTTON:
            ThetaX -= ROTFACTOR * dy;
            ThetaY += ROTFACTOR * dx;
            glutPostRedisplay();
            break;
        case GLUT_MIDDLE_BUTTON:
            Pan -= ROTFACTOR * dx;
            Tilt += ROTFACTOR * dy;
            glutPostRedisplay();
            break;
        case GLUT_RIGHT_BUTTON:
            delta = (fabs(dx) > fabs(dy)? dx: dy);
            Approach += XLATEFACTOR * delta;
            glutPostRedisplay();
            break;
    }
    
    MouseX = x;
    MouseY = y;
}

/*
  Watch mouse button presses and handle them
*/
void handleButton(int button, int state, int x, int y)
{
    if(state == GLUT_UP)
        Button = NONE;		// no button pressed
    else{
        MouseY = -y;		// invert y window coordinate to correspond with OpenGL
        MouseX = x;
        
        Button = button;		// store which button pressed
    }
}

/*
  Draw the Orthographic Model
 */
void drawOrthoModel()
{
    float ambient_color[4];
    float diffuse_color[4];
    
    for(int i = 0; i < 4; i++){
        ambient_color[i] = AMBIENT_FRACTION * LightBlue[i];
        diffuse_color[i] = DIFFUSE_FRACTION * LightBlue[i];
    }
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    
    // set drawing color to pale blue
    glColor3f(LightBlue[0], LightBlue[1], LightBlue[2]);
    
    glPushMatrix();
    glTranslatef(0,0,100);
    glRotatef(90,0,1,0);
    // draw cube
    glutSolidTeapot(50);
    glPopMatrix();
}

/*
 Draw the Perspective Model
 */
void drawPerspectiveModel()
{
    float ambient_color[4];
    float diffuse_color[4];
    
    for(int i = 0; i < 4; i++){
        ambient_color[i] = AMBIENT_FRACTION * LightBlue[i];
        diffuse_color[i] = DIFFUSE_FRACTION * LightBlue[i];
    }
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_color);
    
    // set drawing color to pale blue
    glColor3f(LightBlue[0], LightBlue[1], LightBlue[2]);
    
    glRotatef(Tilt, 1, 0, 0);	    // tilt - rotate camera about x axis
    glRotatef(Pan, 0, 1, 0);	    // pan - rotate camera about y axis
    glTranslatef(0, 0, Approach);     // approach - translate camera along z axis
    
    // rotate the model
    glRotatef(ThetaY, 0, 1, 0);       // rotate model about x axis
    glRotatef(ThetaX, 1, 0, 0);       // rotate model about y axis*/
    
    glutSolidTeapot(50);
}

/*
 Set the Orthographic Projection
 */
void setOrthoProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-DRAWWIDTH/2, DRAWWIDTH/2, -DRAWHEIGHT/2, DRAWHEIGHT/2, NEAR, FAR);
}

/*
 Set the Perspective Projection using glFrustrum
 */
void setPerspectiveProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // scale drawing coords so center of cube is same size as in ortho
    // if it is at its nominal location
    double scale = fabs((double)NEAR / (double)DEPTH);
    double xmax = scale * DRAWWIDTH / 2;
    double ymax = scale * DRAWHEIGHT / 2;
    glFrustum(-xmax, xmax, -ymax, ymax, NEAR, FAR);
}

void getLookAt(ViewType v){
    switch (v) {
    
        case 1:     //FRONT
            gluLookAt(0,0,0,0,0,100,0,1,0);                     // define the position of the camera
            break;
            
        case 2:     //BACK
            gluLookAt(0,0,360,0,0,100,0,1,0);                     // define the position of the camera
            break;
            
        case 3:     //LEFT
            gluLookAt(360,0,0,0,0,100,0,1,0);                     // define the position of the camera
            break;
            
        case 4:     //RIGHT
            gluLookAt(-360,0,0,0,0,100,0,1,0);                     // define the position of the camera
            break;
            
        case 5:     //TOP
            gluLookAt(0,360,0,0,0,100,0,1,0);                     // define the position of the camera
            break;
            
        case 6:     //BOTTOM
            gluLookAt(0,-360,0,0,0,100,0,1,0);                     // define the position of the camera
            break;
    }
}

/*
  Draw the scene
*/
void drawScene() 
{
    const float light_position[] = {1, 1, 1, 0};  // distant light source, parallel rays

    // clear window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    if(fullscreen == true){
        if(theViewport[q] == PERSPECTIVE){
            glViewport(0,0,600,600);
            // initialize the projection matrix
            setPerspectiveProjection();
            // restore modelview matrix as the one being updated
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
            drawPerspectiveModel();
        }
        else{
            glViewport(0,0,600,600);
            setOrthoProjection();
            // restore modelview matrix as the one being updated
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
            //    gluLookAt(360,0,0,0,0,100,0,1,0);                     // define the position of the camera
            getLookAt(theViewport[q]);
            drawOrthoModel();
        }
    }
    
//--------------------------------------------------------------------------------------------------------
    else{
        if(theViewport[1] == PERSPECTIVE){
            glViewport(300,300,300,300);
            // initialize the projection matrix
            setPerspectiveProjection();
            // restore modelview matrix as the one being updated
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
            drawPerspectiveModel();
        }
        else{
            glViewport(300,300,300,300);
            // initialize the projection matrix
            setOrthoProjection();
            // restore modelview matrix as the one being updated
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
        //    gluLookAt(360,0,0,0,0,100,0,1,0);                     // define the position of the camera
            getLookAt(theViewport[1]);
            drawOrthoModel();                                   // draw the model
        }
//--------------------------------------------------------------------------------------------------------
            if(theViewport[2] == PERSPECTIVE){
                glViewport(0,0,300,300);
                // initialize the projection matrix
                setPerspectiveProjection();
                // restore modelview matrix as the one being updated
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
                //            getLookAt(theViewport[1]);
                drawPerspectiveModel();
            }
            else{
                glViewport(0,0,300,300);
                // initialize the projection matrix
                setOrthoProjection();
                // restore modelview matrix as the one being updated
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
                getLookAt(theViewport[2]);
                drawOrthoModel();                                   // draw the model
            }
//--------------------------------------------------------------------------------------------------------
            if(theViewport[3] == PERSPECTIVE){
                glViewport(300,0,300,300);
                // initialize the projection matrix
                setPerspectiveProjection();
                // restore modelview matrix as the one being updated
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
                //            getLookAt(theViewport[1]);
                drawPerspectiveModel();
            }
            else{
                glViewport(300,0,300,300);
                // initialize the projection matrix
                setOrthoProjection();
                // restore modelview matrix as the one being updated
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
                getLookAt(theViewport[3]);
                drawOrthoModel();                                   // draw the model
            }
//--------------------------------------------------------------------------------------------------------                                  // draw the model
            if(theViewport[0] == PERSPECTIVE){
                glViewport(0,300,300,300);
                // initialize the projection matrix
                setPerspectiveProjection();
                // restore modelview matrix as the one being updated
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
                //            getLookAt(theViewport[1]);
                drawPerspectiveModel();
            }
            else{
                glViewport(0,300,300,300);
                // initialize the projection matrix
                setOrthoProjection();
                // restore modelview matrix as the one being updated
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);  // light should be positioned in camera space so it does not move with object
                getLookAt(theViewport[0]);
                drawOrthoModel();                                   // draw the model
            }
    }
//--------------------------------------------------------------------------------------------------------
        
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        // Full Viewport
        glColor3f(0,0,0);
        glViewport(0, 0, WIDTH, HEIGHT);
        // initialize the projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,600,0,300);
        // restore modelview matrix as the one being updated
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
    glLineWidth(2);
    if(fullscreen == false){
    glBegin(GL_LINES);                                  // draw vertical line to separate viewports
        glVertex2i(300,300);
        glVertex2i(300,0);
    glEnd();
    glBegin(GL_LINES);                                  // draw horizontal line to separate viewports
        glVertex2i(0,150);
        glVertex2i(600,150);
    glEnd();
    }

    // label the viewports
    if(fullscreen == true){
        output(10, 280, label[theViewport[q]]);
    }
    else{
        output(10, 280, label[theViewport[0]]);
        output(10, 130, label[theViewport[2]]);
        output(310, 130, label[theViewport[3]]);
        output(310, 280, label[theViewport[1]]);
    }

    glutSwapBuffers();
}

//
// Keyboard callback routine.
// Set various modes or take actions based on key presses
//
void handleKey(unsigned char key, int x, int y){
    q = findQuadrent(x,y);
    switch(key){
        case '0':
            theViewport[q] = PERSPECTIVE;
            glutPostRedisplay();
            break;
        case '1':
             theViewport[q] = FRONT;
            glutPostRedisplay();
            break;
        case '2':
            theViewport[q] = BACK;
            glutPostRedisplay();
            break;
        case '3':
            theViewport[q] = LEFT;
            glutPostRedisplay();
            break;
        case '4':
            theViewport[q] = RIGHT;
            glutPostRedisplay();
            break;
        case '5':
            theViewport[q] = TOP;
            glutPostRedisplay();
            break;
        case '6':
            theViewport[q] = BOTTOM;
            glutPostRedisplay();
            break;
            
        case 'f':
        case 'F':
            fullscreen = !fullscreen;
            glutPostRedisplay();
            break;
            
        case 'i':			// I -- reinitialize
        case 'I':
            setInitialState();
            glutPostRedisplay();
            break;
  
        case 'q':			// Q or Esc -- exit program
        case 'Q':
        case ESC:
            exit(0);
    }
}

/*
   Main program
*/
int main(int argc, char* argv[]) 
{
    glutInit(&argc, argv);

    // open window and establish coordinate system on it
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Multiview");
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    // register display and mouse-button callback routines
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKey);
    glutMouseFunc(handleButton);
    glutMotionFunc(m_Motion);         // mouse movement event callback

    // Set up to clear screen to black
    glClearColor(.5,.5,.5, 1);

    setInitialState();

    glutMainLoop();

    return 0;
}
