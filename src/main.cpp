#include "main.h"
#include "timer.h"
#include "ball.h"
#include "water.h"
#include "ground.h"
#include "triangle.h"
#include "enemy.h"
#include "porc.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1, ball2;
Enemy enemy[15];
Water water1, trampoline, magnetCO, magnetCI, magnetCO2, magnetCI2;
Ground bigGround, smallGround, trampbar1, trampbar2, magnetBar1, magnetBar2, magnetN, magnetS, magnetBar12, magnetBar22, magnetN2, magnetS2;
Triangle plank1, plank2;
Porc porc1, porc2;
bool magnet1 = false, magnet2 = false, porc1s = false, porc2s = false, drag = false;
int mtimeout,porc1t,porc2t;

Mix_Music* pop;
Mix_Music* splashs;
Mix_Music* hurts;
Mix_Music* jumps;

double cxpos, pxpos, cypos, pypos;

float randomfInRange(float LO, float HI) {
    return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

int randomiInRange(int min, int max) {
    return min + (rand() % static_cast<int>(max - min + 1));
}

void dispScore(){
//    cout << ball2.score << endl;
    string e = "Pacman-Killer | Score : ";
    std::string s = std::to_string(ball2.score);
    s = e+s;
    glfwSetWindowTitle(window, const_cast<char*>(s.c_str()));
}

void splash(){
    if (splashs)
    {
        // Start Playback
        if (Mix_PlayMusic(splashs, 1) != 0){
            std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
    }
}

void hurt(){
    if (hurts)
    {
        // Start Playback
        if (Mix_PlayMusic(hurts, 1) != 0){
            std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
    }
}

void jump(){
    if (jumps)
    {
        // Start Playback
        if (Mix_PlayMusic(jumps, 1) != 0){
            std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
    }
}

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0),
                                glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
//    ball1.draw(VP);
    bigGround.draw(VP);
    smallGround.draw(VP);
    water1.draw(VP);

    if (ball2.score >= 500) {
        int rand = randomiInRange(0, 20);
        mtimeout--;
        if (!magnet2 && !magnet1 && mtimeout <= 0) {
            if (rand == 1) {
                magnet1 = true;
                mtimeout = 480;
            } else if (rand == 2) {
                magnet2 = true;
                mtimeout = 480;
            }
        }
        if (magnet1) {
            magnetCO.draw(VP);
            magnetCI.draw(VP);
            magnetBar1.draw(VP);
            magnetBar2.draw(VP);
            magnetN.draw(VP);
            magnetS.draw(VP);
            if (mtimeout <= 0) {
                magnet1 = false;
                mtimeout = 480;
            }
        }
        if (magnet2) {
            magnetCO2.draw(VP);
            magnetCI2.draw(VP);
            magnetBar12.draw(VP);
            magnetBar22.draw(VP);
            magnetN2.draw(VP);
            magnetS2.draw(VP);
            if (mtimeout <= 0) {
                magnet2 = false;
                mtimeout = 480;
            }
        }
    }
    trampbar1.draw(VP);
    trampbar2.draw(VP);
    trampoline.draw(VP);
//    plank1.draw(VP);
//    plank2.draw(VP);
    for (int i = 0; i < 15; ++i) {
        enemy[i].draw(VP);
    }
    ball2.draw(VP);
    if (porc1.present)
        porc1.draw(VP);
    if (porc2.present)
        porc2.draw(VP);
}

void tick_input(GLFWwindow *window) {
    glfwGetCursorPos(window, &cxpos, &cypos);
    int left = glfwGetKey(window, GLFW_KEY_A);
    int right = glfwGetKey(window, GLFW_KEY_D);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    int screenl = glfwGetKey(window, GLFW_KEY_LEFT);
    int screenr = glfwGetKey(window, GLFW_KEY_RIGHT);
    int screenu = glfwGetKey(window, GLFW_KEY_UP);
    int screend = glfwGetKey(window, GLFW_KEY_DOWN);
    if (cxpos < pxpos && drag) {
        left = 1;
    } else if (cxpos > pxpos && drag) {
        right = 1;
    }
    pxpos = cxpos;
    pypos = cypos;
    if (screenl) {
        screen_center_x -= 0.01;
        reset_screen();
    }
    if (screenr) {
        screen_center_x += 0.01;
        reset_screen();
    }
    if (screenu) {
        screen_center_y += 0.01;
        reset_screen();
    }
    if (screend) {
        screen_center_y -= 0.01;
        reset_screen();
    }
//    ball2.speed = -0.01;
    if (ball2.speed > 0 && !left && ball2.vertspeed == 0 && (!ball2.inwater || abs(ball2.position.x) <= 0.05)) {
        ball2.speed = 0;
    } else if (ball2.speed > 0 && !left && ball2.inwater && ball2.position.x < 0 &&
               (abs(sqrt(ball2.position.x * ball2.position.x + (ball2.position.y + 2) * (ball2.position.y + 2)) -
                    0.8)) <= 0.02) {
        ball2.speed = -0.015;
    }
    if (left) {
        if (ball2.inwater)
            ball2.speed = 0.015;
            // Do something
        else
            ball2.speed = 0.035;
    }
    if (ball2.speed < 0 && !right && ball2.vertspeed == 0 && (!ball2.inwater || abs(ball2.position.x) <= 0.05)) {
        ball2.speed = 0;
    } else if (ball2.speed < 0 && !right && ball2.inwater && ball2.position.x > 0 &&
               (abs(sqrt(ball2.position.x * ball2.position.x + (ball2.position.y + 2) * (ball2.position.y + 2)) -
                    0.8)) <= 0.02) {
        ball2.speed = +0.015;
    }
    if (ball2.speed == 0 && ball2.inwater && !left && !right &&
        (abs(sqrt(ball2.position.x * ball2.position.x + (ball2.position.y + 2) * (ball2.position.y + 2)) - 0.8))) {
        if (ball2.position.x < -0.05) {
            ball2.speed = -0.015;
        }
        if (ball2.position.x > 0.05) {
            ball2.speed = +0.015;
        }
    }
    if (right) {
        if (ball2.inwater)
            ball2.speed = -0.015;
        else
            ball2.speed = -0.035;
    }
    if (ball2.movement && ball2.vertspeed == 0) {
        if (up) {
            if (!ball2.inwater) {
                if (ball2.gravity == 0) {
                    ball2.gravity = -0.0045;
                    ball2.vertspeed = 0.12;
                }
            } else {
//                if (ball2.gravity == 0) {
                ball2.gravity = -0.0015;
                ball2.vertspeed = 0.052;
//                }
            }
        }
    }
}

void tick_elements() {
    if (ball2.score >= 1000) {
        if (!porc1s) {
            porc1s = true;
            porc1.present = true;
        }
        if (!porc2s) {
            porc2s = true;
            porc2.present = true;
        }
    }
    if (porc1s && porc1t<=0 && !porc1.present){
        porc1.present = true;
    }
    else if (porc1s && porc1t>0 && !porc1.present){
        porc1t--;
    }
    if (porc2s && porc2t<=0 && !porc2.present){
        porc2.present = true;
    }
    else if (porc2s && porc2t>0 && !porc2.present){
        porc2t--;
    }
//    ball1.tick();
    if (magnet1 && ball2.score >= 500 && ball2.position.y > 2.1) {
        ball2.speed += 0.005;
    }
    if (magnet2 && ball2.score >= 500 && ball2.position.y > 2.1) {
        ball2.speed -= 0.005;
    }
    ball2.tick();
    for (int i = 0; i < 15; ++i) {
        enemy[i].tick();
        if (enemy[i].plank == 1) {
            if (ball2.vertspeed <= 0 && ball2.position.x >= (enemy[i].position.x - ((sqrt(2)) * enemy[i].radius + ball2.radius)) &&
                ball2.position.x <= enemy[i].position.x+ball2.radius && abs(ball2.position.y-(enemy[i].position.y+sqrt(2)*enemy[i].radius+ball2.position.x-enemy[i].position.x))<=ball2.radius + 0.05) {
                ball2.vertspeed = 0.10;
                ball2.speed = 0.10;
            }
        }
        if (enemy[i].plank == 2) {
            if (ball2.vertspeed <= 0 && ball2.position.x >= enemy[i].position.x-ball2.radius &&
                ball2.position.x <= (enemy[i].position.x + ((sqrt(2)) * enemy[i].radius + ball2.radius)) && abs(ball2.position.y-(enemy[i].position.y+sqrt(2)*enemy[i].radius-ball2.position.x+enemy[i].position.x)) <= ball2.radius + 0.05) {
                ball2.vertspeed = 0.10;
                ball2.speed = -0.10;
            }
        }
        if (ball2.vertspeed <= 0 && ball2.position.y > enemy[i].position.y &&
            (sqrt((ball2.position.x - enemy[i].position.x) * (ball2.position.x - enemy[i].position.x) +
                  (ball2.position.y - enemy[i].position.y) * (ball2.position.y - enemy[i].position.y)) <
             0.2 + enemy[i].radius)) {
            enemy[i].movement = false;
            ball2.vertspeed = 0.12;
            if (enemy[i].color.r >= 200)
                ball2.score += 50;
            else if (enemy[i].color.g >= 200)
                ball2.score += 40;
            else if (enemy[i].color.r == 160)
                ball2.score += 30;
            else if (enemy[i].color.b == 200)
                ball2.score += 20;
            else
                ball2.score += 10;
            if(enemy[i].plank)
                ball2.score += 20;
            if (pop)
            {
                // Start Playback
                if (Mix_PlayMusic(pop, 1) != 0){
                    std::cerr << "Mix_PlayMusic ERROR: " << Mix_GetError() << std::endl;
                }
            }
            else
            {
                std::cerr << "Mix_LoadMuS ERROR: " << Mix_GetError() << std::endl;
            }
            dispScore();
        }
    }
    if (porc1.present)
        porc1.tick();
    if (porc2.present)
        porc2.tick();
    if (porc1.present &&
        (ball2.position.x >= porc1.position.x && ball2.position.x <= porc1.position.x + porc1.n * porc1.width) &&
        (abs(ball2.position.y - porc1.position.y) <= 0.4)) {
        if (!ball2.inporc) {
            ball2.score -= 500;
            hurt();
            dispScore();
        }
        ball2.inporc = true;
        porc1.present = false;
        porc1t = 480;
    } else if (porc2.present &&
               (ball2.position.x >= porc2.position.x && ball2.position.x <= porc2.position.x + porc2.n * porc2.width) &&
               (abs(ball2.position.y - porc2.position.y) <= 0.4)) {
        if (!ball2.inporc) {
            ball2.score -= 500;
            hurt();
            dispScore();
        }
        ball2.inporc = true;
        porc2.present = false;
        porc2t = 480;
    } else {
        if (ball2.inporc)
            ball2.inporc = false;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

//    ball1       = Ball(2, 0, 0.2, COLOR_RED);
    bigGround = Ground(-40, -4, 2, 80, COLOR_BROWN);
    smallGround = Ground(-40, -2.5, 0.5, 80, COLOR_DGREEN);
    trampbar1 = Ground(2.75, -2, 1, 0.05, COLOR_BROWN);
    trampbar2 = Ground(3.75, -2, 1, 0.05, COLOR_BROWN);
    trampoline = Water(3.25 + 0.025, -2 + 1, 0.5, -90, COLOR_RED);
//    smallPlank1 = Ground(-0.58,-1.25,0.5,0.2,COLOR_BROWN);
    plank1 = Triangle(-0.8, -2, -1, -2, -1, -2.1, COLOR_DGREEN);
    plank2 = Triangle(0.8, -2, 1, -2, 1, -2.1, COLOR_DGREEN);
    water1 = Water(0, -2, 1, -90, COLOR_BLUE);

    magnetCO = Water(-3, 3, 0.7, 180, COLOR_RED);
    magnetCI = Water(-3, 3, 0.3, 180, COLOR_BACKGROUND);
    magnetBar1 = Ground(-3, 3 - 0.7, 0.4, 0.5, COLOR_RED);
    magnetBar2 = Ground(-3, 3 + 0.3, 0.4, 0.5, COLOR_RED);
    magnetN = Ground(-3 + 0.5, 3 - 0.7, 0.4, 0.35, COLOR_GREY);
    magnetS = Ground(-3 + 0.5, 3 + 0.3, 0.4, 0.35, COLOR_GREY);

    magnetCO2 = Water(3, 3, 0.7, 0, COLOR_RED);
    magnetCI2 = Water(3, 3, 0.3, 0, COLOR_BACKGROUND);
    magnetBar12 = Ground(3 - 0.5, 3 - 0.7, 0.4, 0.5, COLOR_RED);
    magnetBar22 = Ground(3 - 0.5, 3 + 0.3, 0.4, 0.5, COLOR_RED);
    magnetN2 = Ground(3 - 0.4 - 0.4, 3 - 0.7, 0.4, 0.35, COLOR_GREY);
    magnetS2 = Ground(3 - 0.4 - 0.4, 3 + 0.3, 0.4, 0.35, COLOR_GREY);

    porc1 = Porc(-3.9, -2, 0.3, 0.3, 5, -3.9, -1.2, COLOR_GREY);
    porc2 = Porc(1.5, -2, 0.3, 0.3, 3, 1.1, 2.75, COLOR_GREY);

//    ball2 = Ball(-1.5, -2 + 0.2, 0.2, COLOR_BLACK);
    ball2 = Ball(0, -2 + 0.2, 0.2, COLOR_BLACK);
    ball2.speed = -ball2.speed;
    for (int i = 0; i < 15; ++i) {
        int col = randomiInRange(0, 4);
        switch (col) {
            case 0:
                enemy[i] = Enemy(randomfInRange(-3.75, 2.5), randomfInRange(3, -1.5), randomfInRange(0.15, 0.3),
                                 randomfInRange(0.01, 0.04), ((i / 2 == 0) ? 1 : ((i / 4 == 0) ? 2 : 0)), COLOR_DRED);
                break;
            case 1:
                enemy[i] = Enemy(randomfInRange(-3.75, 2.5), randomfInRange(3, -1.5), randomfInRange(0.15, 0.3),
                                 randomfInRange(0.01, 0.04), ((i / 2 == 0) ? 1 : ((i / 4 == 0) ? 2 : 0)), COLOR_GREEN);
                break;
            case 2:
                enemy[i] = Enemy(randomfInRange(-3.75, 2.5), randomfInRange(3, -1.5), randomfInRange(0.15, 0.3),
                                 randomfInRange(0.01, 0.04), ((i / 2 == 0) ? 1 : ((i / 4 == 0) ? 2 : 0)), COLOR_PURPLE);
                break;
            case 3:
                enemy[i] = Enemy(randomfInRange(-3.75, 2.5), randomfInRange(3, -1.5), randomfInRange(0.15, 0.3),
                                 randomfInRange(0.01, 0.04), ((i / 2 == 0) ? 1 : ((i / 4 == 0) ? 2 : 0)), COLOR_DBLUE);
                break;
            default:
                enemy[i] = Enemy(randomfInRange(-3.75, 2.5), randomfInRange(3, -1.5), randomfInRange(0.15, 0.3),
                                 randomfInRange(0.01, 0.04), ((i / 2 == 0) ? 1 : ((i / 4 == 0) ? 2 : 0)), COLOR_YELLOW);
        }
    }

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0,
                 0.0f); // R, G, B, A
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width = 1000;
    int height = 1000;

    window = initGLFW(width, height);

    glfwGetCursorPos(window, &pxpos, &pypos);
    glfwSetScrollCallback(window, scroll_callback);

    initGL(window, width, height);

    // Init
    if (SDL_Init(SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "SDL_Init ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Open Audio device
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) != 0)
    {
        std::cerr << "Mix_OpenAudio ERROR: " << Mix_GetError() << std::endl;
        return -1;
    }

    // Set Volume
    Mix_VolumeMusic(100);

    // Open Audio Files
    pop = Mix_LoadMUS("pop.mp3");
    splashs = Mix_LoadMUS("splash.mp3");
    hurts = Mix_LoadMUS("hurt.mp3");
    jumps = Mix_LoadMUS("jump.mp3");

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    // Free File
    Mix_FreeMusic(pop);
    Mix_FreeMusic(splashs);
    Mix_FreeMusic(hurts);
    Mix_FreeMusic(jumps);
    pop = 0;
    splashs = 0;
    hurts = 0;
    jumps = 0;
    // End
    Mix_CloseAudio();

    quit(window);
}

void reset_screen() {
    float top = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left = screen_center_x - 4 / screen_zoom;
    float right = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

void dragS() {
    drag = true;
}

void dragE() {
    drag = false;
}
