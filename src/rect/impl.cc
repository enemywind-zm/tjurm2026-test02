#include "impls.h"


std::pair<cv::Rect, cv::RotatedRect> get_rect_by_contours(const cv::Mat& input) {
    /**
     * 要求：
     * 使用findCountours函数找出输入图像（彩色图像）中的矩形轮廓，并且返回它的外接矩形，以及
     * 最小面积外接矩形。所需要的函数自行查找。
     * 
     * 通过条件：
     * 运行测试点，你找到的矩形跟答案一样就行。
    */
    std::pair<cv::Rect, cv::RotatedRect> res;
    // IMPLEMENT YOUR CODE HERE
    using namespace cv;
    using namespace std;

    //imshow("input", input);

    Mat gray, binary, blurred;  //灰度图，二值图，模糊图
    vector<vector<Point>> contours; //存放轮廓点
    vector<vector<Point>> rectangle_contours; //存放矩形轮廓点
    vector<Vec4i> hierarchy;  //存放轮廓的层次结构

    cvtColor(input, gray, COLOR_BGR2GRAY);  
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    threshold(blurred, binary, 127, 255, THRESH_BINARY);  //二值化图像

    //imshow("binary", binary);

    findContours(binary,contours,hierarchy,RETR_TREE, CHAIN_APPROX_NONE);

    //cout << "轮廓数量: " << contours.size() << endl;

    int total_pixels = binary.total();

    //printf("Total pixels: %d\n", total_pixels);

    /*
    Mat display_image = input.clone();  // 创建输入图像的副本
    drawContours(display_image, contours, 0, Scalar(255, 0, 0), 2);
    drawContours(display_image, contours, 1, Scalar(0, 255, 0), 2);
    drawContours(display_image, contours, 2, Scalar(0, 255, 255), 2);
    imshow("contours", display_image);
    */

    for(int i = 0; i <contours.size(); ++i){
        double epsilon = 0.02 * arcLength(contours[i], true);
        vector<Point> rectangle_contours_temp;
        approxPolyDP(contours[i], rectangle_contours_temp, epsilon, true);  
        if(rectangle_contours_temp.size() == 4 && isContourConvex(rectangle_contours_temp) && contourArea(rectangle_contours_temp) < total_pixels * 0.90){  //顶点为4，凸多边形，去除整体

            rectangle_contours.push_back(rectangle_contours_temp);
        }

    }

    if(rectangle_contours.empty()) {
        cout << "警告：未找到任何矩形轮廓！" << endl;
        return make_pair(Rect(), RotatedRect());
    }


    Rect bounding_Rect = boundingRect(rectangle_contours[0]);
    RotatedRect minArea_Rect = minAreaRect(rectangle_contours[0]);

    res = make_pair(bounding_Rect , minArea_Rect);


 

    return res;
}