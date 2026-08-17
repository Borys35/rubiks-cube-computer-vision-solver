#include <iostream>
#include "include/cubie_cube.hpp"
#include "include/move_tables.hpp"
#include "include/moves.hpp"
#include "include/terminal_visualizer.hpp"
#include "include/bfs_solver.hpp"
#include "include/kociemba_solver.hpp"
#include "include/pruning_tables.hpp"
#include "CubeVision.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

int main()
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

	CubeVision::ColorRange color_ranges[COLOR_COUNT] = {
		{{148, 255}, {135, 156}, {31, 255}}, // U_COLOR (white)
		{{78, 255}, {0, 255}, {167, 255}}, // R_COLOR (red)
		{{13, 255}, {120, 177}, {10, 86}}, // F_COLOR (green)
		{{0, 255}, {0, 112}, {107, 133}}, // D_COLOR (yellow)
		{{0, 117}, {140, 250}, {0, 110}}, // L_COLOR (blue)
		{{140, 210}, {52, 148}, {160, 250}}  // B_COLOR (orange)
	};
	CubeVision::BgrColor display_colors[COLOR_COUNT] = {
		{255, 255, 255}, // U_COLOR (white)
		{0, 0, 255},     // R_COLOR (red)
		{0, 255, 0},     // F_COLOR (green)
		{255, 255, 0},   // D_COLOR (yellow)
		{255, 0, 0},     // L_COLOR (blue)
		{0, 165, 255}    // B_COLOR (orange)
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
        .min_solidity = 0.9f,
    };

	CubeVision cubeVision(my_cfg);

    while (true)
    {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty())
        {
            std::cerr << "Error: Could not capture a frame." << std::endl;
            break;
        }


        cv::Mat yuv = cubeVision.normalizeYuv(frame);
		cv::Mat combined_mask = cv::Mat::zeros(yuv.size(), CV_8UC1);
		std::array<cv::Mat, COLOR_COUNT> masks = cubeVision.detectCellColors(yuv, combined_mask);
        std::optional<cv::Rect> my_rect = cubeVision.extractCubeFaceRect(combined_mask, masks);

        if (my_rect.has_value()) {
            std::cout << "Rect found" << std::endl;
        }

        cv::Mat output = cv::Mat::zeros(yuv.size(), CV_8UC3);;
		for (int i = 0; i < COLOR_COUNT; ++i)
		{
		    cv::add(masks[i], output, output);
		}

		cv::imshow("Camera Feed", output);

        if (cv::waitKey(30) == 'q')
        {
            break;
        }
    }

    return 0;
}

/*
int main()
{
    MoveTables::init_move_tables();
    Kociemba::PruningTables::init_all_pruning_tables();

    CubieCube cc = CubieCube();

    std::string scramble_string = "R L F' D' F R' U2 B' R D' R2 B2 U R2 F2 B2 R2 U D2 L2";
    std::cout << "Scramble: " << scramble_string << std::endl;

    std::vector<int> scramble_moves = string_to_moves(scramble_string);
    for (size_t i = 0; i < scramble_moves.size(); i++)
    {
        cc.multiply(ALL_MOVES[scramble_moves[i]]);
    }

    Visualizer *visualizer = new TerminalVisualizer();
    visualizer->display_cube(cc.to_facelet_cube());

    // ISolver *solver = new BFSSolver(max_depth_total);
    auto max_depth_total{22u};
    auto max_depth1{12u};
    ISolver *solver = new Kociemba::KociembaSolver(max_depth_total, max_depth1);
    std::vector<int> solve_moves = solver->solve(cc);

    // Solve: R' U2 R' L
    std::cout << "Solve: " << moves_to_string(solve_moves) << " (" << solve_moves.size() << " moves)" << std::endl;

    delete visualizer;
    delete solver;

    return 0;
}
*/