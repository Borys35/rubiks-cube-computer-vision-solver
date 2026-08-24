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

int main()
{
    MoveTables::init_move_tables();
    Kociemba::PruningTables::init_all_pruning_tables();

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

	/*CubeVision::ColorRange color_ranges[COLOR_COUNT] = {
		{{148, 255}, {135, 156}, {31, 255}},  // U_COLOR (white)
		{{104, 143}, {0, 255}, {167, 255}},   // R_COLOR (red)
		{{13, 255}, {120, 177}, {10, 86}},    // F_COLOR (green)
		{{0, 255}, {0, 112}, {107, 133}},     // D_COLOR (yellow)
        {{167, 203}, {52, 148}, {160, 250}},  // L_COLOR (orange)
		{{0, 117}, {140, 250}, {0, 110}}      // B_COLOR (blue)
	};*/
    CubeVision::ColorRange color_ranges[COLOR_COUNT] = {
        {{148, 255}, {135, 156}, {31, 255}},  // U_COLOR (white)
        {{78, 255}, {0, 255}, {167, 255}},    // R_COLOR (red)
        {{13, 255}, {120, 177}, {10, 86}},    // F_COLOR (green)
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
        .stable_frame_count = 5,
        .iou_threshold = 0.8f,
        .min_area_ratio = 0.075f,
        .min_solidity = 0.9f,
        .max_angle = 10.0f
    };

	CubeVision cubeVision(my_cfg);
    OpenCV2DVisualizer visualizer = OpenCV2DVisualizer();
	CubieCube currentCube = CubieCube();
	Kociemba::KociembaSolver solver = Kociemba::KociembaSolver(25u, 20u);
	MenuManager menuManager(cubeVision, visualizer, currentCube, solver);

    while (true)
    {
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
        
    }

    return 0;
}

/*
// 0 0 0 0 0 0 0 0 0 4 4 4 1 1 1 1 1 1 5 5 5 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 1 1 1 4 4 4 4 4 4 2 2 2 5 5 5 5 5 5 <- U2 from cv
// 0 0 0 0 0 0 0 0 0 4 4 4 1 1 1 1 1 1 5 5 5 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 1 1 1 4 4 4 4 4 4 2 2 2 5 5 5 5 5 5
int main()
{
    MoveTables::init_move_tables();
    Kociemba::PruningTables::init_all_pruning_tables();

    CubieCube cc = CubieCube();

    std::string scramble_string = "U2 R";
    std::cout << "Scramble: " << scramble_string << std::endl;

    std::vector<int> scramble_moves = string_to_moves(scramble_string);
    for (size_t i = 0; i < scramble_moves.size(); i++)
    {
        cc.multiply(ALL_MOVES[scramble_moves[i]]);
    }

	cc.from_facelet_cube(cc.to_facelet_cube()); // Ensure the cube is in a valid state after scrambling
	std::cout << "Scrambled cube state: ";
	for (size_t i = 0; i < 54; i++)
	{
		std::cout << cc.to_facelet_cube()[i] << " ";
	}
    Visualizer *visualizer = new TerminalVisualizer();
    visualizer->display_cube(cc.to_facelet_cube());

    // ISolver *solver = new BFSSolver(max_depth_total);
    auto max_depth_total{20u};
    auto acceptable_length{20u};
    ISolver *solver = new Kociemba::KociembaSolver(max_depth_total, acceptable_length);
    std::vector<int> solve_moves = solver->solve(cc);

    // Solve: R' U2 R' L
    std::cout << "Solve: " << moves_to_string(solve_moves) << " (" << solve_moves.size() << " moves)" << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}*/