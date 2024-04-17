#include<iostream>

#include<SDL.h>
#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>

using namespace ge::gl;

int main(int argc,char*argv[]){
  std::cerr << "Ahoj" << std::endl;
  auto window = SDL_CreateWindow("MUL2024",0,0,1024,768,SDL_WINDOW_OPENGL);

  auto context = SDL_GL_CreateContext(window);

  ge::gl::init();

  auto vs = std::make_shared<Shader>(GL_VERTEX_SHADER,R".(
  #version 450

  out vec2 vTexCoord;

  void main(){

    vec2 coord = vec2(gl_VertexID&1,gl_VertexID>>1);

    gl_Position = vec4(coord*4-1,0,1);

    vTexCoord = coord*2;
  }
  ).");

  auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER,R".(
  #version 450

  in vec2 vTexCoord;

  out vec4 fColor;
  void main(){
    fColor = vec4(vTexCoord,0,1);

  }
  ).");

  auto prg = std::make_shared<Program>(vs,fs);

  bool running = true;
  while(running){// MAIN LOOP
    SDL_Event event;
    while(SDL_PollEvent(&event)){ // EVENT LOOP
      if(event.type == SDL_QUIT)running = false;
    }

    glClearColor(.2,.2,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    prg->use();
    glDrawArrays(GL_TRIANGLE_STRIP,0,3);

    SDL_GL_SwapWindow(window);
  }


  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}

