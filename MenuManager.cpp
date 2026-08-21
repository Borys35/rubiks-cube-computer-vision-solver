#include "MenuManager.h"

void MenuManager::updateMenuPanel() {
	menu_panel.setTo(cv::Scalar(50, 50, 50));

	cv::putText(menu_panel, "Real-time Rubik's Cube Solver", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    cv::putText(menu_panel, "[q] Quit", cv::Point(20, 70), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
    cv::putText(menu_panel, "[r] Reset", cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
    if (!isCubeCaptured())
    {
        cv::putText(menu_panel, "Current step: Capture cube", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
    }
    else {
		if (!isCubeSolvable())
		{
			cv::putText(menu_panel, "Current step: Cube is not solvable. Capture again.", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 2);
		}
		else
		{
            if (!currently_solving)
			    cv::putText(menu_panel, "Current step: Cube is solvable. [s] Get solve", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
            else
                cv::putText(menu_panel, "Current step: Wait for the solve...", cv::Point(20, 150), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
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
	currentCube.from_facelet_cube(cubeVision.getCurrentFaceletCube());
    return currentCube.is_solvable();
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
    cv::Mat left_image;
    cv::putText(visualizer_output, "Visualizer", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    cv::vconcat(menu_panel, visualizer_output, left_image);

    cv::Mat right_image;
	cv::putText(frame, "Camera", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(0, 0, 0), 2);
    cv::putText(output, "Output Masks", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    cv::vconcat(frame, output, right_image);

	cv::Mat combined_output;
	cv::hconcat(left_image, right_image, combined_output);
    cv::imshow("Cube Solver", combined_output);

	int key = cv::waitKey(1);
    if (key == 'q')
    {
		return false; // Exit the loop if 'q' is pressed
    } else if (key == 'r')
	{
		std::cout << "Resetting cube state..." << std::endl;
		cubeVision.reset();
	}
    else if (key == 's')
    {
        if (!currently_solving) {
            // TODO: move to a separate thread to avoid blocking the main loop
            std::cout << "Getting solve..." << std::endl;
            solver_thread = std::thread(&MenuManager::solveCube, this);
        }
	}

    return true;
}

void MenuManager::solveCube() {
    currently_solving = true;
    std::vector<int> solve_moves = solver.solve(currentCube);
    currently_solving = false;
    std::cout << "Solved: " << moves_to_string(solve_moves) << " (" << solve_moves.size() << " moves)" << std::endl;
}