#pragma once

#include "ModelGeometry.h"


namespace Hildur {


    class Model {
    public:
        std::vector<double> background_color;
        Rectangle scene_left_wall;
        Rectangle scene_right_wall;
        Rectangle scene_back_wall;
        Rectangle scene_floor;
        Rectangle scene_ceiling;
        Box back_box;
        Box front_box;

        Model();
    };


}