#include<iostream>

#include<SDL.h>

int main(int argc,char*argv[]){
  std::cerr << "Ahoj" << std::endl;
  auto window = SDL_CreateWindow("MUL2024",0,0,1024,768,0);

  bool running = true;
  while(running){// MAIN LOOP
    SDL_Event event;
    while(SDL_PollEvent(&event)){ // EVENT LOOP
      if(event.type == SDL_QUIT)running = false;
    }
  }

  SDL_DestroyWindow(window);


  return 0;
}

