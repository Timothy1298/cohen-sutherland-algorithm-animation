#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Window dimensions
const int WIDTH = 800, HEIGHT = 600;

// Clipping window
float xmin = -0.5, ymin = -0.5, xmax = 0.5, ymax = 0.5;

// Region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

struct Line {
    float x1, y1, x2, y2;
};

// Compute region code for a point (x, y)
int computeCode(float x, float y) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
bool cohenSutherlandClip(float &x1, float &y1, float &x2, float &y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) { // Both points inside
            accept = true;
            break;
        } else if (code1 & code2) { // Both points outside (same region)
            break;
        } else {
            int codeOut;
            float x, y;

            codeOut = code1 ? code1 : code2;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else if (codeOut & LEFT) {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    return accept;
}

// Generate random lines
vector<Line> generateLines(int count) {
    vector<Line> lines;
    srand(time(0));
    for (int i = 0; i < count; i++) {
        float x1 = (rand() % 200 - 100) / 100.0f;
        float y1 = (rand() % 200 - 100) / 100.0f;
        float x2 = (rand() % 200 - 100) / 100.0f;
        float y2 = (rand() % 200 - 100) / 100.0f;
        lines.push_back({x1, y1, x2, y2});
    }
    return lines;
}

// Draw a rectangle (clipping window)
void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Render function
void render(vector<Line> &lines) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw the clipping rectangle
    glColor3f(1.0, 1.0, 1.0);
    drawRectangle(xmin, ymin, xmax, ymax);

    // Draw original lines (red)
    for (auto &line : lines) {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex2f(line.x1, line.y1);
        glVertex2f(line.x2, line.y2);
        glEnd();
    }

    // Draw clipped lines (green)
    for (auto &line : lines) {
        float x1 = line.x1, y1 = line.y1, x2 = line.x2, y2 = line.y2;
        if (cohenSutherlandClip(x1, y1, x2, y2)) {
            glColor3f(0.0, 1.0, 0.0);
            glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();
        }
    }
}

int main() {
    if (!glfwInit()) {
        cerr << "GLFW Initialization Failed!" << endl;
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Cohen-Sutherland Line Clipping", NULL, NULL);
    if (!window) {
        cerr << "Failed to create GLFW window!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(-1, 1, -1, 1, -1, 1); // Set coordinate system

    vector<Line> lines = generateLines(10); // Generate 10 random lines

    while (!glfwWindowShouldClose(window)) {
        render(lines);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
