#include "MenuManager.h"

void MenuManager::updateMenuPanel() {
	menu_panel.setTo(cv::Scalar(50, 50, 50));

	cv::putText(menu_panel, "Cube Solver", cv::Point(20, 40), cv::FONT_HERSHEY_TRIPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    cv::putText(menu_panel, "[q] Quit", cv::Point(20, 70), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
    cv::putText(menu_panel, "[r] Reset", cv::Point(20, 100), cv::FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(255, 255, 255), 2);
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
    cv::vconcat(menu_panel, visualizer_output, left_image);

    cv::Mat right_image;
    cv::vconcat(frame, output, right_image);

	cv::Mat combined_output;
	cv::hconcat(left_image, right_image, combined_output);
    cv::imshow("Cube Solver", combined_output);

    if (cv::waitKey(1) == 'q')
    {
		return false; // Exit the loop if 'q' is pressed
    }

	if (cv::waitKey(1) == 'r')
	{
		cubeVision.reset();
	}

    return true;
}