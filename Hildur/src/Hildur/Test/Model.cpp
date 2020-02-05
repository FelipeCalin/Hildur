#include "hrpcheaders.h"
#include "Model.h"


namespace Hildur {


    Model::Model() :
        background_color(std::vector<double>{0, 0, 0, 1}),
        scene_floor(Rectangle(std::vector<glm::vec3>{
        glm::vec3(552.8f, 0, 0), glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 559.2f), glm::vec3(549.6f, 0, 559.2f)}, glm::vec3(1.0f, 1.0f, 1.0f))),
        scene_ceiling(Rectangle(std::vector<glm::vec3>{
            glm::vec3(556.0f, 548.8f, 0), glm::vec3(556.0f, 548.8f, 559.2f),
                glm::vec3(0, 548.8f, 559.2f), glm::vec3(0, 548.8f, 0)}, glm::vec3(1.0f, 1.0f, 1.0f))),
            scene_back_wall(Rectangle(std::vector<glm::vec3>{
                glm::vec3(549.6f, 0, 559.2f), glm::vec3(0, 0, 559.2f),
                    glm::vec3(0, 548.8f, 559.2f), glm::vec3(556.0f, 548.8f, 559.2f)}, glm::vec3(1.0f, 1.0f, 1.0f))),
                scene_right_wall(Rectangle(std::vector<glm::vec3>{
                    glm::vec3(0, 0, 559.2f), glm::vec3(0, 0, 0),
                        glm::vec3(0, 548.8f, 0), glm::vec3(0, 548.8f, 559.2f)}, glm::vec3(0, 1.0f, 0))),
                    scene_left_wall(Rectangle(std::vector<glm::vec3>{
                        glm::vec3(552.8f, 0, 0), glm::vec3(549.6f, 0, 559.2f),
                            glm::vec3(556.0f, 548.8f, 559.2f), glm::vec3(556.0f, 548.8f, 0)}, glm::vec3(1.0f, 0, 0))),
                        short_block(Block(std::vector<Rectangle>{
                            Rectangle(std::vector<glm::vec3>{
                                glm::vec3(130.0f, 165.0f, 65.0f), glm::vec3(82.0f, 165.0f, 225.0f),
                                    glm::vec3(240.0f, 165.0f, 272.0f), glm::vec3(290.0f, 165.0f, 114.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                Rectangle(std::vector<glm::vec3>{
                                    glm::vec3(290.0f, 0, 114.0f), glm::vec3(290.0f, 165.0f, 114.0f),
                                        glm::vec3(240.0f, 165.0f, 272.0f), glm::vec3(240.0f, 0, 272.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                    Rectangle(std::vector<glm::vec3>{
                                        glm::vec3(130.0f, 0, 65.0f), glm::vec3(130.0f, 165.0f, 65.0f),
                                            glm::vec3(290.0f, 165.0f, 114.0f), glm::vec3(290.0f, 0, 114.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                        Rectangle(std::vector<glm::vec3>{
                                            glm::vec3(82.0f, 0, 225.0f), glm::vec3(82.0f, 165.0f, 225.0f),
                                                glm::vec3(130.0f, 165.0f, 65.0f), glm::vec3(130.0f, 0, 65.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                            Rectangle(std::vector<glm::vec3>{
                                                glm::vec3(240.0f, 0, 272.0f), glm::vec3(240.0f, 165.0f, 272.0f),
                                                    glm::vec3(82.0f, 165.0f, 225.0f), glm::vec3(82.0f, 0, 225.0f)}, glm::vec3(1.0f, 1.0f, 1.0f))
                        })),
                            tall_block(Block(std::vector<Rectangle>{
                                                    Rectangle(std::vector<glm::vec3>{
                                                        glm::vec3(423.0f, 330.0f, 247.0f), glm::vec3(265.0f, 330.0f, 296.0f),
                                                            glm::vec3(314.0f, 330.0f, 456.0f), glm::vec3(472.0f, 330.0f, 406.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                                        Rectangle(std::vector<glm::vec3>{
                                                            glm::vec3(423.0f, 0, 247.0f), glm::vec3(423.0f, 330.0f, 247.0f),
                                                                glm::vec3(472.0f, 330.0f, 406.0f), glm::vec3(472.0f, 0, 406.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                                            Rectangle(std::vector<glm::vec3>{
                                                                glm::vec3(472.0f, 0, 406.0f), glm::vec3(472.0f, 330.0f, 406.0f),
                                                                    glm::vec3(314.0f, 330.0f, 456.0f), glm::vec3(314.0f, 0, 456.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                                                Rectangle(std::vector<glm::vec3>{
                                                                    glm::vec3(314.0f, 0, 456.0f), glm::vec3(314.0f, 330.0f, 456.0f),
                                                                        glm::vec3(265.0f, 330.0f, 296.0f), glm::vec3(265.0f, 0, 296.0f)}, glm::vec3(1.0f, 1.0f, 1.0f)),
                                                                    Rectangle(std::vector<glm::vec3>{
                                                                        glm::vec3(265.0f, 0, 296.0f), glm::vec3(265.0f, 330.0f, 296.0f),
                                                                            glm::vec3(423.0f, 330.0f, 247.0f), glm::vec3(423.0f, 0, 247.0f)}, glm::vec3(1.0f, 1.0f, 1.0f))
                                                })) {}


}