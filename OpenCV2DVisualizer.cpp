#include "OpenCV2DVisualizer.h"

void OpenCV2DVisualizer::display_cube(const FaceletCube& fc) const {
    cv::Mat& canvas = const_cast<cv::Mat&>(this->mat);
    canvas.setTo(cv::Scalar(0, 0, 0));

    int facelet_size = 40;
    int start_x = (canvas.cols - 12 * facelet_size) / 2;
    int start_y = (canvas.rows - 9 * facelet_size) / 2;

    int grid_offsets[6][2] = { {1, 0}, {2, 1}, {1, 1}, {1, 2}, {0, 1}, {3, 1} };

    for (int face = 0; face < 6; ++face) {
        for (int i = 0; i < 9; ++i) {
            Color color_enum = fc[face * 9 + i];
            int color_idx = (color_enum == UNKNOWN_COLOR) ? 6 : color_enum;

            auto c = this->display_colors[color_idx];
            cv::Scalar cv_color(c.b, c.g, c.r);

            int x = start_x + (grid_offsets[face][0] * 3 + (i % 3)) * facelet_size;
            int y = start_y + (grid_offsets[face][1] * 3 + (i / 3)) * facelet_size;

            cv::Rect rect(x, y, facelet_size, facelet_size);
            cv::rectangle(canvas, rect, cv_color, cv::FILLED);
            cv::rectangle(canvas, rect, cv::Scalar(0, 0, 0), 3);
        }
    }
}

cv::Mat OpenCV2DVisualizer::get_mat() {
    return mat;
}