#include <GLFW/glfw3.h>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;
float x = 0.0f, y = 0.0f;  // Ball position
float x_speed = 0.02f, y_speed = 0.015f;  // Ball movement speed
float radius = 0.1f;  // Ball radius

// Function to draw a circle (ball)
void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Center
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

// Main function
int main() {
    // Initialize GLFW
    if (!glfwInit()) return -1;

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bouncing Ball", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Update position
        x += x_speed;
        y += y_speed;

        // Check for bouncing
        if (x + radius > 1.0f || x - radius < -1.0f) x_speed = -x_speed;
        if (y + radius > 1.0f || y - radius < -1.0f) y_speed = -y_speed;

        // Draw the bouncing ball
        glColor3f(1.0f, 0.0f, 0.0f);  // Red color
        drawCircle(x, y, radius);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
