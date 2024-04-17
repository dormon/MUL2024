#include<iostream>

#include<SDL.h>
#include<geGL/geGL.h>
#include<geGL/StaticCalls.h>

#include<videoReader.h>
#include<timer.hpp>

using namespace ge::gl;

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "."
#endif

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
    vTexCoord.y = 1-vTexCoord.y;
  }
  ).");

  auto fs = std::make_shared<Shader>(GL_FRAGMENT_SHADER,R".(
  #version 450

  in vec2 vTexCoord;

  layout(binding=0)uniform sampler2D tex;

  out vec4 fColor;
  void main(){
    fColor = texture(tex,vTexCoord);

  }
  ).");

  auto prg = std::make_shared<Program>(vs,fs);

  auto video = Video(CMAKE_ROOT_DIR "/res/mtm.mp4");


  auto tex = std::make_shared<Texture>(GL_TEXTURE_2D,GL_RGB8,1,
      video.getWidth(),video.getHeight());

  tex->texParameteri(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  tex->texParameteri(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  tex->texParameteri(GL_TEXTURE_WRAP_S,GL_CLAMP);
  tex->texParameteri(GL_TEXTURE_WRAP_T,GL_CLAMP);

  auto timer = Timer<float>();
  timer.reset();

  bool running = true;
  float lastFrameTime = 0.f;
  while(running){// MAIN LOOP
    SDL_Event event;
    while(SDL_PollEvent(&event)){ // EVENT LOOP
      if(event.type == SDL_QUIT)running = false;
    }

    glClearColor(.2,.2,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    float newTime = timer.elapsedFromStart();
    if((newTime-lastFrameTime)>1./video.getFps()){
      tex->setData2D(video.getData(),GL_BGR,GL_UNSIGNED_BYTE);
      lastFrameTime = newTime;
    }


    prg->use();
    tex->bind(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,3);

    SDL_GL_SwapWindow(window);
  }


  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}

