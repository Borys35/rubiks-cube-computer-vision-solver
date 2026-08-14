#include <iostream>
#include "cubie_cube.hpp"
#include "move_tables.hpp"
#include "moves.hpp"
#include "terminal_visualizer.hpp"
#include "bfs_solver.hpp"
#include "kociemba_solver.hpp"
#include "pruning_tables.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

int main()
{
    cv::VideoCapture cap("http://localhost:8080/video");
    if (!cap.isOpened())
    {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

    while (true)
    {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty())
        {
            std::cerr << "Error: Could not capture a frame." << std::endl;
            break;
        }

        cv::Mat yuv;
        cv::cvtColor(frame, yuv, cv::COLOR_BGR2YUV);
        cv::Mat yuv_norm = yuv.clone();

        for (int r = 0; r < yuv.rows; r++)
        {
            for (int c = 0; c < yuv.cols; c++)
            {
                cv::Vec3b pixel = yuv.at<cv::Vec3b>(r, c);
                float Y = pixel[0];
                float U = pixel[1];
                float V = pixel[2];

                if (Y == 0 || Y == 255)
                    continue;

                float kappa = 1.0f;
                if (Y > 128)
                {
                    kappa = 128.0f / Y;
                }
                else if (Y < 128)
                {
                    kappa = 128.0f / (256.0f - Y);
                }

                int U_prime = std::round((U - 128) * kappa + 128);
                int V_prime = std::round((V - 128) * kappa + 128);

                U_prime = std::clamp(U_prime, 0, 255);
                V_prime = std::clamp(V_prime, 0, 255);

                // Preserve original Y for Table 1 filtering, apply normalized U' and V'
                yuv_norm.at<cv::Vec3b>(r, c) = cv::Vec3b(Y, U_prime, V_prime);
            }
        }

        // Define masks for all 6 Rubik's Cube colors based on Table 1
        cv::Mat mask_red, mask_orange, mask_blue, mask_green, mask_white, mask_yellow;

        // Red: Y[50-140], U'[110±38 -> 72-148], V'[205±45 -> 160-250]
        cv::inRange(yuv_norm, cv::Scalar(50, 72, 160), cv::Scalar(140, 148, 250), mask_red);

        // Orange: Y[140-210], U'[100±48 -> 52-148], V'[205±45 -> 160-250]
        cv::inRange(yuv_norm, cv::Scalar(140, 52, 160), cv::Scalar(210, 148, 250), mask_orange);

        // Blue: Y[70-240], U'[195±55 -> 140-250], V'[55±55 -> 0-110]
        cv::inRange(yuv_norm, cv::Scalar(70, 140, 0), cv::Scalar(240, 250, 110), mask_blue);

        // Green: Y[70-240], U'[70±65 -> 5-135], V'[60±50 -> 10-110]
        cv::inRange(yuv_norm, cv::Scalar(70, 5, 10), cv::Scalar(240, 135, 110), mask_green);

        // White: Y[190-255], U'[128±12 -> 116-140], V'[128±11 -> 117-139]
        cv::inRange(yuv_norm, cv::Scalar(190, 116, 117), cv::Scalar(255, 140, 139), mask_white);

        // Yellow: Y[118-150], U'[90±25 -> 65-115], V'[140±20 -> 120-160]
        cv::inRange(yuv_norm, cv::Scalar(118, 65, 120), cv::Scalar(150, 115, 160), mask_yellow);

        // Combine all color masks into a single mask for cell detection
        cv::Mat combined_mask = mask_red | mask_orange | mask_blue | mask_green | mask_white | mask_yellow;

        // Cell Recognition: post-processed by subsequent erosion- and dilatation-like filtering
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
        cv::erode(combined_mask, combined_mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 2);
        cv::dilate(combined_mask, combined_mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)), cv::Point(-1, -1), 2);

        // Border tracking of homogeneous regions
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(combined_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        double max_area = 0.0;
        int max_area_idx = -1;

        // Find the contour with the largest area
        for (size_t i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            if (area > max_area)
            {
                max_area = area;
                max_area_idx = static_cast<int>(i);
            }
        }

        cv::Mat frame_with_box = frame.clone();

        // If a valid contour was found, draw a bounding rectangle around it
        if (max_area_idx != -1)
        {
            cv::Rect bounding_box = cv::boundingRect(contours[max_area_idx]);

            // Draw a green square/rectangle around the largest area
            cv::rectangle(frame_with_box, bounding_box, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Camera Feed (Cube Detection)", frame_with_box);

        if (max_area_idx != -1)
        {
            cv::Rect bounding_box = cv::boundingRect(contours[max_area_idx]);

            // Draw a green rectangle around the largest area on the main frame
            cv::rectangle(frame_with_box, bounding_box, cv::Scalar(0, 255, 0), 2);

            // 1. Extract the cube face ROI from the original frame
            cv::Mat cube_face = frame(bounding_box).clone();

            // 2. Calculate individual cell dimensions
            int cell_width = cube_face.cols / 3;
            int cell_height = cube_face.rows / 3;

            // 3. Draw the 3x3 grid
            for (int row = 0; row < 3; row++)
            {
                for (int col = 0; col < 3; col++)
                {
                    // Define the bounding box for each of the 9 cells
                    cv::Rect cell_rect(col * cell_width, row * cell_height, cell_width, cell_height);

                    // Draw a blue grid line on the extracted face
                    cv::rectangle(cube_face, cell_rect, cv::Scalar(255, 0, 0), 2);

                    // (Optional) Extract the individual cell to analyze its average color
                    // cv::Mat single_cell = cube_face(cell_rect);
                }
            }

            // Display the extracted and subdivided face in a new window
            cv::imshow("Extracted Cube Face", cube_face);
        }
        // cv::imshow("Combined Color Mask", combined_mask);

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