#include <iostream>
#include "include/cubie_cube.hpp"
#include "include/move_tables.hpp"
#include "include/moves.hpp"
#include "include/terminal_visualizer.hpp"
#include "include/bfs_solver.hpp"
#include "include/kociemba_solver.hpp"
#include "include/pruning_tables.hpp"
#include "CubeVision.h"
#include "OpenCV2DVisualizer.h"
#include "MenuManager.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <chrono>

#define NOMINMAX
#include <windows.h>
#include <psapi.h>

PROCESS_MEMORY_COUNTERS_EX readMemory()
{
    PROCESS_MEMORY_COUNTERS_EX pmc{};
    pmc.cb = sizeof(pmc);
    GetProcessMemoryInfo(GetCurrentProcess(),
        reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
        sizeof(pmc));
    return pmc;
}

using namespace std::chrono;

int main()
{
    auto before = readMemory();

    auto begin = steady_clock::now();
    MoveTables::init_move_tables();
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - begin).count();
    std::cout << "Move Tables initialization time: " << duration << " µs" << std::endl;

    begin = steady_clock::now();
    Kociemba::PruningTables::init_all_pruning_tables();
    end = steady_clock::now();
    duration = duration_cast<microseconds>(end - begin).count();
    std::cout << "Pruning Tables initialization time: " << duration << " µs" << std::endl;

    auto after = readMemory();
    std::cout << "Memory usage: " << before.WorkingSetSize << ',' << after.WorkingSetSize << ','
        << after.PeakWorkingSetSize << ',' << after.PrivateUsage << '\n';

    //cv::VideoCapture cap(0);
    cv::VideoCapture cap(0, cv::CAP_DSHOW);
    //cap.set(cv::CAP_PROP_AUTO_WB, 0);
    cap.set(cv::CAP_PROP_SETTINGS, 1);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

    CubeVision::ColorRange color_ranges[COLOR_COUNT] = {
        {{148, 255}, {130, 156}, {31, 255}},  // U_COLOR (white)
        {{78, 122}, {0, 255}, {167, 255}},    // R_COLOR (red)
        {{13, 255}, {91, 148}, {10, 86}},    // F_COLOR (green)
        {{0, 255}, {0, 112}, {107, 133}},     // D_COLOR (yellow)
        {{140, 210}, {52, 148}, {160, 250}},  // L_COLOR (orange)
        {{0, 117}, {140, 250}, {0, 110}}      // B_COLOR (blue)
    };

	CubeVision::BgrColor display_colors[COLOR_COUNT] = {
		{255, 255, 255}, // U_COLOR (white)
		{0, 0, 255},     // R_COLOR (red)
		{0, 255, 0},     // F_COLOR (green)
		{0, 255, 255},   // D_COLOR (yellow)
        {0, 165, 255},   // L_COLOR (orange)
        {255, 0, 0},     // B_COLOR (blue)
	};
    CubeVision::Config my_cfg = {
        .color_ranges = {
            color_ranges[0], color_ranges[1], color_ranges[2], color_ranges[3], color_ranges[4], color_ranges[5]
        },
        .display_colors = {
            display_colors[0], display_colors[1], display_colors[2], display_colors[3], display_colors[4], display_colors[5]
        },
        .stable_frame_count = 7,
        .iou_threshold = 0.8f,
        .min_area_ratio = 0.075f,
        .min_solidity = 0.9f,
        .max_angle = 10.0f
    };

	CubeVision cubeVision(my_cfg);
    OpenCV2DVisualizer visualizer = OpenCV2DVisualizer();
	CubieCube currentCube = CubieCube();
	Kociemba::KociembaSolver solver = Kociemba::KociembaSolver(25u, 24u);
	MenuManager menuManager(cubeVision, visualizer, currentCube, solver);

    while (true)
    {
        begin = steady_clock::now();
        cv::Mat frame;
        cap >> frame;

        if (frame.empty())
        {
            std::cerr << "Error: Could not capture a frame." << std::endl;
            break;
        }

		if (!menuManager.update(frame))
		{
            break;
		}
		end = steady_clock::now();
        duration = duration_cast<microseconds>(end - begin).count();
        std::cout << "While look step time: " << duration << " µs" << std::endl;
    }

    return 0;
}