#include "hrpcheaders.h"
#include "ModelGeometry.h"


namespace Hildur {


    Rectangle::Rectangle(std::vector<glm::vec3> rectangle_corners, glm::vec3 RGB_color) :
        rectangle_corners(rectangle_corners), RGB_color(RGB_color) {}

    const glm::vec3& Rectangle::operator[](const size_t& index) const {
        return rectangle_corners[index];
    }

    Block::Block(std::vector<Rectangle> block_sides) :block_sides(block_sides) {}


}