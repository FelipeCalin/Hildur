#include "hrpcheaders.h"
#include "View.h"


namespace Hildur {


    void View::SceneLoader::loadAllRectangles() {
        loadRectangle(view.model.scene_floor);
        loadRectangle(view.model.scene_ceiling);
        loadRectangle(view.model.scene_back_wall);
        loadRectangle(view.model.scene_right_wall);
        loadRectangle(view.model.scene_left_wall);
        loadBlock(view.model.tall_block);
        loadBlock(view.model.short_block);
    }

    void View::SceneLoader::loadBlock(const Block& block) {
        for (size_t i = 0; i < block.block_sides.size(); i++) {
            loadRectangle(block.block_sides[i]);
        }
    }

    void View::SceneLoader::loadRectangle(const Rectangle& rectangle) {
        loadRectangleVertex(0, rectangle);
        loadRectangleVertex(1, rectangle);
        loadRectangleVertex(2, rectangle);
        loadRectangleVertex(0, rectangle);
        loadRectangleVertex(2, rectangle);
        loadRectangleVertex(3, rectangle);
    }

    void View::SceneLoader::loadRectangleVertex(const int& vertex_number, const Rectangle& rectangle) {
        for (size_t j = 0; j < 3; j++) {
            rectangle_data_vector.push_back(rectangle[vertex_number][j]);
        }
        for (size_t i = 0; i < 3; i++) {
            rectangle_data_vector.push_back(rectangle.RGB_color[i]);
        }
    }

    View::SceneLoader::SceneLoader(View& view) :view(view) {

        loadAllRectangles();
        float* rectangle_data_array = new float[rectangle_data_vector.size()];
        for (size_t i = 0; i < rectangle_data_vector.size(); i++) {
            rectangle_data_array[i] = rectangle_data_vector[i];
        }

        number_of_vertices_in_scene = rectangle_data_vector.size() / 2; //half of the data is color data

        GLuint vertex_array_object;
        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        GLuint rectangle_buffer;
        glGenBuffers(1, &rectangle_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, rectangle_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * rectangle_data_vector.size(), rectangle_data_array, GL_STATIC_DRAW);

        delete[] rectangle_data_array;

        GLint vertex_position_attrib = glGetAttribLocation(view.shader_program, "vertex_position");
        glVertexAttribPointer(vertex_position_attrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        glEnableVertexAttribArray(vertex_position_attrib);

        GLint texture_coordinate_attrib = glGetAttribLocation(view.shader_program, "color_input");
        glVertexAttribPointer(texture_coordinate_attrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
            (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(texture_coordinate_attrib);
    }

    View::View(Model& model) :model(model) {
        glEnable(GL_DEPTH_TEST);

        vertex_shader = LoadShader(std::string("GLSL/vertex_shader.txt"), GL_VERTEX_SHADER);
        fragment_shader = LoadShader(std::string("GLSL/fragment_shader.txt"), GL_FRAGMENT_SHADER);

        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);

        glBindFragDataLocation(shader_program, 0, "color_out");
        glAttachShader(shader_program, fragment_shader);

        glLinkProgram(shader_program);
        CheckLinkStatus(shader_program);

        glDetachShader(shader_program, vertex_shader);
        glDetachShader(shader_program, fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        glUseProgram(shader_program);

        mvp_uniform_attribute = glGetUniformLocation(shader_program, "mvp");

        scene_loader = new SceneLoader(*this);
    }

    View::~View() {
        delete scene_loader;
    }

    void View::render(SDL_Window* window) {
        glClearColor(model.background_color[0],
            model.background_color[1], model.background_color[2], model.background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updateViewMatrix();

        glm::mat4 model_view_projection = camera.projection_matrix * camera.view_matrix;

        glUniformMatrix4fv(mvp_uniform_attribute, 1, GL_FALSE, &model_view_projection[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, scene_loader->number_of_vertices_in_scene);

        glFlush();

        SDL_GL_SwapWindow(window);
    }


}