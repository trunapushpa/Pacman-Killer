#include "porc.h"
#include "main.h"

Porc::Porc(float x, float y, float h, float w, int n, float b0, float b1, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->n = n;
    this->width = w;
    this->height = h;
    this->boundary0 = b0;
    this->boundary1 = b1;
    this->speed = -0.01;
    GLfloat vertex_buffer_data[n*9];
    for (int i = 0; i < n; ++i) {
        vertex_buffer_data[9*i] = i*w;
        vertex_buffer_data[9*i+1] = 0;
        vertex_buffer_data[9*i+2] = 0;
        vertex_buffer_data[9*i+3] = (i+1)*w;
        vertex_buffer_data[9*i+4] = 0;
        vertex_buffer_data[9*i+5] = 0;
        vertex_buffer_data[9*i+6] = ((2*i+1)*w)/2;
        vertex_buffer_data[9*i+7] = h;
        vertex_buffer_data[9*i+8] = 0;
    }
    this->object = create3DObject(GL_TRIANGLES, 3*n, vertex_buffer_data, color, GL_FILL);
}

void Porc::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porc::tick() {
    this->position.x += this->speed;
    if (this->position.x + n*this->width > boundary1 || this->position.x < boundary0){
        this->speed = -this->speed;
    }
}

void Porc::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
