#pragma once

#include "ModelGeometry.h"

#include <glad/glad.h>


namespace Hildur {


    class View {

    public:

        ~View();

    private:

        //Previous code

        class SceneLoader {

        private:
            View& view;
            std::vector<float> rectangle_data_vector;
            void loadAllRectangles();
            void loadBlock(const Block& block);
            void loadRectangle(const Rectangle& rectangle);
            void loadRectangleVertex(const int& vertex_number, const Rectangle& rectangle);
        public:
            GLuint number_of_vertices_in_scene;
            SceneLoader(View& view);
        };
        SceneLoader* scene_loader;

    }


}