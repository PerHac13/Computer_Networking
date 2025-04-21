// Include necessary headers
#include <GL/glut.h>
#include <cmath>
#include <cstring>

// Initialize window size
const int WIDTH = 800, HEIGHT = 600;

// Utility function to set pixel
typedef void (*DrawFunc)();

void setPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// 1. Draw basic shapes
void drawShapes()
{
    // Concentric circles
    for (int r = 10; r <= 100; r += 20)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++)
        {
            float theta = i * M_PI / 180;
            glVertex2f(200 + r * cos(theta), 200 + r * sin(theta));
        }
        glEnd();
    }

    // Ellipse
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++)
    {
        float theta = i * M_PI / 180;
        glVertex2f(500 + 80 * cos(theta), 200 + 40 * sin(theta));
    }
    glEnd();

    // Parallel and perpendicular lines
    glBegin(GL_LINES);
    glVertex2i(100, 400);
    glVertex2i(200, 400);
    glVertex2i(100, 420);
    glVertex2i(200, 420);
    glVertex2i(150, 400);
    glVertex2i(150, 500);
    glEnd();
}

// 2. DDA Algorithm
void DDA(int x1, int y1, int x2, int y2)
{
    float dx = x2 - x1, dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));
    float xInc = dx / steps, yInc = dy / steps;
    float x = x1, y = y1;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2i(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

// 3. Midpoint & Bresenham's Circle
void midpointCircle(int xc, int yc, int r)
{
    int x = 0, y = r, p = 1 - r;
    while (x <= y)
    {
        setPixel(xc + x, yc + y);
        setPixel(xc - x, yc + y);
        setPixel(xc + x, yc - y);
        setPixel(xc - x, yc - y);
        setPixel(xc + y, yc + x);
        setPixel(xc - y, yc + x);
        setPixel(xc + y, yc - x);
        setPixel(xc - y, yc - x);
        x++;
        p += (p < 0) ? 2 * x + 1 : 2 * (x - y--) + 1;
    }
}

// 4. 2D Transformations (example: rotation)
void drawSquare()
{
    glBegin(GL_LINE_LOOP);
    glVertex2i(300, 300);
    glVertex2i(400, 300);
    glVertex2i(400, 400);
    glVertex2i(300, 400);
    glEnd();
}

// 5. 3D Transformations
void drawCube()
{
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(45, 1.0, 1.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
}

// 6. Cohen-Sutherland Line Clipping
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

int computeOutCode(double x, double y, double xmin, double xmax, double ymin, double ymax)
{
    int code = INSIDE;
    if (x < xmin)
        code |= LEFT;
    else if (x > xmax)
        code |= RIGHT;
    if (y < ymin)
        code |= BOTTOM;
    else if (y > ymax)
        code |= TOP;
    return code;
}

void cohenSutherlandClip(double x0, double y0, double x1, double y1, double xmin, double xmax, double ymin, double ymax)
{
    int outcode0 = computeOutCode(x0, y0, xmin, xmax, ymin, ymax);
    int outcode1 = computeOutCode(x1, y1, xmin, xmax, ymin, ymax);
    bool accept = false;

    while (true)
    {
        if (!(outcode0 | outcode1))
        {
            accept = true;
            break;
        }
        else if (outcode0 & outcode1)
        {
            break;
        }
        else
        {
            double x, y;
            int outcodeOut = outcode0 ? outcode0 : outcode1;

            if (outcodeOut & TOP)
            {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            }
            else if (outcodeOut & BOTTOM)
            {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            }
            else if (outcodeOut & RIGHT)
            {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            }
            else
            {
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }

            if (outcodeOut == outcode0)
            {
                x0 = x;
                y0 = y;
                outcode0 = computeOutCode(x0, y0, xmin, xmax, ymin, ymax);
            }
            else
            {
                x1 = x;
                y1 = y;
                outcode1 = computeOutCode(x1, y1, xmin, xmax, ymin, ymax);
            }
        }
    }

    if (accept)
    {
        glBegin(GL_LINES);
        glVertex2d(x0, y0);
        glVertex2d(x1, y1);
        glEnd();
    }
}

// 7. Liang-Barsky Line Clipping
bool cliptest(double p, double q, double &u1, double &u2)
{
    double r;
    if (p < 0.0)
    {
        r = q / p;
        if (r > u2)
            return false;
        else if (r > u1)
            u1 = r;
    }
    else if (p > 0.0)
    {
        r = q / p;
        if (r < u1)
            return false;
        else if (r < u2)
            u2 = r;
    }
    else if (q < 0.0)
        return false;
    return true;
}

void liangBarskyClip(double x0, double y0, double x1, double y1, double xmin, double xmax, double ymin, double ymax)
{
    double dx = x1 - x0, dy = y1 - y0, u1 = 0.0, u2 = 1.0;
    if (cliptest(-dx, x0 - xmin, u1, u2))
        if (cliptest(dx, xmax - x0, u1, u2))
            if (cliptest(-dy, y0 - ymin, u1, u2))
                if (cliptest(dy, ymax - y0, u1, u2))
                {
                    double nx0 = x0 + u1 * dx;
                    double ny0 = y0 + u1 * dy;
                    double nx1 = x0 + u2 * dx;
                    double ny1 = y0 + u2 * dy;
                    glBegin(GL_LINES);
                    glVertex2d(nx0, ny0);
                    glVertex2d(nx1, ny1);
                    glEnd();
                }
}

// 8. Fill Circle and Polygon
void drawFilledCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; i++)
    {
        float theta = i * M_PI / 180;
        glVertex2f(cx + r * cos(theta), cy + r * sin(theta));
    }
    glEnd();
}

void drawFilledPolygon()
{
    glBegin(GL_POLYGON);
    glVertex2i(100, 100);
    glVertex2i(200, 100);
    glVertex2i(200, 200);
    glVertex2i(100, 200);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);

    drawShapes();
    DDA(50, 50, 150, 150);
    midpointCircle(400, 400, 50);
    drawSquare();

    glColor3f(0, 0, 1);
    drawCube();

    glColor3f(1, 0, 1);
    cohenSutherlandClip(50, 50, 600, 400, 100, 500, 100, 500);

    glColor3f(0, 1, 1);
    liangBarskyClip(100, 100, 700, 500, 200, 600, 200, 600);

    glColor3f(0, 1, 0);
    drawFilledCircle(600, 300, 50);
    drawFilledPolygon();

    glFlush();
}

void init()
{
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glPointSize(2);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL CG Lab Solutions");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
// g++ -o lab_solutions main.cpp -lGL -lGLU -lglut -lm
// To compile and run the program, use the command:
// g++ -o lab_solutions lab.cpp -lGL -lGLU -lglut -lm
// Make sure you have the OpenGL and GLUT libraries installed on your system.
// You can run the program using the command:
// ./lab_solutions