#include "CubeVision.h"

cv::Mat CubeVision::normalizeYuv(const cv::Mat& bgr_frame)
{
	cv::Mat yuv;
	cv::cvtColor(bgr_frame, yuv, cv::COLOR_BGR2YUV);
	cv::Mat yuv_norm = yuv.clone();
	for (int r = 0; r < yuv.rows; r++)
	{
		for (int c = 0; c < yuv.cols; c++)
		{
			cv::Vec3b pixel = yuv.at<cv::Vec3b>(r, c);
			uchar Y = pixel[0];
			uchar U = pixel[1];
			uchar V = pixel[2];
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
			uchar U_prime = cv::saturate_cast<uchar>(std::round((U - 128.0f) * kappa + 128.0f));
			uchar V_prime = cv::saturate_cast<uchar>(std::round((V - 128.0f) * kappa + 128.0f));

			/*
			U_prime = std::clamp(U_prime, 0, 255);
			V_prime = std::clamp(V_prime, 0, 255);
			*/

			yuv_norm.at<cv::Vec3b>(r, c) = cv::Vec3b(Y, U_prime, V_prime);
		}
	}
	return yuv_norm;
}

std::array<cv::Mat, COLOR_COUNT> CubeVision::detectCellColors(const cv::Mat& yuv_frame, cv::Mat& combined_mask)
{
	cv::Mat erode_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
	std::array<cv::Mat, COLOR_COUNT> masks;
	for (int i = 0; i < COLOR_COUNT; ++i)
	{
		masks[i] = cv::Mat::zeros(combined_mask.size(), combined_mask.type());
		const ColorRange& range = cfg.color_ranges[i];
		cv::inRange(yuv_frame,
			cv::Scalar(range.y_range.min, range.u_range.min, range.v_range.min),
			cv::Scalar(range.y_range.max, range.u_range.max, range.v_range.max),
			masks[i]);
		
		cv::erode(masks[i], masks[i], erode_kernel);
		cv::dilate(masks[i], masks[i], dilate_kernel);
		
		combined_mask |= masks[i];

		cv::Mat colored_mask = cv::Mat::zeros(masks[i].size(), CV_8UC3);
		colored_mask.setTo(cv::Scalar(cfg.display_colors[i].b, cfg.display_colors[i].g, cfg.display_colors[i].r), masks[i]);
		masks[i] = colored_mask;
	}
	
	return masks;
}

std::optional<cv::Rect> CubeVision::extractCubeFaceRect(const cv::Mat& combined_mask, const std::array<cv::Mat, COLOR_COUNT>& masks)
{
	// cv::findContours(combined_mask)
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(combined_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	if (contours.empty())
	{
		return std::nullopt;
	}

	auto largest = std::ranges::max_element(contours, {}, [](const auto& c) {
		return cv::contourArea(c);
	});

	if (cv::contourArea(*largest) < cfg.min_area_ratio * combined_mask.total()) {
		return std::nullopt;
	}

	cv::Mat cube_region = cv::Mat::zeros(combined_mask.size(), combined_mask.type());
	int largest_idx = static_cast<int>(std::distance(contours.begin(), largest));

	cv::drawContours(cube_region, contours, largest_idx, cv::Scalar(255), cv::FILLED);

	// white handling
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));

	cv::Mat dilated_region;
	cv::dilate(cube_region, dilated_region, kernel, cv::Point(-1, -1), 1);

	cv::Mat touching;

	std::array<cv::Mat, COLOR_COUNT> colorless_masks;
	for (int i = 0; i < COLOR_COUNT; i++) {
		cv::cvtColor(masks[i], colorless_masks[i], cv::COLOR_BGR2GRAY);
	}
	cv::bitwise_and(colorless_masks[U_COLOR], dilated_region, touching);
	
	cube_region.setTo(cv::Scalar(255), touching);

	// shape check
	int region_area = cv::countNonZero(cube_region);
	std::vector<std::vector<cv::Point>> region_contours;
	cv::findContours(cube_region, region_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	if (region_contours.empty()) {
		return std::nullopt;
	}

	std::vector<cv::Point> region_contour = *std::ranges::max_element(region_contours, {}, [](const auto& c) {
		return cv::contourArea(c);
	});

	// check if face is square (solidity)
	std::vector<cv::Point> hull;
	cv::convexHull(region_contour, hull);
	double hull_area = cv::contourArea(hull);

	if (hull_area > 0.0 && (region_area / hull_area) < cfg.min_solidity) {
		return std::nullopt;
	}

	cv::RotatedRect rotated_rect = cv::minAreaRect(region_contour);
	if (rotated_rect.angle < (90.0f - cfg.max_angle / 2.0f) && rotated_rect.angle > (cfg.max_angle / 2.0f)) 
	{
		return std::nullopt;
	}

	cv::Rect rect = rotated_rect.boundingRect();
	float aspect_ratio = static_cast<float>(rect.width) / static_cast<float>(rect.height);
	if (aspect_ratio <= 0.85 || aspect_ratio >= 1.15) {

		return std::nullopt;
	}

	return rect;
}

