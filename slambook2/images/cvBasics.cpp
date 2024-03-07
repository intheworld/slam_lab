#include <iostream>
#include <chrono>

using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "filename not provided." << endl;
        return EXIT_FAILURE;
    }
    cv::Mat image;
    image = cv::imread(argv[1]);

    if (image.data == nullptr) {
        cerr << "file:" << argv[1] << "not exist." << endl;
        return EXIT_FAILURE;
    }

    cout << "width: " << image.cols << ", height: " << image.rows
        << ", channel: " << image.channels() << endl;

    cv::imshow("image", image);
    cv::waitKey(0);

    if (image.type() != CV_8UC1 && image.type() != CV_8UC3) {
        // 图像类型不符合要求
        cout << "请输入一张彩色图或灰度图." << endl;
        return 0;
    }

    // clone image
    cv::Mat imageClone = image.clone();
    imageClone(cv::Rect(0, 0, 100, 100)).setTo(0);

    cv::imshow("image", image);
    cv::imshow("imageClone", imageClone);
    cv::waitKey(0);

    cv::destroyAllWindows();
    return EXIT_SUCCESS;
}