#include "water.h"
#include "main.h"

Water::Water(float x, float y, float radius, float rotation, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    this->radius = radius;
    this->gravity = 0;
    GLfloat vertex_buffer_data[100005];
    for (int i = 0; i <= 3142; ++i) {
        vertex_buffer_data[i*9] = 0.0f;
        vertex_buffer_data[i*9+1] = 0.0f;
        vertex_buffer_data[i*9+2] = 0.0f;
        if (i==0){
            vertex_buffer_data[i*9+3] = 0.0f;
            vertex_buffer_data[i*9+4] = radius;
            vertex_buffer_data[i*9+5] = 0.0f;
        }
        else{
            vertex_buffer_data[i*9+3] = vertex_buffer_data[(i-1)*9+6];
            vertex_buffer_data[i*9+4] = vertex_buffer_data[(i-1)*9+7];
            vertex_buffer_data[i*9+5] = vertex_buffer_data[(i-1)*9+8];
        }
        vertex_buffer_data[i*9+6] = (float)cos(-0.001)*vertex_buffer_data[i*9+3] - (float)sin(-0.001)*vertex_buffer_data[i*9+4];
        vertex_buffer_data[i*9+7] = (float)sin(-0.001)*vertex_buffer_data[i*9+3] + (float)cos(-0.001)*vertex_buffer_data[i*9+4];
        vertex_buffer_data[i*9+8] = 0.0f;
    }
    this->object = create3DObject(GL_TRIANGLES, 3142*3, vertex_buffer_data, color, GL_FILL);
}

void Water::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Water::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Water::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