float CubeVision::iou(const cv::Rect& rect1, const cv::Rect& rect2) {
	int ix = std::ranges::max(rect1.x, rect2.x);
	int iy = std::ranges::max(rect1.y, rect2.y);
	int ix2 = std::ranges::max(rect1.x + rect1.width, rect2.x + rect2.width);
	int iy2 = std::ranges::max(rect1.y + rect1.height, rect2.y + rect2.height);
	int iw = std::ranges::max(0, ix2 - ix);
	int ih = std::ranges::max(0, iy2 - iy);
	int inter = iw * ih;
	int uni = rect1.width * rect1.height + rect2.width * rect2.height - inter;
	if (uni == 0)
		return 0.0f;
	else
		return static_cast<float>(inter) / static_cast<float>(uni);
}

bool CubeVision::registerRect(const std::optional<cv::Rect>& rect) {
	if (rect.has_value()) {
		if (prev_rect.has_value() && iou(prev_rect.value(), rect.value()) >= cfg.iou_threshold) {
			stable_frames++;
		}
		else {
			stable_frames = 0;
		}
		prev_rect = rect.value();

		if (stable_frames >= cfg.stable_frame_count) {
			std::cout << "Stable cube face detected" << std::endl;
			stable_frames = 0;
			return true;
		}
	}
	else {
		stable_frames = 0;
		prev_rect = std::nullopt;
	}
	return false;
}

void CubeVision::readFaceState(const cv::Rect& rect, const std::array<cv::Mat, COLOR_COUNT>& masks) {
	int cell_width = rect.width / 3;
	int cell_height = rect.height / 3;
	FaceletFace facelet_face;
	
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			cv::Rect cell_rect(rect.x + j * cell_width, rect.y + i * cell_height, cell_width, cell_height);
			Color max_color = UNKNOWN_COLOR;
			int max_color_count = 0;
			for (int k = 0; k < COLOR_COUNT; ++k) {
				cv::Rect safe_rect = cell_rect & cv::Rect(0, 0, masks[k].cols, masks[k].rows);
				cv::Mat cell_mask = masks[k](safe_rect);
				cv::Mat single_channel;
				cv::cvtColor(cell_mask, single_channel, cv::COLOR_BGR2GRAY);
				int color_count = cv::countNonZero(single_channel);
				if (color_count > max_color_count) {
					max_color_count = color_count;
					max_color = static_cast<Color>(k);
				}
			}
			facelet_face[i * 3 + j] = max_color;
		}
	}

	int start_cell_idx = facelet_face[4] * 9;
	for (int i = 0; i < 9; i++) {
		current_facelet_cube[start_cell_idx + i] = facelet_face[i];
	}

	std::cout << "Current facelet cube: ";
	for (int i = 0; i < 54; ++i) {
		std::cout << current_facelet_cube[i] << " ";
	}
	std::cout << std::endl;
}

void CubeVision::reset() {
	stable_frames = 0;
	prev_rect = std::nullopt;
	current_facelet_cube = FaceletCube();
	for (int i = 0; i < 54; ++i) {
		current_facelet_cube[i] = UNKNOWN_COLOR;
	}
}