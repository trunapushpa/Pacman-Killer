#include "main.h"

#ifndef PACMAN_KILLER_GROUND_H
#define PACMAN_KILLER_GROUND_H


class Ground {
public:
    Ground() {}
    Ground(float x, float y, float height, float width, color_t color);
    glm::vec3 position;
    bool movement = true;
    float rotation;
    float height,width;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *object;
};


#endif //PACMAN_KILLER_GROUND_H
