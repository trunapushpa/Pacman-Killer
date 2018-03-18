#include "enemy.h"
#include "main.h"

float randomFInRange(float LO, float HI){
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}
int randomIInRange(int min, int max){
    return min + (rand() % static_cast<int>(max - min + 1));
}

Enemy::Enemy(float x, float y, float radius, float speed, int plank, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->radius = radius;
    this->speed = speed;
    this->color = color;
    this->plank = plank;
    GLfloat vertex_buffer_data[8005];
    for (int i = 0; i < 629; ++i) {
        vertex_buffer_data[i*9] = 0.0f;
        vertex_buffer_data[i*9+1] = 0.0f;
        vertex_buffer_data[i*9+2] = 0.0f;
        if (i==0){
            vertex_buffer_data[i*9+3] = 0.0f;
            vertex_buffer_data[i*9+4] = this->radius;
            vertex_buffer_data[i*9+5] = 0.0f;
        }
        else{
            vertex_buffer_data[i*9+3] = vertex_buffer_data[(i-1)*9+6];
            vertex_buffer_data[i*9+4] = vertex_buffer_data[(i-1)*9+7];
            vertex_buffer_data[i*9+5] = vertex_buffer_data[(i-1)*9+8];
        }
        vertex_buffer_data[i*9+6] = (float)cos(-0.01)*vertex_buffer_data[i*9+3] - (float)sin(-0.01)*vertex_buffer_data[i*9+4];
        vertex_buffer_data[i*9+7] = (float)sin(-0.01)*vertex_buffer_data[i*9+3] + (float)cos(-0.01)*vertex_buffer_data[i*9+4];
        vertex_buffer_data[i*9+8] = 0.0f;
    }
    this->object = create3DObject(GL_TRIANGLES, 629*3, vertex_buffer_data, this->color, GL_FILL);
    if (this->plank == 1){
        vertex_buffer_data[0] = -(sqrt(2)-0.1)*this->radius;
        vertex_buffer_data[1] = 0;
        vertex_buffer_data[2] = 0;
        vertex_buffer_data[3] = -(sqrt(2)-0.1)*this->radius - 0.05;
        vertex_buffer_data[4] = 0.05;
        vertex_buffer_data[5] = 0;
        vertex_buffer_data[6] = 0;
        vertex_buffer_data[7] = (sqrt(2)-0.1)*this->radius;
        vertex_buffer_data[8] = 0;

        vertex_buffer_data[9] = 0;
        vertex_buffer_data[10] = (sqrt(2)-0.1)*this->radius;
        vertex_buffer_data[11] = 0;
        vertex_buffer_data[12] = -0.05;
        vertex_buffer_data[13] = (sqrt(2)-0.1)*this->radius+0.05;
        vertex_buffer_data[14] = 0;
        vertex_buffer_data[15] = -(sqrt(2)-0.1)*this->radius-0.05;
        vertex_buffer_data[16] = 0.05;
        vertex_buffer_data[17] = 0;
    }
    else if (this->plank == 2){
        vertex_buffer_data[0] = (sqrt(2)-0.1)*this->radius;
        vertex_buffer_data[1] = 0;
        vertex_buffer_data[2] = 0;
        vertex_buffer_data[3] = (sqrt(2)-0.1)*this->radius + 0.05;
        vertex_buffer_data[4] = 0.05;
        vertex_buffer_data[5] = 0;
        vertex_buffer_data[6] = 0;
        vertex_buffer_data[7] = (sqrt(2)-0.1)*this->radius;
        vertex_buffer_data[8] = 0;

        vertex_buffer_data[9] = 0;
        vertex_buffer_data[10] = (sqrt(2)-0.1)*this->radius;
        vertex_buffer_data[11] = 0;
        vertex_buffer_data[12] = 0.05;
        vertex_buffer_data[13] = (sqrt(2)-0.1)*this->radius+0.05;
        vertex_buffer_data[14] = 0;
        vertex_buffer_data[15] = +(sqrt(2)-0.1)*this->radius+0.05;
        vertex_buffer_data[16] = 0.05;
        vertex_buffer_data[17] = 0;
    }
    else {
        vertex_buffer_data[0] = 0;
        vertex_buffer_data[1] = 0;
        vertex_buffer_data[2] = 0;
        vertex_buffer_data[3] = 0;
        vertex_buffer_data[4] = 0;
        vertex_buffer_data[5] = 0;
        vertex_buffer_data[6] = 0;
        vertex_buffer_data[7] = 0;
        vertex_buffer_data[8] = 0;

        vertex_buffer_data[9] = 0;
        vertex_buffer_data[10] = 0;
        vertex_buffer_data[11] = 0;
        vertex_buffer_data[12] = 0;
        vertex_buffer_data[13] = 0;
        vertex_buffer_data[14] = 0;
        vertex_buffer_data[15] = 0;
        vertex_buffer_data[16] = 0;
        vertex_buffer_data[17] = 0;
    }
    this->object2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Enemy::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object2);
}

