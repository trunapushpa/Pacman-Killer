#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float radius, color_t color);
    glm::vec3 position;
    double gravity = 0,vertspeed = 0;
    bool movement = true;
    float rotation;
    bool inwater = false;
    float radius;
    long score=0;
    bool inporc = false;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
