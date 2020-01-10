//  McKenzie Burch
//  VIST 271
//  Date: February 11, 2017
//  Burch_McKenzie_assignment2.cpp
//

/* To compile Mac:   g++ -o reversiGame reversiGame.cpp -framework Carbon -framework OpenGL -framework GLUT
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
#include <cmath>
using namespace std;

#define WIDTH    800    /* initial window dimensions */
#define HEIGHT   800

#define N_ROWS   8      /* number of rows in grid */
#define N_COLS   8      /* number of columns in grid */

#define BOARDSIZE 8   /* define board size to generalize solution */

#define EMPTY 0   /* constants for indicating board square contents */
#define BLACK -1
#define WHITE 1

#define PI 3.1415926536     // mathematical pi, not apple pie

int player = 1;


int board[BOARDSIZE][BOARDSIZE];  /* make board global since all routines use it */

static int Width;       /* global variable window width in pixels*/
static int Height;      /* global variable window height in pixels */

int prow=0, pcol=0;

//draws cirles
void circle(int i)
{
    int k;                   // loop counter
    float angle, ainc;       // angle and angle increment
    
    ainc = 2 * PI / i;
    
    glBegin(GL_POLYGON);
    for(k = 0; k < i; k++) {
        angle = k * ainc;
        glVertex2f(cos(angle)*.4, sin(angle)*.4);
    }
    glEnd();
}

//loops through the board in order to draw either white or black pieces
void drawPieces(){
    for (int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(board[i][j] == WHITE){  //draws white pieces
                glColor3f(1,1,1);
                glPushMatrix();
                glTranslated(j+.5, i+.5, 0);
                circle(36);
                glPopMatrix();
            }
            else if(board[i][j] == BLACK){  //draws black pieces
                glColor3f(0,0,0);
                glPushMatrix();
                glTranslated(j+.5, i+.5, 0);
                circle(36);
                glPopMatrix();
            }
        }
    }
}

/*
 Clear the board and place the initial 4 checkers in the center
 */
void initboard(){
    int i, j;
    
    for(i = 0; i < BOARDSIZE; i++)
        for(j = 0; j < BOARDSIZE; j++)
            board[i][j] = EMPTY;
    
    board[BOARDSIZE / 2 - 1][BOARDSIZE / 2 - 1] = WHITE;
    board[BOARDSIZE / 2][BOARDSIZE / 2] = WHITE;
    board[BOARDSIZE / 2 - 1][BOARDSIZE / 2] = BLACK;
    board[BOARDSIZE / 2][BOARDSIZE / 2 - 1] = BLACK;
}

/*
 Check if (i, j) is a location on the board. Return TRUE if it is, FALSE otherwise
 */
int onboard(int i, int j){
    return (i >= 0 && i < BOARDSIZE && j >= 0 && j < BOARDSIZE);
}

/*
 Get row and column value
 */
void getRowCol(int *row, int *col, int x, int y)
{
    float dx, dy;

    y = Height - y;

    dy = (float)Height / N_ROWS;
    dx = (float)Width / N_COLS;

    *row = (int)(y / dy);
    *col = (int)(x / dx);
}

/*
 Starting from position (i, j) on board, moving in direction indicated by (di, dj),
 count the number of pieces that would be captured by placing a piece of the
 indicated color on square (i, j).
 
 This routine assumes that (i, j) is a valid empty board position and that
 di and dj are both either -1, 0, or 1.
 */
int countcaptured(int i, int j, int di, int dj, int color){
    int k;
    int capturecount;
    
    if(di == 0 && dj == 0)  /* make sure at least one of di, dj is non-zero */
        return 0;
    
    /*
     move in direction indicated by (di, dj) until either a match for color is
     found, or the square is empty, or we are off the board. If a match for the
     color is found, return a count of the opposite color pieces captured.
     If off the board or an empty space, return a count of 0.
     */
    capturecount = 0;
    for(k = 1; k < BOARDSIZE; k++){
        if(!onboard(i + k * di, j + k * dj) || board[i + k * di][j + k * dj] == EMPTY)
            return 0;
        if(board[i + k * di][j + k * dj] == color)
            return capturecount;
        capturecount++;
    }
    
    cout << "error, should never get here!\n";
    return 0;
}

/*
 Checks captures in all directions from (i, j) if specified color is placed at
 this location. Returns total pieces captured, and returns an array of the
 number of pieces captured in each direction.
 
 If the position (i, j) is not on the board, or if the space isn't empty,
 returns 0 pieces captured.
 */
int checkplacement(int i, int j, int color, int numcaptured[3][3]){
    int di, dj;   //change in direction
    int k, l;
    int total;
    
    for(k = 0; k < 3; k++)
        for(l = 0; l < 3; l++)
            numcaptured[k][l] = 0;
    
    if(!onboard(i, j) || board[i][j] != EMPTY)
        return 0;
    
    total = 0;
    for(k = 0, di = -1; di <= 1; di++, k++){
        for(l = 0, dj = -1; dj <= 1; dj++, l++){
            numcaptured[k][l] = countcaptured(i, j, di, dj, color);
            total += numcaptured[k][l];
        }
    }
    return total;
}

