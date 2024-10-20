#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;


void find_feature_matches(
    const Mat &img_1, const Mat &img_2,
    std::vector<KeyPoint> &keypoints_1,
    std::vector<KeyPoint> &keypoints_2,
    std::vector<DMatch> &matches
);


void pos_estimation_2d2d(
    std::vector<KeyPoint> keypoints_1,
    std::vector<KeyPoint> keypoints_2,
    std::vector<DMatch> matches,
    Mat &R, Mat &t
);

Point2d pixel2cam(const Point2d &p, const Mat &K);

int main(int argc, char **argv) {
    if (argc != 3) {
        cout << "usage: pos_estimation_2d2d img1 img2" << endl;
        return 1;
    }
    // 读取文件
    Mat img1 = imread(argv[1], IMREAD_COLOR);
    Mat img2 = imread(argv[2], IMREAD_COLOR);
    assert(img1.data && img2.data && "Can not load images!");

    vector<KeyPoint> keypoints_1, keypoints_2;
    vector<DMatch> matches;
    find_feature_matches(img1, img2, keypoints_1, keypoints_2, matches);
    cout << "一共找到了" << matches.size() << "组匹配点" << endl;

    // 运动估计
    Mat R, t;
    pos_estimation_2d2d(keypoints_1, keypoints_2, matches, R, t);

    // 验证 E = t^R*scale
    Mat t_x = (Mat_<double>(3, 3) << 0, -t.at<double>(2, 0), t.at<double>(1, 0),
        t.at<double>(2, 0), 0, -t.at<double>(0, 0),
        -t.at<double>(1, 0), t.at<double>(0, 0), 0);

    cout << "t^R=" << endl << t_x * R << endl;

    // 验证对极约束
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    for (DMatch m: matches) {
        Point2d pt1 = pixel2cam(keypoints_1[m.queryIdx].pt, K);
        Mat y1 = (Mat_<double>(3, 1) << pt1.x, pt1.y, 1);
        Point2d pt2 = pixel2cam(keypoints_2[m.queryIdx].pt, K);
        Mat y2 = (Mat_<double>(3, 1) << pt2.x, pt2.y, 1);
        Mat d = y2.t() * t_x * R * y1;
        cout << "epipolar constraint = " << d << endl;
    }
    return 0;
}


void find_feature_matches(
    const Mat &img_1, const Mat &img_2,
    std::vector<KeyPoint> &keypoints_1,
    std::vector<KeyPoint> &keypoints_2,
    std::vector<DMatch> &matches
) {
  //-- 初始化
  Mat descriptors_1, descriptors_2;
  Ptr<FeatureDetector> detector = ORB::create();
  Ptr<DescriptorExtractor> descriptor = ORB::create();
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
  //-- 第一步:检测 Oriented FAST 角点位置
  detector->detect(img_1, keypoints_1);
  detector->detect(img_2, keypoints_2);

  //-- 第二步:根据角点位置计算 BRIEF 描述子
  descriptor->compute(img_1, keypoints_1, descriptors_1);
  descriptor->compute(img_2, keypoints_2, descriptors_2);

  //-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
  vector<DMatch> match;
  //BFMatcher matcher ( NORM_HAMMING );
  matcher->match(descriptors_1, descriptors_2, match);

  //-- 第四步:匹配点对筛选
  double min_dist = 10000, max_dist = 0;

  //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
  for (int i = 0; i < descriptors_1.rows; i++) {
    double dist = match[i].distance;
    if (dist < min_dist) min_dist = dist;
    if (dist > max_dist) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist);
  printf("-- Min dist : %f \n", min_dist);

  //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.
  for (int i = 0; i < descriptors_1.rows; i++) {
    if (match[i].distance <= max(2 * min_dist, 30.0)) {
      matches.push_back(match[i]);
    }
  }
}


void pos_estimation_2d2d(
    std::vector<KeyPoint> keypoints_1,
    std::vector<KeyPoint> keypoints_2,
    std::vector<DMatch> matches,
    Mat &R, Mat &t
) {
    Mat K = (Mat_<double>(3, 3) << 520.9, 0, 325.1, 0, 521.0, 249.7, 0, 0, 1);

    vector<Point2f> points1;
    vector<Point2f> points2;

    for (int i = 0; i < (int) matches.size(); i++) {
      points1.push_back(keypoints_1[matches[i].queryIdx].pt);
      points2.push_back(keypoints_2[matches[i].queryIdx].pt);
    }

    // 基础矩阵
    Mat fundamental_maxtrix;
    fundamental_maxtrix = findFundamentalMat(points1, points2, FM_8POINT);
    cout << "fundamental_matrix is" << endl << fundamental_maxtrix << endl;
    
    //相机光心
    Point2d principal_point(K.at<double>(0, 2), K.at<double>(1, 2));
    //相机焦距
    double focal_length = K.at<double>(1, 1);
    Mat essential_matrix = findEssentialMat(points1, points2, focal_length, principal_point);
    cout << "essential_matrix is " << endl << essential_matrix << endl;

    //恢复旋转和平移信息
    recoverPose(essential_matrix, points1, points2, R, t, focal_length, principal_point);
    cout << "R is " << endl << R << endl;
    cout << "t is " << endl << t << endl;

    // 单应矩阵
    Mat homegraphy_matrix = findHomography(points1, points2, RANSAC, 3);
    cout << "homegraphy_matrix is " << endl << essential_matrix << endl;
}

/**
 * Cx = (Px - u0) / fx
 * Cy = (Py - v0) / fy
 */
Point2d pixel2cam(const Point2d &p, const Mat &K) {
  return Point2d(
      (p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
      (p.y - K.at<double>(1, 2)) / K.at<double>(1, 1)
    );
}

