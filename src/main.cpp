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

  bool running = true;
  while(running){// MAIN LOOP
    SDL_Event event;
    while(SDL_PollEvent(&event)){ // EVENT LOOP
      if(event.type == SDL_QUIT)running = false;
    }

    glClearColor(.2,.2,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
  }


  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}

