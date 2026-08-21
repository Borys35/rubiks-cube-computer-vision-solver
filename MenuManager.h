#pragma once

#include "CubeVision.h"
#include "OpenCV2DVisualizer.h"
#include <opencv2/opencv.hpp>

class MenuManager
{
public:
	MenuManager(CubeVision& cube_vision, OpenCV2DVisualizer& visualizer, CubieCube& current_cube) : 
		cubeVision(cube_vision), visualizer(visualizer), currentCube(current_cube) {
		menu_panel = cv::Mat::zeros(480, 640, CV_8UC3);
	}

	bool update(cv::Mat& frame);
private:
	CubeVision& cubeVision;
	OpenCV2DVisualizer& visualizer;
	CubieCube& currentCube;

	void updateMenuPanel();
	bool isCubeCaptured() const;
	bool isCubeSolvable() const;

	cv::Mat menu_panel;
};

