 /*
  * Copyright 2023 Jean-Pierre de la Croix
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */


#include <GL/glut.h>

#include <chrono>

#include <cstdlib>
#include <cmath>

float _r = 1.0f;
float _g = 0.5f;
float _b = 0.0f;

std::chrono::time_point<std::chrono::system_clock> _ts;


void handle_key_event(unsigned char key, int x, int y) {
  switch (key) {
    // escape key
    case '\x1B':
    {
      exit(EXIT_SUCCESS);
      break;
    }
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  glViewport(0, 0, 100, 100);

  glColor3f(_r, _g, _b);

  glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f( 1.0f, -1.0f);
    glVertex2f( 1.0f,  1.0f);
    glVertex2f(-1.0f,  1.0f);
  glEnd();

  glFlush();
}

void cycle_color() {

  auto ts = std::chrono::system_clock::now();

  if (std::chrono::duration_cast<std::chrono::milliseconds>(ts - _ts) >= std::chrono::milliseconds(250)) {
    _r = fmod(_r + 0.1f, 1.0f);
    _g = fmod(_g + 0.1f, 1.0f);
    _b = fmod(_b + 0.1f, 1.0f);

    _ts = ts;

    glutPostRedisplay();
  }
}

int main(int argc, char **argv) {

  glutInit(&argc, argv);

  glutInitWindowSize(200, 200);


  glutCreateWindow("Hello, world!");
  glutKeyboardFunc(&handle_key_event);
  glutDisplayFunc(&display);
  glutIdleFunc(&cycle_color);


  _ts = std::chrono::system_clock::now();
  glutMainLoop();

  return EXIT_SUCCESS; 
}