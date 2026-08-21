#pragma once

#include "include/visualizer.hpp"
#include "CubeVision.h"
#include <opencv2/opencv.hpp>

class OpenCV2DVisualizer :
    public Visualizer
{
public:
	OpenCV2DVisualizer() : mat(cv::Mat::zeros(480, 640, CV_8UC3)) {
		this->display_colors = std::array<CubeVision::BgrColor, COLOR_COUNT + 1>{
				CubeVision::BgrColor{255, 255, 255}, // U_COLOR (white)
				CubeVision::BgrColor{0, 0, 255},     // R_COLOR (red)
				CubeVision::BgrColor{0, 255, 0},     // F_COLOR (green)
				CubeVision::BgrColor{255, 255, 0},   // D_COLOR (yellow)
				CubeVision::BgrColor{255, 0, 0},     // L_COLOR (blue)
				CubeVision::BgrColor{0, 165, 255},    // B_COLOR (orange)
				CubeVision::BgrColor{10, 29, 61}    // UKNOWN_COLOR (brown)
		};
	}

    void display_cube(const FaceletCube& fc) const override;
    cv::Mat get_mat();
private:
	cv::Mat mat;
    std::array<CubeVision::BgrColor, COLOR_COUNT + 1> display_colors;
};

