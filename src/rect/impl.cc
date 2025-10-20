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
    threshold(blurred, binary, 127, 255, THRESH_BINARY);
    findContours(binary, contours,hierarchy,RETR_EXTERNAL, CHAIN_APPROX_NONE);
    //imshow("binary", binary);

    int total_pixels = binary.total();
    for(int i = 0; i <contours.size(); ++i){
        double epsilon = 0.02 * arcLength(contours[i], true);
        vector<Point> rectangle_contours_temp;
        approxPolyDP(contours[i], rectangle_contours_temp, epsilon, true);  
        if(rectangle_contours_temp.size() == 4 && isContourConvex(rectangle_contours_temp) && contourArea(rectangle_contours_temp) < total_pixels * 0.99){  //顶点为4，凸多边形，去除整体

            rectangle_contours.push_back(rectangle_contours_temp);
        }

    }

    // 在筛选循环中添加详细调试
    /*cout << "=== 详细调试信息 ===" << endl;
    cout << "初始轮廓数量: " << contours.size() << endl;
    for(int i = 0; i < contours.size(); ++i){
        double epsilon = 0.02 * arcLength(contours[i], true);
        vector<Point> approx;
        approxPolyDP(contours[i], approx, epsilon, true);
        // 打印每个轮廓的详细信息
        cout << "轮廓 " << i << ": 顶点数=" << approx.size() 
            << ", 凸性=" << (isContourConvex(approx) ? "是" : "否")
            << ", 面积=" << contourArea(approx) 
            << ", 面积阈值=" << (contourArea(approx) < total_pixels * 0.9 ? "通过" : "不通过") << endl;
        if(approx.size() == 4 && isContourConvex(approx) && 
            contourArea(approx) < total_pixels * 0.9){
            rectangle_contours.push_back(approx);
            cout << "  --> 符合条件，已添加到rectangle_contours!" << endl;
        }
    }
    cout << "最终rectangle_contours大小: " << rectangle_contours.size() << endl;
    // 验证绘制的是哪个变量
    cout << "即将绘制的轮廓变量大小: " << rectangle_contours.size() << endl;
    // 创建副本进行绘制（避免修改原始图像）
    Mat display_image = input.clone();
    drawContours(display_image, rectangle_contours, -1, Scalar(0, 0, 255), 2);
    imshow("contours", display_image);
    waitKey(100); // 短暂显示
    cout << "绘制完成后的rectangle_contours大小: " << rectangle_contours.size() << endl;*/


    Mat display_image = input.clone();  // 创建输入图像的副本
    drawContours(display_image, rectangle_contours, -1, Scalar(0, 0, 255), 2);
    imshow("contours", display_image);
    int a = rectangle_contours.size();
    cout << "矩形轮廓数量: " << a << endl;

    if(rectangle_contours.empty()) {
        // 没有找到符合条件的矩形轮廓，返回默认值或进行错误处理
        cout << "警告：未找到任何矩形轮廓！" << endl;
        return make_pair(Rect(), RotatedRect());
    }


    Rect bounding_Rect = boundingRect(rectangle_contours[0]);
    RotatedRect minArea_Rect = minAreaRect(rectangle_contours[0]);

    res = make_pair(bounding_Rect , minArea_Rect);


    

    return res;
}