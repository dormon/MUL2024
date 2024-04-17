#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

class Video{
  public:
    Video(std::string fileName);
    ~Video();
    void* getData();
    float getFps()const;
    uint32_t getWidth ()const;
    uint32_t getHeight()const;
    uint32_t currentFrame()const;
    void     toStart();
    void     move(int32_t frames);
    void     move(float   time  );
    void     setFramePos(uint32_t frame);
  protected:
    cv::Mat bgr_frame;
    bool _opened = false;
    cv::VideoCapture _video;
    uint32_t _frameCount = 0;
    uint32_t _frameCounter = 0;
    float _fps = 0;
    uint32_t _width = 1;
    uint32_t _height = 1;
};

