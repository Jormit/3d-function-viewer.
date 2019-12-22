#include "MyApplication.hpp"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

float outputFunc(glm::vec2 input) {
    //return 0.5 * sin(input.x * 10.0f) * sin(input.y * 10.0f);

    //return sin(10 * (input.x * input.x * 2 + input.y * input.y * 2))/10;

    return sin(5 * input.x * 2) * cos(5 * input.y * 2)/5;
}

Vertex getVertex(glm::vec2 input) {
    Vertex newVertex;
    const glm::vec2 dx(1.0 , 0.0);
    const glm::vec2 dy(0.0 , 1.0);

    float z = outputFunc(input);
    float zx = 100.0f * ( outputFunc(input + 0.01f * dx) - z);
    float zy = 100.0f * ( outputFunc(input + 0.01f * dy) - z);


    newVertex.position = glm::vec3(input, z);
    newVertex.normal = glm::normalize(glm::vec3(zx, zy, -1.0f));

    //printf("%f, %f, %f\n", newVertex.normal.x, newVertex.normal.y, newVertex.normal.z);

    return newVertex;
}

int size = 100;

// The main guts of application go here.
// -------------------------------------
MyApplication::MyApplication(int width, int height) :
    Application(width, height),
    shaderProgram ("./shaders/vert.glsl", "./shaders/frag.glsl")
{
    // Generate mesh vertices.
    vector<Vertex> vertices;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            vertices.push_back(getVertex(glm::vec2((float)x/(float)size, (float)y/(float)size)));
        }
    }

    // Create the indexes.
    vector<unsigned int>indices;
    for (int y = 0; y < size - 1; ++y) {
        for (int x = 0; x < size - 1; ++x) {
          indices.push_back((x + 0) + (size) * (y + 0));
          indices.push_back((x + 1) + (size) * (y + 0));
          indices.push_back((x + 1) + (size) * (y + 1));

          indices.push_back((x + 1) + (size) * (y + 1));
          indices.push_back((x + 0) + (size) * (y + 1));
          indices.push_back((x + 0) + (size) * (y + 0));
        }
    }

    std::cout << "vertices=" << vertices.size() << std::endl;
    std::cout << "index=" << indices.size() << std::endl;

    // Create vao, vbo, ebo.
    // ---------------------
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Bind to vertex array.
    glBindVertexArray(vao);

    // Copy vertices into vertex buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 2 * (sizeof(glm::vec3)), &vertices[0], GL_STATIC_DRAW);

    // Copy indices into element buffer object.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Position attribute.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture uniform for the shader to use.
    shaderProgram.use();

    // Projection transformation.
    // -------------------------------------------
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // View at center.
    glm::mat4 view = glm::lookAt(glm::vec3(-0.5f, -0.5f, 1.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int viewLoc = glGetUniformLocation(shaderProgram.shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 rotation = glm::mat4(1.0f);
    //rotation = glm::rotate(rotation, glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int modelLoc = glGetUniformLocation(shaderProgram.shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotation));

    loop();
}

// Main loop of the application.
// -----------------------------
void MyApplication::loop()
{
    while(!glfwWindowShouldClose(window))
    {
        // Handle delta time.
        // ------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process user Input.
        // --------------
        process_input();

        // Render the screen.
        // ------------------
        shaderProgram.use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, size * size * 2 * 3 , GL_UNSIGNED_INT, 0);

        // Flip buffers and clear z-buffer.
        // --------------------------------
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }
}

// Process keypress events.
// ------------------------
void MyApplication::process_input()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}


