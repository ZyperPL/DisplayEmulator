#include "zylib.h"

struct Zwindow Zwindow;

// shaders
const GLchar* vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    in vec2 texcoord;
    out vec2 Texcoord;
    void main()
    {
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 0.5);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
        outColor = texture(tex, Texcoord);
    }
)glsl";

zPixel zRGB (uint8_t r, uint8_t g, uint8_t b)
{
  return { r, g, b, 255 };
}

zPixel zRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  return { r, g, b, a };
}

struct Zwindow zCreateWindow(uint32_t w, uint32_t h, const char *name)
{
  glfwInit();

  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  

  // initialize Zwindow
  Zwindow.width = w;
  Zwindow.height = h;
  Zwindow.aspect = (double)(w)/(double)(h);
  Zwindow.name = name;
  Zwindow.buffer = new uint8_t[w*h*COMPONENTS];

  // create window
  GLFWwindow *wnd = glfwCreateWindow(w, h, name, NULL, NULL);
  glfwMakeContextCurrent(wnd);

  glewExperimental = GL_TRUE; 
  glewInit();


  // create vertices
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  GLfloat vertices[] = {
      -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint ebo;
  glGenBuffers(1, &ebo);

  GLuint elements[] = {
      0, 1, 2,
      2, 3, 0
  };

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // create shaders
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

  GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

  // create texture
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  memset(&Zwindow.buffer[0], 0, w*h*COMPONENTS);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, &Zwindow.buffer[0]);
    
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return Zwindow;
}

bool zIsOpen()
{
  return !glfwWindowShouldClose(glfwGetCurrentContext());
}

void zClear(zPixel c)
{
  glClearColor((float)(c.r)/255.F, (float)(c.g)/255.F, (float)(c.b)/255.F, (float)(c.a)/255.F);

  zClear();  

}

void zClear()
{
  memset(&Zwindow.buffer[0], 0, Zwindow.width*Zwindow.height*COMPONENTS);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void zUpdate()
{
  int width, height;
  glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
  double r = double(width)/double(Zwindow.width);
  double r2 = double(height)/double(Zwindow.height);
  if (r > r2) r = r2;
  int w = int(r*Zwindow.width);
  int h = int(r*Zwindow.height);
  glViewport((width-w)/2, (height-h)/2, w, h);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glfwPollEvents();
  glfwSwapBuffers(glfwGetCurrentContext());
}

void zDrawPixel(uint16_t x, uint16_t y, zPixel c)
{
  Zwindow.buffer[y*Zwindow.width + x + 0] = c.r;
  Zwindow.buffer[y*Zwindow.width + x + 1] = c.g;
  Zwindow.buffer[y*Zwindow.width + x + 2] = c.b;  

  // update texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Zwindow.width, Zwindow.height, 0, 
               GL_RGB, GL_UNSIGNED_BYTE, &Zwindow.buffer[0]);  
}

void zFree()
{
  glfwDestroyWindow(glfwGetCurrentContext());
  glfwTerminate();
}