/* this function should mimic the structure of countcaptured while actualy changing the value of the pieces that already have been solved for and "captured"*/
void changePieces(int i, int j, int di, int dj, int color, int numcap){
    int k;
    int changeCount;
//    cout <<"hello\n";  //check to see if function is being refrenced
    changeCount = 0;
    for(k = 1; k <= numcap; k++){  //loop throught the board to find oppponent piece that can be flipped to current players color
        board[i+k*di][j+k*dj] = color;
    }
    board[i][j]=color;  //add piece were player clicks
}

/*calls changePieces for already solved captured array in each direction that the array shows by a non-zero number*/

void flipPieces(int i, int j, int color, int numcaptured[3][3]){
    int k, l;
    int di, dj;
    int total;

    for(k = 0, di = -1; di <= 1; di++, k++){
        for(l = 0, dj = -1; dj <= 1; dj++, l++){
            if(numcaptured[k][l] > 0){  //supposed to call changePieces to flip the color value of pieces captured
                changePieces(i, j, di, dj, player, numcaptured[k][l]);
            }
        }
    }
//    cout << "Total " << total << endl;  //check to see if function works

}

bool checkLegalMoves(){
// check if move is avalible
    int numcaptured[3][3];
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(checkplacement(i, j, player, numcaptured))
            return true;
        }
    }
    return false;
}
/*
Draws the hilighted square 
*/
void drawHilight (int col, int row)
{
    /* enable blending with color already stored in the framebuffer */
    glEnable (GL_BLEND);
        glPushAttrib(GL_CURRENT_BIT);         /* save current drawing color     */
        glPushAttrib(GL_POLYGON_BIT);         /* prepare for glPolygonMode      */
            glPolygonMode(GL_FRONT, GL_FILL); /* front facing polygon is filled */
            glColor4f(0,0,.6,.4);             /* transparent shade of blue      */
            glRecti(col, row, col+1, row+1);
        glPopAttrib();
        glPopAttrib();
    glDisable(GL_BLEND);
}

/*
Display Callback Routine: clear the screen and draw a M x N grid
This routine is called every time the window on the screen needs
to be redrawn, such as when the window is iconized and then reopened
It is also called when the window is first created.
*/
void drawGrid(){
    int row, col;

    /* draw the grid using thick red lines */
    glColor3f(0, 0, 0);
    glLineWidth(2);

    /* draw horizontal grid lines */
    glBegin(GL_LINES);
    for(row = 0; row <= N_ROWS; row++){
        glVertex2i(0, row);
        glVertex2i(N_COLS, row);
    }
    glEnd();

    /* draw vertical grid lines */
    glBegin(GL_LINES);
    for(col = 0; col <= N_COLS; col++){
        glVertex2i(col, 0);
        glVertex2i(col, N_ROWS);
    }
    glEnd();
}

void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawHilight(pcol,prow);
    drawGrid();
    drawPieces();
    glutSwapBuffers();
}

/*
PassiveMotionFunc callback. Watch mouse movement 
*/
void p_Motion(int x, int y)
{
    static int oldrow, oldcol;

    getRowCol(&prow, &pcol, x, y);

    if ((prow == oldrow)&&(pcol == oldcol)) return;

    oldrow=prow; oldcol=pcol;  

    glutPostRedisplay();
}

/*
Mouse button event handler, called when button pressed or released
*/
void handleButtons(int button, int state, int x, int y){
    int row, col;
    int numcaptured[3][3];
    int totalcaptured;
    
    if(button != GLUT_LEFT_BUTTON || state != GLUT_UP)
        return;

    getRowCol(&row, &col, x, y);
    
    totalcaptured = checkplacement(row, col, player, numcaptured);
    if(totalcaptured == 0){
        cout << "Invalid move. Please try again.\n";
        return;
    }
    else{
        cout << "button click in cell (" << row << ", " << col << ")\n";
        flipPieces(row, col, player, numcaptured);
        glutPostRedisplay();
        player *= -1; //switch player
        cout << player << endl;
        if(checkLegalMoves()){  return;}        //check legal moves
        else{
            cout << "hello there\n";    //check if else is ever reached
            player *= -1; //switch player
            cout << player << endl;
            if(checkLegalMoves()) return;
            else{
                cout << "GAME OVER\n";
            }
        }
    }
}

/*
Reshape event handler, called whenever window is resized by user
*/
void handleResize(int w, int h){

    Width = w;
    Height = h;

    /* lower left of window is (0, 0), upper right is (WIDTH, HEIGHT) */
    glViewport(0, 0, Width, Height);

    /* drawing coordinates are always in terms of number of rows/cols */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, N_COLS, 0, N_ROWS);
    glMatrixMode(GL_MODELVIEW);
}
/*
Main program to draw the grid, and accept mouse clicks
*/
int main(int argc, char* argv[]){
    cout << player << endl;

    initboard();

    /* start up the glut utilities */
    glutInit(&argc, argv);

    /* create the graphics window, giving width, height, and title text */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Reversi");

    /* define the operation of blending when enabled */
    /* transparency is best implemented with the following 2 parameters */
    glBlendFunc (GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    /* drawing callback routine */
    glutDisplayFunc(drawScene);

    /* mouse button callback routine */
    glutMouseFunc(handleButtons);
    glutPassiveMotionFunc(p_Motion);

    /* window resize callback routine */
    glutReshapeFunc(handleResize);

    /* specify window clear (background) color to be white */
    glClearColor(.7235, .7059, .5490, 1);   // clear screen to tan

    glutMainLoop();
    return 0;
}

