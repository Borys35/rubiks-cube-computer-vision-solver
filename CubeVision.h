#pragma once
#include <opencv2/opencv.hpp>
#include "include/color.hpp"

class CubeVision
{
public:
	typedef struct {
		uint8_t min;
		uint8_t max;
	} Range;

	typedef struct {
		Range y_range;
		Range u_range;
		Range v_range;
	} ColorRange;

	typedef struct {
		uint8_t b;
		uint8_t g;
		uint8_t r;
	} BgrColor;

	struct Config {
		std::array<ColorRange, COLOR_COUNT> color_ranges;
		std::array<BgrColor, COLOR_COUNT> display_colors;
		uint stable_frame_count = 5;
		float iou_threshold = 0.7f;
		float min_area_ratio = 0.1f;
		float min_solidity = 0.8f;
		float min_rectangularity = 0.7f;
		float min_cell_ratio = 0.3f;
		float max_angle = 5.0f;
	};

	CubeVision(const Config& config) : cfg(config) {}

	cv::Mat normalizeYuv(const cv::Mat& bgr_frame); // Returns YUV frame with normalized U and V channels
	std::array<cv::Mat, COLOR_COUNT> detectCellColors(const cv::Mat& yuv_frame, cv::Mat& combined_mask); // Returns a color-coded frame. Can be converted to a combined mask
	std::optional<cv::Rect> extractCubeFaceRect(const cv::Mat& combined_mask, std::array<cv::Mat, COLOR_COUNT> masks); // Returns BoundingBox of the detected cube face
	void registerRect(const std::optional<cv::Rect>& rect); // Registers a rect
private:
	Config cfg;

	uint stable_frames;
	std::optional<cv::Rect> prev_rect;

	float iou(const cv::Rect& rect1, const cv::Rect& rect2);
};

