#include<iostream>

#include<SDL.h>

int main(int argc,char*argv[]){
  std::cerr << "Ahoj" << std::endl;
  auto window = SDL_CreateWindow("MUL2024",0,0,1024,768,0);


  SDL_DestroyWindow(window);


  return 0;
}

