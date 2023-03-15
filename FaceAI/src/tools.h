#pragma once

#include <opencv2/opencv.hpp>
#include "CvvImage.h"
#include <Windows.h>
#include <graphics.h>

using namespace cv;

void paiZhao(VideoCapture* cap, const char* name);

void showImage(CvvImage* out, HWND hwnd, UINT imgID);

void refreshCamera(VideoCapture* cap, HWND hwnd, UINT id);

void putimagePNG(int  picture_x, int picture_y, IMAGE* srcImage, IMAGE* destImage);
