//#include "vector2d.h"
#include "polygon.h"
#include <cmath>
//using namespace std;

float sqr(float x){
    return x*x;
}

// Constructor to intialize variables
Polygon::Polygon() {
	head = tail = selected = NULL;
	closed = false;
}

// append points to end of list using a circular linked list
void Polygon::append(int x, int y) {
    selected = new Point;
    //store data
    selected -> x = (float)x;
    selected -> y = (float)y;
//    selected -> next = NULL;
    //add to list
    if(head == NULL){
        head = selected;
        cout << "head\n";
    }
    else{
        //add to tail
        tail -> next = selected;
    }
    tail = selected;
    tail -> next = head;
}

// update the selected point if not null
void Polygon::updatePoint(int x, int y) {
	if (selected != NULL) {
		selected->x = (float)x;
		selected->y = (float)y;
	}
}

// find the point and set selected if found
bool Polygon::hitPoint(int x, int y) {
    Point *ptr;
    ptr = head;
    if(head != NULL){
        do{
            //distance formula
            if((sqr(ptr -> x - x) + sqr(ptr -> y - y) <= sqr(RADIUS))){
                selected = ptr;
                return true;
            }
            ptr = ptr -> next;
        }while(ptr != head);
    }
    return false;
}

// return true if selected point is not null
bool Polygon::isSelected() {
	return (selected != NULL);
}

//deleting points on polygon
bool Polygon::deletePoint(int x, int y){
    Point *ptr, *prior;
    ptr = head;
    prior = tail;
    if(head != NULL){
        do{
            //distance formula
            if((sqr(ptr -> x - x) + sqr(ptr -> y - y) <= sqr(RADIUS))){
                //conditions
                if(head == tail){
                    cout << "head = tail\n";
                    delete ptr;
                    head = NULL;
                    tail = NULL;
                    closed = false;
                    return true;
                }
                prior -> next = ptr ->next;
                if(ptr == head){
                    head = ptr -> next;
                    delete ptr;
                    return true;
                }
                if(ptr == tail){
                    tail = prior;
                    delete ptr;
                    return true;
                }
            }
            ptr = ptr -> next;
            prior = prior -> next;
        }while(ptr != head);
    }
    return false;
}

//inserting a point on closed polygon
bool Polygon::hitLine(float x, float y){
    Point *ptr, *p1, *p0;
    vector2d A, B, C, D;
    ptr = head;
    
    do{
        p0 = ptr;
        p1 = ptr -> next;
        A.setx((p1 -> x) - (p0 -> x));
        A.sety((p1 -> y) - (p0 -> y));
        B.setx(x - (p0 -> x));
        B.sety(y - (p0 -> y));
        
        C = (B * A.unitVector()) * A.unitVector();
        D = B - C;
        if(D.magnitude() <= RADIUS){
            if(((B * A.unitVector()) > 0) && (C.magnitude() < A.magnitude())){
                Point *temp = new Point;
//               selected = new Point;
               temp -> x = x;
               temp -> y = y;
               temp -> next = p1;
                if(p1 == head)
                    tail = temp;
                p0->next = temp;
                selected = temp;
               return true;
            }
        }
        ptr = ptr->next;
    }while(ptr != head);
    return false;
}

void Polygon::circle(int cx, int cy, int r){
    int k;
    float angle, ainc;
    ainc = 2 * PI/72;
    
    glColor3f(0.4980,1,0);//teal

    glBegin(GL_POLYGON);
    for(k = 0; k < 72; k++){
        angle = k * ainc;
        glVertex2f(cx + r*cos(angle), cy + r*sin(angle));
    }
    glEnd();
}

