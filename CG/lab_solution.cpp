#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

void setPixel(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

// 1. Draw basic shapes
void drawShapes()
{
    for (int r = 10; r <= 100; r += 20)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; ++i)
        {
            float theta = i * 3.14159f / 180.0f;
            glVertex2f(200 + cos(theta) * r, 200 + sin(theta) * r);
        }
        glEnd();
    }
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; ++i)
    {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(500 + cos(theta) * 80, 200 + sin(theta) * 40);
    }
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(100, 400);
    glVertex2f(200, 400);
    glVertex2f(100, 420);
    glVertex2f(200, 420);
    glVertex2f(150, 400);
    glVertex2f(150, 500);
    glEnd();
}

// 2. DDA Line Drawing
void DDA(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1, dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    float xInc = dx / steps, yInc = dy / steps;
    float x = x1, y = y1;
    for (int i = 0; i <= steps; ++i)
    {
        setPixel(round(x), round(y));
        x += xInc;
        y += yInc;
    }
}

// 3a. Midpoint Circle
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
        ++x;
        p += (p < 0) ? 2 * x + 1 : 2 * (x - y--) + 1;
    }
}

// 4. 2D Transformations
void drawRotatedSquare(float angleDeg)
{
    float cx = 350, cy = 350, side = 100;
    float angle = angleDeg * 3.14159f / 180.0f;
    float s = sin(angle), c = cos(angle);
    float x[4] = {-side / 2, side / 2, side / 2, -side / 2};
    float y[4] = {-side / 2, -side / 2, side / 2, side / 2};
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; ++i)
    {
        float xr = cx + x[i] * c - y[i] * s;
        float yr = cy + x[i] * s + y[i] * c;
        glVertex2f(xr, yr);
    }
    glEnd();
}

// 5. 3D Transformations (Just 2D simulation for GLFW)
void drawScaledTranslatedRotatedQuad()
{
    float angle = 45 * 3.14159f / 180.0f;
    float c = cos(angle), s = sin(angle);
    float scale = 1.5;
    float tx = 600, ty = 400;
    float x[4] = {-50, 50, 50, -50};
    float y[4] = {-50, -50, 50, 50};
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 4; ++i)
    {
        float xr = (x[i] * c - y[i] * s) * scale + tx;
        float yr = (x[i] * s + y[i] * c) * scale + ty;
        glVertex2f(xr, yr);
    }
    glEnd();
}

// 6. Cohen-Sutherland
bool cohenVisible(float x0, float y0, float x1, float y1, float xmin, float xmax, float ymin, float ymax)
{
    int INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;
    auto computeOutCode = [&](float x, float y)
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
    };
    int out0 = computeOutCode(x0, y0);
    int out1 = computeOutCode(x1, y1);
    bool accept = false;
    while (true)
    {
        if (!(out0 | out1))
        {
            accept = true;
            break;
        }
        else if (out0 & out1)
            break;
        else
        {
            float x, y;
            int out = out0 ? out0 : out1;
            if (out & TOP)
            {
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            }
            else if (out & BOTTOM)
            {
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            }
            else if (out & RIGHT)
            {
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            }
            else
            {
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }
            if (out == out0)
            {
                x0 = x;
                y0 = y;
                out0 = computeOutCode(x0, y0);
            }
            else
            {
                x1 = x;
                y1 = y;
                out1 = computeOutCode(x1, y1);
            }
        }
    }
    if (accept)
    {
        glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glEnd();
    }
    return accept;
}

// 7. Liang-Barsky
void liangBarsky(float x0, float y0, float x1, float y1, float xmin, float xmax, float ymin, float ymax)
{
    float dx = x1 - x0, dy = y1 - y0, u1 = 0.0f, u2 = 1.0f;
    auto clip = [&](float p, float q) -> bool
    {
        float r;
        if (p < 0)
        {
            r = q / p;
            if (r > u2)
                return false;
            else if (r > u1)
                u1 = r;
        }
        else if (p > 0)
        {
            r = q / p;
            if (r < u1)
                return false;
            else if (r < u2)
                u2 = r;
        }
        else if (q < 0)
            return false;
        return true;
    };
    if (clip(-dx, x0 - xmin) && clip(dx, xmax - x0) && clip(-dy, y0 - ymin) && clip(dy, ymax - y0))
    {
        float nx0 = x0 + u1 * dx, ny0 = y0 + u1 * dy;
        float nx1 = x0 + u2 * dx, ny1 = y0 + u2 * dy;
        glBegin(GL_LINES);
        glVertex2f(nx0, ny0);
        glVertex2f(nx1, ny1);
        glEnd();
    }
}

// 8. Fill Circle and Polygon
void drawFilledCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; ++i)
    {
        float theta = i * 3.14159f / 180.0f;
        glVertex2f(cx + cos(theta) * r, cy + sin(theta) * r);
    }
    glEnd();
}

void drawFilledPolygon()
{
    glBegin(GL_POLYGON);
    glVertex2f(200, 200);
    glVertex2f(300, 200);
    glVertex2f(300, 300);
    glVertex2f(200, 300);
    glEnd();
}

void renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 0, 0);
    drawShapes();
    glColor3f(0, 1, 0);
    DDA(100, 100, 400, 300);
    glColor3f(0, 0, 1);
    midpointCircle(600, 100, 50);
    glColor3f(1, 0.5, 0);
    drawRotatedSquare(30);
    glColor3f(0, 1, 1);
    drawScaledTranslatedRotatedQuad();
    glColor3f(1, 0, 1);
    cohenVisible(100, 100, 700, 500, 200, 600, 200, 500);
    glColor3f(0.5, 0.5, 1);
    liangBarsky(150, 150, 650, 450, 200, 600, 200, 500);
    glColor3f(0.3, 1, 0.3);
    drawFilledCircle(700, 500, 40);
    drawFilledPolygon();
}

int main()
{
    if (!glfwInit())
        return -1;
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Lab - GLFW", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
    while (!glfwWindowShouldClose(window))
    {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
// g++ -o lab_glfw lab_glfw.cpp -lglfw -lGL -lm
