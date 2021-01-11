//
// Emscripten/SDL2/OpenGLES2 sample that demonstrates simple geometry and shaders, mouse and touch input, and window resizing
//
// Setup:
//     Install emscripten: http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html
//
// Build:
//     emcc -std=c++11 hello_triangle.cpp events.cpp camera.cpp -s USE_SDL=2 -s FULL_ES2=1 -s WASM=0 -o hello_triangle.html
//
// Run:
//     emrun hello_triangle.html
//
// Result:
//     A colorful triangle.  Left mouse pans, mouse wheel zooms in/out.  Window is resizable.
//

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL.h>
#include <SDL_opengles2.h>
#include <fstream>
#include <chrono>
#include <iostream>

#include "events.h"

// Vertex shader
GLint shaderPan, shaderZoom, shaderAspect;
const GLchar* vertexSource =
    "uniform vec2 pan;                             \n"
    "uniform float zoom;                           \n"
    "uniform float aspect;                         \n"
    "attribute vec4 position;                      \n"
    "varying vec3 color;                           \n"
    "void main()                                   \n"
    "{                                             \n"
    "    gl_Position = vec4(position.xyz, 1.0);    \n"
    "    gl_Position.xy += pan;                    \n"
    "    gl_Position.xy *= zoom;                   \n"
    "    gl_Position.y *= aspect;                  \n"
    "    color = gl_Position.xyz + vec3(0.5);      \n"
    "}                                             \n";

// Fragment/pixel shader
const GLchar* fragmentSource =
    "precision mediump float;                     \n"
    "varying vec3 color;                          \n"
    "void main()                                  \n"
    "{                                            \n"
    "    gl_FragColor = vec4 ( color, 1.0 );      \n"
    "}                                            \n";

void updateShader(EventHandler& eventHandler)
{
    Camera& camera = eventHandler.camera();

    glUniform2fv(shaderPan, 1, camera.pan());
    glUniform1f(shaderZoom, camera.zoom()); 
    glUniform1f(shaderAspect, camera.aspect());
}

GLuint initShader(EventHandler& eventHandler)
{
    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link vertex and fragment shader into shader program and use it
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Get shader variables and initialize them
    shaderPan = glGetUniformLocation(shaderProgram, "pan");
    shaderZoom = glGetUniformLocation(shaderProgram, "zoom");    
    shaderAspect = glGetUniformLocation(shaderProgram, "aspect");
    updateShader(eventHandler);

    return shaderProgram;
}

void initGeometry(GLuint shaderProgram)
{
    // Create vertex buffer object and copy vertex data into it
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLfloat vertices[] = 
    {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Specify the layout of the shader vertex data (positions only, 3 floats)
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void redraw(EventHandler& eventHandler)
{
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the vertex buffer
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap front/back framebuffers
    eventHandler.swapWindow();
}

int randomId;
float randomIncrement;

void logKey(int key)
{
    std::cout << "Wasm " << randomId << ": Keydown " << key << std::endl;
}

void zoomInOut(EventHandler& eventHandler, const float increment)
{
    static float counter = 50.0f;
    static bool zoomOut = false;

    if (zoomOut && (counter -= increment) <= 50.0f)
        zoomOut = false;
    else if (!zoomOut && (counter += increment) >= 150.0f)
        zoomOut = true;
    
    eventHandler.camera().setZoom(counter / 100.0f);
}

void mainLoop(void* mainLoopArg) 
{
    // FPS timer: https://stackoverflow.com/a/61784821
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps = 0; fps++;

    {
        // Processing
        EventHandler& eventHandler = *((EventHandler*)mainLoopArg);
        eventHandler.processEvents();

        zoomInOut(eventHandler, randomIncrement);

        // Update shader if camera changed
        if (eventHandler.camera().updated())
            updateShader(eventHandler);

        redraw(eventHandler);
    }

    static int counter = 0; counter++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) {
        // Output log
        std::cout << "Wasm " << randomId << ": "
            << (fps-1) << " FPS, " << counter << " Frames" << std::endl;

        // Reset FPS timer
        oldTime = std::chrono::high_resolution_clock::now();
        fps = 0;
        
    }
}

int main(int argc, char** argv)
{
    EventHandler eventHandler("Hello Triangle");

    // Initialize shader and geometry
    GLuint shaderProgram = initShader(eventHandler);
    initGeometry(shaderProgram);

    // Start the main loop
    void* mainLoopArg = &eventHandler;

    // Seed random generator
    uint64_t microseconds_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::srand(microseconds_since_epoch);

    randomId = std::rand() % 900 + 100;
    randomIncrement = static_cast<float>(std::rand() % 400 + 200) / 100.0f;

    // Write test file (invokes FS)
    std::ofstream myFile("testFile.txt");
    myFile << "Test file! Wasm Instance: " << randomId << std::endl;
    myFile.close();

#ifdef __EMSCRIPTEN__
    int fps = 0; // Use browser's requestAnimationFrame
    emscripten_set_main_loop_arg(mainLoop, mainLoopArg, fps, true);
#else
    while(true) 
        mainLoop(mainLoopArg);
#endif

    return 0;
}