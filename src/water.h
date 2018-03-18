#include "main.h"

#ifndef WATER_H
#define WATER_H


class Water {
public:
    Water() {}
    Water(float x, float y, float radius, float rotation, color_t color);
    glm::vec3 position;
    double gravity = 0;
    bool movement = true;
    float rotation=-90.0;
    float radius;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bounding_box_t bounding_box();
private:
    VAO *object;
};


#endif //PACMAN_KILLER_WATER_H
