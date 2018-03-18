#include "main.h"

#ifndef PACMAN_KILLER_PORC_H
#define PACMAN_KILLER_PORC_H


class Porc {
public:
    Porc() {}
    Porc(float x, float y, float h, float w, int n, float b0, float b1, color_t color);
    glm::vec3 position;
    float rotation,width,height,n,boundary0,boundary1,speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool present = false;
private:
    VAO *object;
};


#endif //PACMAN_KILLER_PORC_H