void Enemy::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Enemy::tick() {
    if (this->movement){
        this->position.x += speed;
        if (this->position.x>4.3){
            this->movement = false;
        }
    }
    else{
        this->movement = true;
        int col = randomIInRange(0,4);
        this->position.x = -4.3;
        this->position.y = randomFInRange(-1.5,3);
        this->radius = randomFInRange(0.15,0.3);
        this->speed = randomFInRange(0.01,0.04);
        switch (col){
            case 0:
                this->color = COLOR_DRED;
                break;
            case 1:
                this->color = COLOR_GREEN;
                break;
            case 2:
                this->color = COLOR_PURPLE;
                break;
            case 3:
                this->color = COLOR_DBLUE;
                break;
            default:
                this->color = COLOR_YELLOW;
        }
        GLfloat vertex_buffer_data[8005];
        for (int i = 0; i <= 629; ++i) {
            vertex_buffer_data[i*9] = 0.0f;
            vertex_buffer_data[i*9+1] = 0.0f;
            vertex_buffer_data[i*9+2] = 0.0f;
            if (i==0){
                vertex_buffer_data[i*9+3] = 0.0f;
                vertex_buffer_data[i*9+4] = this->radius;
                vertex_buffer_data[i*9+5] = 0.0f;
            }
            else{
                vertex_buffer_data[i*9+3] = vertex_buffer_data[(i-1)*9+6];
                vertex_buffer_data[i*9+4] = vertex_buffer_data[(i-1)*9+7];
                vertex_buffer_data[i*9+5] = vertex_buffer_data[(i-1)*9+8];
            }
            vertex_buffer_data[i*9+6] = (float)cos(-0.01)*vertex_buffer_data[i*9+3] - (float)sin(-0.01)*vertex_buffer_data[i*9+4];
            vertex_buffer_data[i*9+7] = (float)sin(-0.01)*vertex_buffer_data[i*9+3] + (float)cos(-0.01)*vertex_buffer_data[i*9+4];
            vertex_buffer_data[i*9+8] = 0.0f;
        }
        this->object = create3DObject(GL_TRIANGLES, 629*3, vertex_buffer_data, this->color, GL_FILL);
        if (this->plank == 1){
            vertex_buffer_data[0] = -(sqrt(2)-0.1)*this->radius;
            vertex_buffer_data[1] = 0;
            vertex_buffer_data[2] = 0;
            vertex_buffer_data[3] = -(sqrt(2)-0.1)*this->radius - 0.05;
            vertex_buffer_data[4] = 0.05;
            vertex_buffer_data[5] = 0;
            vertex_buffer_data[6] = 0;
            vertex_buffer_data[7] = (sqrt(2)-0.1)*this->radius;
            vertex_buffer_data[8] = 0;

            vertex_buffer_data[9] = 0;
            vertex_buffer_data[10] = (sqrt(2)-0.1)*this->radius;
            vertex_buffer_data[11] = 0;
            vertex_buffer_data[12] = -0.05;
            vertex_buffer_data[13] = (sqrt(2)-0.1)*this->radius+0.05;
            vertex_buffer_data[14] = 0;
            vertex_buffer_data[15] = -(sqrt(2)-0.1)*this->radius-0.05;
            vertex_buffer_data[16] = 0.05;
            vertex_buffer_data[17] = 0;
        }
        else if (this->plank == 2){
            vertex_buffer_data[0] = (sqrt(2)-0.1)*this->radius;
            vertex_buffer_data[1] = 0;
            vertex_buffer_data[2] = 0;
            vertex_buffer_data[3] = (sqrt(2)-0.1)*this->radius + 0.05;
            vertex_buffer_data[4] = 0.05;
            vertex_buffer_data[5] = 0;
            vertex_buffer_data[6] = 0;
            vertex_buffer_data[7] = (sqrt(2)-0.1)*this->radius;
            vertex_buffer_data[8] = 0;

            vertex_buffer_data[9] = 0;
            vertex_buffer_data[10] = (sqrt(2)-0.1)*this->radius;
            vertex_buffer_data[11] = 0;
            vertex_buffer_data[12] = 0.05;
            vertex_buffer_data[13] = (sqrt(2)-0.1)*this->radius+0.05;
            vertex_buffer_data[14] = 0;
            vertex_buffer_data[15] = +(sqrt(2)-0.1)*this->radius+0.05;
            vertex_buffer_data[16] = 0.05;
            vertex_buffer_data[17] = 0;
        }
        else {
            vertex_buffer_data[0] = 0;
            vertex_buffer_data[1] = 0;
            vertex_buffer_data[2] = 0;
            vertex_buffer_data[3] = 0;
            vertex_buffer_data[4] = 0;
            vertex_buffer_data[5] = 0;
            vertex_buffer_data[6] = 0;
            vertex_buffer_data[7] = 0;
            vertex_buffer_data[8] = 0;

            vertex_buffer_data[9] = 0;
            vertex_buffer_data[10] = 0;
            vertex_buffer_data[11] = 0;
            vertex_buffer_data[12] = 0;
            vertex_buffer_data[13] = 0;
            vertex_buffer_data[14] = 0;
            vertex_buffer_data[15] = 0;
            vertex_buffer_data[16] = 0;
            vertex_buffer_data[17] = 0;
        }
        this->object2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_BLACK, GL_FILL);
    }
}

bounding_box_t Enemy::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
