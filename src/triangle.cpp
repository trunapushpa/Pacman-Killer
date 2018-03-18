#include "triangle.h"
#include "main.h"

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, color_t color) {
    this->position = glm::vec3(x1, y1, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[9]={
            0,0,0,
            x2-x1,y2-y1,0,
            x3-x1,y3-y1,0
    };

    this->object = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color, GL_FILL);
}

void Triangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Triangle::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Triangle::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
