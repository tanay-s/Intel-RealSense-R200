#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "math.h"
#include "iostream"
#include "tinyxml2.h"
#include "tinyxml2.cpp"
#include <fstream>
#include <windows.h>
#include "RealSense/SenseManager.h"
#include "RealSense/SampleReader.h"
#include "util_cmdline.h"
#include "util_render.h"
#include <conio.h>
#include "iostream"


using namespace tinyxml2;
using namespace cv;
using namespace std;
using namespace Intel::RealSense;

cv::Mat PXCImage2CVMat(PXCImage *pxcImage, PXCImage::PixelFormat format)
{
	PXCImage::ImageData data;
	pxcImage->AcquireAccess(PXCImage::ACCESS_READ, format, &data);
	int width = pxcImage->QueryInfo().width;
	int height = pxcImage->QueryInfo().height;
	if (!format)
		format = pxcImage->QueryInfo().format;
	int type = 0;
	if (format == PXCImage::PIXEL_FORMAT_Y8)
		type = CV_8UC1;
	else if (format == PXCImage::PIXEL_FORMAT_RGB24)
		type = CV_8UC3;
	else if (format == PXCImage::PIXEL_FORMAT_DEPTH_F32)
		type = CV_16U;
	cv::Mat ocvImage = cv::Mat(cv::Size(width, height), type, data.planes[0], data.pitches[0]);
	pxcImage->ReleaseAccess(&data);
	return ocvImage;
}

int wmain(int argc, WCHAR* argv[]) {
	//int MAX_FRAMES = 2000;
	PXCSenseManager *psm = PXCSenseManager::CreateInstance();
	PXCImage *colorIm, *depthIm;
	//UtilRender *renderColor(L"Color");
	UtilRender *renderColor = new UtilRender(L"COLOR STREAM");
	UtilRender *renderDepth = new UtilRender(L"DEPTH STREAM");
	// Select the color stream
	psm->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480);
	psm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 628, 468);
	
	PXCImage:: PixelFormat format;
	
	Mat color_image;
	Mat depth_image;
	
    	
	// Initialize and Stream Samples
	psm->Init();

	for (int i=1;i<20;i++) {

		// This function blocks until all streams are ready (depth and color)
		// if false streams will be unaligned

		if (psm->AcquireFrame(true)<PXC_STATUS_NO_ERROR) break;

		// retrieve all available image samples
		PXCCapture::Sample *sample = psm->QuerySample();

		// retrieve the image or frame by type from the sample
		colorIm = sample->color;
		depthIm = sample->depth;

		
		format = colorIm->QueryInfo().format;
		color_image = PXCImage2CVMat(colorIm, format);
		imwrite("alpha.png", color_image);
		
		
		format = depthIm->QueryInfo().format;
		depth_image = PXCImage2CVMat(depthIm, format);
		imwrite("beta.png", depth_image);
		// render the frame
		//if (!renderColor->RenderFrame(colorIm)) break;
		//if (!renderDepth->RenderFrame(depthIm)) break;

		
		// release or unlock the current frame to fetch the next frame
		psm->ReleaseFrame();

	}
	
	
}
