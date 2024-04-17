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

  uniform int isFilterActivated = 1;

  out vec4 fColor;
  void main(){
    vec3 color = texture(tex,vTexCoord).rgb;
    if(isFilterActivated != 1){
      fColor = vec4(color,1);
      return;
    }

    // filter
    vec2 A=vec2(0.5f,0.5f);
    vec2 B=vec2(-1,1);
    vec2 s = normalize(B-A);
    vec2 n = vec2(-s.y,s.x);
    vec3 p = vec3(n,-dot(n,A));

    float dist = dot(p,vec3(vTexCoord,1.f));

    float stripe = fract(dist*20);

    if(color.r < 0.5)color *= vec3(stripe);


    if(stripe < 3.f/3.f && stripe>=2.f/3.f){color.b = float((stripe-2/3.f)*3.f > 1-color.b);}
    if(stripe < 2.f/3.f && stripe>=1.f/3.f){color.g = float((stripe-1/3.f)*3.f > 1-color.g);}
    if(stripe < 1.f/3.f && stripe>=0.f/3.f){color.r = float((stripe-0/3.f)*3.f > 1-color.r);}

    fColor = vec4(color,1);

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
      if(event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_f)prg->set1i("isFilterActivated",1);
        if(event.key.keysym.sym == SDLK_g)prg->set1i("isFilterActivated",0);
      }
    }

    glClearColor(.2,.2,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    //float newTime = timer.elapsedFromStart();
    //if((newTime-lastFrameTime)>1./video.getFps()){
      tex->setData2D(video.getData(),GL_BGR,GL_UNSIGNED_BYTE);
    //  lastFrameTime = newTime;
    //}


    prg->use();
    tex->bind(0);
    glDrawArrays(GL_TRIANGLE_STRIP,0,3);

    SDL_GL_SwapWindow(window);
  }


  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);


  return 0;
}

