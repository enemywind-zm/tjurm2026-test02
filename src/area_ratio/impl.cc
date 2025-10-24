#include "impls.h"


float compute_area_ratio(const std::vector<cv::Point>& contour) {
    /**
     * 要求：
     *      计算输入的轮廓的面积与它的最小外接矩形面积的比例。自行查找
     * 获得轮廓面积的函数。这个量可以用来判断一个轮廓到底有多靠近一个矩形。
     * 
     * 提示：
     * 无。
     * 
     * 通过条件:
     * 运行测试点，通过即可。
     */
    double contour_area = cv::contourArea(contour);
    cv::RotatedRect min_rect = cv::minAreaRect(contour);
    double min_rect_area = min_rect.size.width * min_rect.size.height;
    if (min_rect_area <= 0) {
        return 0.f;
    }
    float area_ratio = static_cast<float>(contour_area / min_rect_area);
    return area_ratio;
}