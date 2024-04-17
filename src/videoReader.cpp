#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>

#include<videoReader.h>

Video::Video(std::string fileName){
  this->_opened=this->_video.open(fileName);
  if(!this->_opened){
    std::cerr<<"file: "<<fileName<<"cannot be opened as video";
    return;
  }
  this->_frameCount = this->_video.get(cv::CAP_PROP_FRAME_COUNT);
  this->_fps        = this->_video.get(cv::CAP_PROP_FPS);
  this->_width      = this->_video.get(cv::CAP_PROP_FRAME_WIDTH);
  this->_height     = this->_video.get(cv::CAP_PROP_FRAME_HEIGHT);
}

Video::~Video(){
  if(this->_opened)
    this->_video.release();
}

void* Video::getData(){
  //std::cout<<this->_frameCounter<<"/"<<this->_frameCount<<std::endl;
  if(!this->_opened)return nullptr;
  if(this->_frameCounter>=this->_frameCount){
    this->toStart();
  }
  bool status = this->_video.read(bgr_frame);
  if(!status){
    this->_frameCount = this->_frameCounter;
    this->toStart();
    if(this->_video.read(bgr_frame))return nullptr;
    this->_frameCounter++;
    return bgr_frame.data;
  }
  this->_frameCounter++;
  return bgr_frame.data;
}

float Video::getFps()const{
  return this->_fps;
}

uint32_t Video::getWidth ()const{
  return this->_width;
}

uint32_t Video::getHeight()const{
  return this->_height;
}

uint32_t Video::currentFrame()const{
  return this->_frameCounter;
}

void Video::toStart(){
  this->setFramePos(0);
}

void Video::move(int32_t frames){
  if(frames<0&&this->_frameCounter<(uint32_t)std::abs(frames))
    this->setFramePos(0);
  this->setFramePos(this->_frameCounter+frames);
}

void Video::move(float time){
  this->move((int32_t)(this->getFps()*time));
}

void Video::setFramePos(uint32_t frame){
  if(!this->_opened)return;
  if(frame>this->_frameCount)frame = this->_frameCount-1;
  this->_video.set(cv::CAP_PROP_POS_FRAMES,frame);
  this->_frameCounter = frame;
}