// draw the polygon
void Polygon::draw() {
    // traverse through linked list and draw the polygon connecting the points
    // if the polygon is closed we need to close the last line of the polygon
    Point *connect = head;
    Point *p0, *p1, *p2, *p3;
    float mB[4][4] = {{-1,3,-3,1}, {3,-6,3,0}, {-3,0,3,0}, {1,4,1,0}};
    float mC[4][4] = {{-1,3,-3,1}, {2,-5,4,-1}, {-1,0,1,0}, {0,2,0,0}};
    float cvx[4], cvy[4], ccx[4], ccy[4];
    float x, y;
    if(head != NULL){
//    needs to know open or closed!!
//    closed true or false??
        glLineWidth(5);
        if(closed && head -> next != tail){
            p0 = head;
            p1 = p0 -> next;
            p2 = p1 -> next;
            p3 = p2 -> next;

//b-spline-------------------------------------------------------------
            glColor3f(0,1,1);//aqua
            do{
                //populate cvx and cvy
                cvx[0] = p0 -> x;
                cvx[1] = p1 -> x;
                cvx[2] = p2 -> x;
                cvx[3] = p3 -> x;

                cvy[0] = p0 -> y;
                cvy[1] = p1 -> y;
                cvy[2] = p2 -> y;
                cvy[3] = p3 -> y;
                
                //                ccx = m * cvx;
                //                ccy = m * cvy;
                for(int i = 0; i<4; i++){
                    ccx[i]=0;
                    ccy[i]=0;
                    for(int j = 0; j<4; j++){
                        ccx[i] += mB[i][j] * cvx[j];
                        ccy[i] += mB[i][j] * cvy[j];
                    }
                }
                glBegin(GL_LINE_LOOP);
                for(float t = 0; t < 1.1; t += .1){
                    x = (ccx[0]*t*t*t + ccx[1]*t*t + ccx[2]*t + ccx[3])/6.0;
                    y = (ccy[0]*t*t*t + ccy[1]*t*t + ccy[2]*t + ccy[3])/6.0;
                    glVertex2f(x,y);
                }
                p0 = p0 -> next;
                p1 = p1 -> next;
                p2 = p2 -> next;
                p3 = p3 -> next;
            }while(p0 != head);
            glEnd();

//catmull-rom-------------------------------------------------------------
            glColor3f(1,0.647,0);//orange
            do{
                //populate cvx and cvy
                cvx[0] = p0 -> x;
                cvx[1] = p1 -> x;
                cvx[2] = p2 -> x;
                cvx[3] = p3 -> x;
                
                cvy[0] = p0 -> y;
                cvy[1] = p1 -> y;
                cvy[2] = p2 -> y;
                cvy[3] = p3 -> y;
                
                //                ccx = m * cvx;
                //                ccy = m * cvy;
                for(int i = 0; i<4; i++){
                    ccx[i]=0;
                    ccy[i]=0;
                    for(int j = 0; j<4; j++){
                        ccx[i] += mC[i][j] * cvx[j];
                        ccy[i] += mC[i][j] * cvy[j];
                    }
                }
                glBegin(GL_LINE_LOOP);
                for(float t = 0; t < 1.1; t += .1){
                    x = (ccx[0]*t*t*t + ccx[1]*t*t + ccx[2]*t + ccx[3])/2.0;
                    y = (ccy[0]*t*t*t + ccy[1]*t*t + ccy[2]*t + ccy[3])/2.0;
                    glVertex2f(x,y);
                }
                p0 = p0 -> next;
                p1 = p1 -> next;
                p2 = p2 -> next;
                p3 = p3 -> next;
            }while(p0 != head);
//-------------------------------------------------------------
            glEnd();
        }
        glColor3f(1,0,1);//pink
        if(closed == true){
            //        create another line
            glBegin(GL_LINE_LOOP);
        }
        else{
            glBegin(GL_LINE_STRIP);
        }
        do{
            glVertex2i(connect->x, connect->y);
            connect = connect -> next;
        }while(connect != head);
        glEnd();
        
        connect = head;
        do{
            circle(connect->x, connect->y, RADIUS);
            connect = connect -> next;
        }while(connect != head);
    }
}
