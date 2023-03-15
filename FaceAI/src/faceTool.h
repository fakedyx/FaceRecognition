#pragma once

#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include <direct.h>
#include <iostream>  
#include <stdarg.h>
#include <string>
#include <opencv2\opencv.hpp>

using namespace std;
#pragma comment(lib, "libarcsoft_face_engine.lib")

#define SafeFree(p) { if ((p)) free(p); (p) = NULL; }
#define SafeArrayDelete(p) { if ((p)) delete [] (p); (p) = NULL; } 
#define SafeDelete(p) { if ((p)) delete (p); (p) = NULL; } 

#define APPID "2Faex38JmTDEbFc1bwvaWh8Mj17JNbDq6Y174yL9rLcc"

// 32位
//#define SDKKey "8LJqeAmH6wsjcdBBMt6E1WRjt8aHyaWdsfUg7ELx8KPD"

//64位
#define SDKKey  "8LJqeAmH6wsjcdBBMt6E1WRjwNoXSvycf1ehVngD1epB"

//裁剪图片,做四字节对齐
void CutIplImage(IplImage* src, IplImage* dst, int x, int y);

void faceInit(MHandle* handle);

float faceCompare(MHandle handle, IplImage* img1, IplImage* img2);