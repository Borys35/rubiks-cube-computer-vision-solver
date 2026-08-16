#pragma once
#include <opencv2/opencv.hpp>

class CubeVision
{
public:
	struct Config {};
	CubeVision(const Config& config) : cfg(config) {}

	cv::Mat yuvNormalize(const cv::Mat& bgr_frame);
	cv::Mat makeCombinedColorMask(const cv::Mat& yuv_norm);

private:
	Config cfg;
};

