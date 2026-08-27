#pragma once

#include "CubeVision.h"
#include "OpenCV2DVisualizer.h"
#include "include/i_solver.hpp"
#include "include/kociemba_solver.hpp"
#include <opencv2/opencv.hpp>
#include <thread>

class MenuManager
{
public:
	MenuManager(CubeVision& cube_vision, OpenCV2DVisualizer& visualizer, CubieCube& current_cube, ISolver& solver) : 
		cubeVision(cube_vision), visualizer(visualizer), currentCube(current_cube), solver(solver) {
		menu_panel = cv::Mat::zeros(480, 640, CV_8UC3);
	}

	~MenuManager() {
		if (solver_thread.joinable()) {
			solver_thread.join();
		}
	}

	bool update(cv::Mat& frame);
private:
	CubeVision& cubeVision;
	OpenCV2DVisualizer& visualizer;
	CubieCube& currentCube;
	ISolver& solver;

	void updateMenuPanel();
	bool isCubeCaptured() const;
	bool isCubeSolvable() const;
	void solveCube();

	cv::Mat menu_panel;
	std::atomic<bool> currently_solving = false;
	std::thread solver_thread;
	std::vector<int> solution = {};
	std::mutex solution_mutex;
};

