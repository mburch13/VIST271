//
//  main.cpp
//  
// To compile MAC: g++ -o polygon *.cpp -framework Carbon -framework OpenGL -framework GLUT -lm
//  Created by McKenzie on 3/2/18.
//
#include "polygon.h"
using namespace std;

Polygon myPolygon;

void m_Motion(int x, int y) {
    
    y = HEIGHT - y;
    
    if(myPolygon.isSelected()){
        cout << "updating point\n";
        myPolygon.updatePoint(x,y);
    }
    glutPostRedisplay();
}

void handleButton(int button, int state, int x, int y){
    
    static int toolIndex;
    
    y = HEIGHT - y;   // reverse y, so zero at bottom, and max at top
    
    if(button == GLUT_LEFT_BUTTON){   // ignore all but left button
        if(state == GLUT_DOWN){
            if(myPolygon.isClosed() == false){
                myPolygon.append(x,y);
                glutPostRedisplay();
                cout << "redisplay\n";
            }
            if(myPolygon.isClosed() == true){
                if(myPolygon.hitPoint(x,y) == true){
                    glutPostRedisplay();
                    cout << "redisplay\n";
                }
                else if(myPolygon.hitPoint(x,y) == false){
                    myPolygon.hitLine(x,y); //allows for point to be added
                    glutPostRedisplay();
                    cout << "redisplay\n";
                }
            }
            cout << "DOWN\n";
        }
        else if(state == GLUT_UP){
            myPolygon.unSelect();
            cout << "UP\n";
        }
    }
    if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
        myPolygon.close();
        glutPostRedisplay();
    }
    
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
            //if(myPolygon.hitPoint(x,y) == true){
        if(myPolygon.deletePoint(x,y)==true){ //allows for point to be deleted
                    glutPostRedisplay();
                cout << "redisplay\n";
//            }
        }
    }
    if (state == GLUT_UP){
        myPolygon.unSelect();
    }
}


void drawScene(){
    glClear(GL_COLOR_BUFFER_BIT);
    myPolygon.draw();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    
    // open window and establish coordinate system on it
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Polygon Builder");
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    
    // register display and mouse-button callback routines
    glutDisplayFunc(drawScene);
    glutMotionFunc(m_Motion);         // mouse movement event callback
    glutMouseFunc(handleButton);
    
    glClearColor(0, 0, 0, 1);
    
    glutMainLoop();
    
    return 0;
}

