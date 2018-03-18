#include "main.h"

#ifndef PACMAN_KILLER_ENEMY_H
#define PACMAN_KILLER_ENEMY_H


class Enemy {
public:
    Enemy() {}
    Enemy(float x, float y, float radius, float speed, int plank, color_t color);
    glm::vec3 position;
    bool movement = true;
    float rotation;
    bool inwater = false;
    float radius;
    int plank;
    color_t color;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object2;
};


#endif //PACMAN_KILLER_ENEMY_H
