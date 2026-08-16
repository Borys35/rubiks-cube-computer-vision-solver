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
	std::array<cv::Mat, COLOR_COUNT> masks;
	for (int i = 0; i < COLOR_COUNT; ++i)
	{
		const ColorRange& range = cfg.color_ranges[i];
		cv::inRange(yuv_frame,
			cv::Scalar(range.y_range.min, range.u_range.min, range.v_range.min),
			cv::Scalar(range.y_range.max, range.u_range.max, range.v_range.max),
			masks[i]);
		cv::Mat erode_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
		cv::erode(masks[i], masks[i], erode_kernel);
		cv::dilate(masks[i], masks[i], dilate_kernel);

		combined_mask |= masks[i];
		masks[i].setTo(cv::Scalar(cfg.display_colors[i].b, cfg.display_colors[i].g, cfg.display_colors[i].r), masks[i]);
	}
	
	return masks;
}

//cv::Mat CubeVision::makeCombinedColorMask(const cv::Mat& yuv_norm)
//{
//	cv::Mat mask_red, mask_orange, mask_blue, mask_green, mask_white, mask_yellow;
//	// Red: Y[50-255], U'[128±10 -> 118-138], V'[128±10 -> 118-138]
//	cv::inRange(yuv_norm, cv::Scalar(50, 118, 118), cv::Scalar(255, 138, 138), mask_red);
//	// Orange: Y[50-255], U'[128±20 -> 108-148], V'[128±20 -> 108-148]
//	cv::inRange(yuv_norm, cv::Scalar(50, 108, 108), cv::Scalar(255, 148, 148), mask_orange);
//	// Blue: Y[50-255], U'[128±15 -> 113-143], V'[128±15 -> 113-143]
//	cv::inRange(yuv_norm, cv::Scalar(50, 113, 113), cv::Scalar(255, 143, 143), mask_blue);
//	// Green: Y[50-255], U'[128±15 -> 113-143], V'[128±15 -> 113-143]
//	cv::inRange(yuv_norm, cv::Scalar(50, 113, 113), cv::Scalar(255, 143, 143), mask_green);
//	// White: Y[190-255], U'[128±12 -> 116-140], V'[128±11 -> 117-139]
//	cv::inRange(yuv_norm, cv::Scalar(190, 116, 117), cv::Scalar(255, 140, 139), mask_white);
//	// Yellow: Y[118-150], U'[90±25 -> 65-115], V'[140±20 -> 120-160]
//	cv::inRange(yuv_norm, cv::Scalar(118, 65, 120), cv::Scalar(150, 115, 160), mask_yellow);
//	// Combine all color masks into a single mask for cell detection
//	cv::Mat combined_mask = mask_red | mask_orange | mask_blue | mask_green | mask_white | mask_yellow;
//	return combined_mask;
//}