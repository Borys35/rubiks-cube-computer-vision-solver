#include "MenuManager.h"

void MenuManager::updateMenuPanel() {
	menu_panel.setTo(cv::Scalar(50, 20, 20));

	cv::putText(menu_panel, "Real-time Rubik's Cube Solver", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    cv::putText(menu_panel, "[q] Quit", cv::Point(20, 70), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    cv::putText(menu_panel, "[r] Reset", cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    if (!isCubeCaptured())
    {
        cv::putText(menu_panel, "Current step: Capture cube", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(55, 55, 255), 2);

        cv::putText(menu_panel, "Show cube with these orientations (Front/Top):", cv::Point(20, 190), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

        cv::Scalar W(255, 255, 255), G(0, 200, 0), R(0, 0, 255);
        cv::Scalar B(255, 0, 0), O(0, 128, 255), Y(0, 255, 255);

        struct LegendItem { cv::Scalar front, top; std::string text; };
        std::vector<LegendItem> legend = {
            {W, B, "Scan White (Top: Blue)"},
            {G, W, "Scan Green (Top: White)"},
            {R, W, "Scan Red (Top: White)"},
            {B, W, "Scan Blue (Top: White)"},
            {O, W, "Scan Orange (Top: White)"},
            {Y, G, "Scan Yellow (Top: Green)"}
        };

        int start_y = 210;
        for (size_t i = 0; i < legend.size(); ++i) {
            int y = start_y + (static_cast<int>(i) * 28);

            cv::rectangle(menu_panel, cv::Rect(20, y, 18, 6), legend[i].top, cv::FILLED); // top square
            cv::rectangle(menu_panel, cv::Rect(20, y + 6, 18, 18), legend[i].front, cv::FILLED); // front square
            cv::rectangle(menu_panel, cv::Rect(20, y, 18, 24), cv::Scalar(100, 100, 100), 1); // outline

            cv::putText(menu_panel, legend[i].text, cv::Point(48, y + 17), cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(220, 220, 220), 1);
        }
    }
    else {
		if (!isCubeSolvable())
		{
			cv::putText(menu_panel, "Current step: Cube is not solvable. Capture again.", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(55, 55, 255), 2);
		}
		else
		{
            if (!currently_solving) {
				if (solution.empty()) {
					cv::putText(menu_panel, "Current step: Cube is solvable. [s] Get solve", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
				}
				else {
					std::lock_guard<std::mutex> lock(solution_mutex);
					std::string solution_str = moves_to_string(solution);
					cv::putText(menu_panel, "Current step: Solution found (" + std::to_string(solution.size()) + " moves). Perform moves below.", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(0, 255, 0), 2);
                    cv::putText(menu_panel, solution_str, cv::Point(20, 180), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);

                    cv::putText(menu_panel, "Hold the cube in this position:", cv::Point(20, 220), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

                    int sy = 240;
                    cv::Scalar W(255, 255, 255);
                    cv::Scalar G(0, 200, 0);

                    cv::rectangle(menu_panel, cv::Rect(20, sy, 18, 6), W, cv::FILLED); // top face
                    cv::rectangle(menu_panel, cv::Rect(20, sy + 6, 18, 18), G, cv::FILLED); // front face
                    cv::rectangle(menu_panel, cv::Rect(20, sy, 18, 24), cv::Scalar(100, 100, 100), 1); // outline

                    cv::putText(menu_panel, "Front Green, Top White", cv::Point(48, sy + 17), cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(220, 220, 220), 1);
                }
            }
            else {
                cv::putText(menu_panel, "Current step: Wait for the solve...", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
            }
        }
    }
}

bool MenuManager::isCubeCaptured() const {
	FaceletCube current_facelet_cube = cubeVision.getCurrentFaceletCube();
	for (int i = 0; i < 54; ++i) {
		if (current_facelet_cube[i] == UNKNOWN_COLOR) {
			return false;
		}
	}
    return true;
}

bool MenuManager::isCubeSolvable() const {
    CubieCube tempCube;
	tempCube.from_facelet_cube(cubeVision.getCurrentFaceletCube());
    return tempCube.is_solvable();
}

bool MenuManager::update(cv::Mat& frame) {
    cv::Mat yuv = cubeVision.normalizeYuv(frame);
    cv::Mat combined_mask = cv::Mat::zeros(yuv.size(), CV_8UC1);
    std::array<cv::Mat, COLOR_COUNT> masks = cubeVision.detectCellColors(yuv, combined_mask);
    std::optional<cv::Rect> my_rect = cubeVision.extractCubeFaceRect(combined_mask, masks);

    cv::Mat output = cv::Mat::zeros(yuv.size(), CV_8UC3);;
    for (int i = 0; i < COLOR_COUNT; ++i)
    {
        cv::add(masks[i], output, output);
    }

    if (my_rect.has_value()) {
        cv::rectangle(output, my_rect.value(), cv::Scalar(200, 200, 200));
    }

    if (cubeVision.registerRect(my_rect)) {
        if (my_rect.has_value()) {
            cubeVision.readFaceState(my_rect.value(), masks);
        }
    }

    updateMenuPanel();

    visualizer.display_cube(cubeVision.getCurrentFaceletCube());
    cv::Mat visualizer_output = visualizer.get_mat();
    cv::Mat top_image;
    cv::putText(visualizer_output, "Visualizer", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    cv::hconcat(menu_panel, visualizer_output, top_image);

    cv::Mat bottom_image;
	cv::putText(frame, "Camera", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
    cv::putText(output, "Output Mask", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    cv::hconcat(frame, output, bottom_image);

	cv::Mat combined_output;
	cv::vconcat(top_image, bottom_image, combined_output);
    cv::imshow("Cube Solver", combined_output);

	int key = cv::waitKey(1);
    if (key == 'q')
    {
        if (solver_thread.joinable()) {
            solver_thread.request_stop();
            solver_thread.join();
        }
		return false; // Exit the loop if 'q' is pressed
    } 
    else if (key == 'r')
	{
		std::cout << "Resetting cube state..." << std::endl;
        if (solver_thread.joinable()) {
            solver_thread.request_stop();
            solver_thread.join();
        }
        std::cout << "Cube state reset." << std::endl;
		cubeVision.reset();
		solution.clear();
	}
    else if (key == 's')
    {
        if (!currently_solving) {
            // TODO: move to a separate thread to avoid blocking the main loop
            std::cout << "Getting solve..." << std::endl;
            if (solver_thread.joinable()) {
                solver_thread.request_stop();
                solver_thread.join();
            }
            solver_thread = std::jthread([this](std::stop_token stoken) {
                this->solveCube(stoken);
            });
        }
	}

    return true;
}

void MenuManager::solveCube(std::stop_token stoken) {
    std::lock_guard<std::mutex> lock(solution_mutex);
    currently_solving = true;
    std::cout << "Current facelet state: ";
    CubieCube tempCube;
    tempCube.from_facelet_cube(cubeVision.getCurrentFaceletCube());
    for (int i = 0; i < 54; ++i) {
        std::cout << tempCube.to_facelet_cube()[i] << " ";
    }
    std::cout << std::endl;
    solution = solver.solve(tempCube, stoken);
    currently_solving = false;
    std::cout << "Solved: " << moves_to_string(solution) << " (" << solution.size() << " moves)" << std::endl;
}