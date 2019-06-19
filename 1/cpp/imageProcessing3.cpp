#include "stdafx.h"
#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <math.h>
#include <map>

using namespace cv;
using namespace std;

Mat g_imgSrc, g_imgDst, g_imgMask, g_imgRound, g_imgAnnular,
	g_imgBlur, g_imgCanny, g_imgMorph, g_imgThreshod;

const char* g_imgName[] = {"3.jpg"};

map<char*, char*> g_mapWinName;
map<char*, char*> g_mapTrackbarName;

/* ksize */
static int g_nTrackbarValue0 = 1;
static const int g_nTrackbarCount0 = 25;
/* canny min threshold */
static int g_nTrackbarValue1 = 100;
static const int g_nTrackbarCount1 = 500;
/* canny max threshold */
static int g_nTrackbarValue2 = 215;
static const int g_nTrackbarCount2 = 1000;
/* morphology */
static int g_nTrackbarValue3 = 3;
static const int g_nTrackbarCount3 = 8;
/* adaptive threshold */
static int g_nTrackbarValue4 = 20;
static const int g_nTrackbarCount4 = 100;
static int g_nTrackbarValue5 = 1100;
static const int g_nTrackbarCount5 = 3000;
/* get good match */
static int g_nTrackbarValue6 = 250;
static const int g_nTrackbarCount6 = 500;

/* ksize */
static int& g_nKSize = g_nTrackbarValue0;
/* canny min/max threshold */
static int& g_nCannyMinThreshold = g_nTrackbarValue1;
static int& g_nCannyMaxThreshold = g_nTrackbarValue2;
/* morphology */
static int& g_nMorphologyOption = g_nTrackbarValue3;
/* adaptive threshold */
static int& g_nThresholdBSize = g_nTrackbarValue4;
static int& g_nThresholdMeanC = g_nTrackbarValue5;
/* get good match */
static int& g_nGoodMatchDistance = g_nTrackbarValue6;



Vec3f g_v3fDescriptorRoundnessRate;
vector<vector<Point>> g_vvpContours;
map<int, int> g_mArea;
map<int, Point> g_mCentroid;
Vec3f g_v3fLeastSquaresFitting;
map<int, int> g_mapProject;

template<typename Tkey, typename Tvalue> class sortMapByValue{
	/**
	bool operator()(const pair<Tkey, Tvalue>& left, const pair<Tkey, Tvalue>& right)const{
		cout << "pair" << endl;
		return left.second < right.second;
	}
	/**
	bool operator()(const Tvalue& left, const Tvalue& right){
		cout << "value" << endl;
		return left < right;
	}
	/**/
};
struct sortByValue{
	bool operator()(const pair<int, double>& left, const pair<int, double>& right){
		cout<<"pair"<<endl;
		return left.second < right.second;
	}
	/**
	bool operator()(const int& left, const int& right){
		cout << "int";
		return left < right;
	}
	/**/
	bool operator()(const double& left, const double& right){
		cout << "double";
		return left > right;
	}
	/**/
};

void init();
template<typename T, typename T1> void getMinMax(T& src, int& minIndex, int& maxIndex);
void getDescriptorRoundnessRate(Mat& imgsrc, Vec3f& descriptorRoundnessRate);
void getMask(Mat& imgsrc, Mat& imgdst);
void getRoundTarget(Mat& imgsrc, Mat& imgdst, Mat& imgmask);
void myMorphologyEx(Mat& imgsrc, Mat& imgdst, int option);
void drawHistogram(const Mat& imgsrc, MatND& hist, int histSize[], Mat& imgHist);
template<typename T, typename Tf, typename Ti> void getTop3(T& src, Vec4i& top3Index, int size);
void getDescriptorArea(Mat& imgsrc, Vec2f& range);
void getDescriptorArea1(Mat& imgsrc, vector<vector<Point>>& contours, map<int, int>& mapArea);
void getCentroid(vector<vector<Point>>& contours, map<int, int>& mapArea, map<int, Point>& centroid);
void leastSquaresFitting(map<int, Point>& mapcentroid, Vec3f& leastsquaresfitting);
void leastSquaresFittingContour(vector<Point>& contour, Vec3f& v3fleastsquaresfittingcontour);
void leastSquaresFittingCircle(map<int, Point>& mapcentroid, Vec3f& v3fLeastsquaresfittingcircle);
void getDescriptorPointer(Mat& imgsrc, Mat& imgdst);
void getAngleFromTwoPoints(float& angle, Point& pointStart, Point& pointEnd);
void project(Mat& imgsrc, map<int, int>& mapProj, Point& center, int psize);
void drawHistogramFromUserData(MatND& imgsrc, Mat& imgshow);
void getAnnularMask(Mat& mask);
void getAnnularTarget(Mat& imgsrc, Mat& imgdst, Mat& mask);
void getResult();

void getTemplate();
void getDescriptorAndMatch(Mat& imgTempate, Mat& imgSample);
void registration(Mat& imgsrc, Mat& imgdst, Mat& imgTemplate);
void testPerspective();
/* ksize */
void trackbarCallback0(int trackbarValue, void* userdata);
void trackbar0(int value, Mat& userdataimg, Mat& imgdst);
/* canny */
void trackbarCallback1(int trackbarValue, void* userdata);
void trackbar1(int value, Mat& userdataimg, Mat& imgdst);
void trackbarCallback2(int trackbarValue, void* userdata);
void trackbar2(int value, Mat& userdataimg, Mat& imgdst);
/* morphology */
void trackbarCallback3(int trackbarValue, void* userdata);
void trackbar3(int value, Mat& userdataimg, Mat& imgdst);
/* threshold */
void trackbarCallback4(int trackbarValue, void* userdata);
void trackbar4(int value, Mat& userdataimg, Mat& imgdst);
void trackbarCallback5(int trackbarValue, void* userdata);
void trackbar5(int value, Mat& userdataimg, Mat& imgdst);
/* good match */
void trackbarCallback6(int trackbarValue, void* userdata);
void trackbar6(int value, Mat& userdataimg);
/**/
void mouseCallback(int event, int x, int y, int flags, void* userdata);


void main(){
	/**
	test();
	/**
	testHist();
	/**
	init();
	/**
	testProjectDirectly();
	/**
	testDrawHistogramFromUserData();
	/**
	testGetContours();
	/**
	test();
	/**/
	namedWindow("winCut", CV_WINDOW_AUTOSIZE);
	/**
	cvResizeWindow("winCut", 400, 400);
	/**/
	VideoCapture videoCapture;
	videoCapture.open(0);
	Mat frame;
	Mat imgSample, imgTempate, imgDst;
	char key = 0;
	setMouseCallback("winCut", mouseCallback, (void*)&videoCapture);
	createTrackbar("goodMatchDistance", "winCut", &g_nTrackbarValue6, g_nTrackbarCount6, trackbarCallback6, 0);
	while(1){
		key = waitKey(50);
		videoCapture>>frame;
		imshow("winCut", frame);
		switch(key){
			case '1':
				imwrite("./template.jpg", frame);
				cout<<"get template.jpg"<<endl;
				imshow("template", frame);
				break;
			case '2':
				imwrite("./sample.jpg", frame);
				cout << "get sample.jpg" << endl;
				imshow("sample", frame);
				break;
			case '3':
				imgSample = imread("./sample.jpg", 0);
				imgTempate = imread("./template.jpg", 0);
				if(!imgSample.data || !imgTempate.data){
					break;
				}
				imshow("sample", imgSample);
				imshow("template", imgTempate);
				/**/
				getDescriptorAndMatch(imgTempate, imgSample);
				/**
				registration(imgSample, imgDst, imgTempate);
				/**/
				break;
			case '4':
				testPerspective();
		}
		if(key == 'q'){
			break;
		}
	}
	/**/
	waitKey();
}

void init(){
	g_imgSrc = imread(g_imgName[0], 0);
	CV_Assert(g_imgSrc.data);
	g_imgDst = g_imgSrc.clone();
	g_imgMask = g_imgSrc.clone();
	g_imgRound = g_imgSrc.clone();
	g_imgBlur = g_imgSrc.clone();
	g_imgCanny = g_imgSrc.clone();
	g_imgThreshod = g_imgSrc.clone();
	/* win name */
	g_mapWinName.insert(pair<char*, char*>("winControl", "winControl"));
	g_mapWinName.insert(pair<char*, char*>("imgSrc", "imgSrc"));
	g_mapWinName.insert(pair<char*, char*>("imgDst", "imgDst"));
	g_mapWinName.insert(pair<char*, char*>("imgBlur", "imgBlur"));
	g_mapWinName.insert(pair<char*, char*>("imgCanny", "imgCanny"));
	g_mapWinName.insert(pair<char*, char*>("imgThreshold", "imgThreshold"));
	/* trackbar name */
	g_mapTrackbarName.insert(pair<char*, char*>("ksize", "ksize"));
	g_mapTrackbarName.insert(pair<char*, char*>("cannyMin", "cannyMin"));
	g_mapTrackbarName.insert(pair<char*, char*>("cannyMax", "cannyMax"));
	g_mapTrackbarName.insert(pair<char*, char*>("morphOp", "morphOp"));
	g_mapTrackbarName.insert(pair<char*, char*>("thresBSize", "thresBSize"));
	g_mapTrackbarName.insert(pair<char*, char*>("thresMeanC", "thresMeanC"));

	/**/
	namedWindow(g_mapWinName.at("winControl"), WINDOW_NORMAL);
	cvResizeWindow(g_mapWinName.at("winControl"), 400, 600);

	/* blur */
	createTrackbar(g_mapTrackbarName.at("ksize"), g_mapWinName.at("winControl"),
		&g_nTrackbarValue0, g_nTrackbarCount0, trackbarCallback0, (void*)&g_imgSrc);
	/* canny */
	createTrackbar(g_mapTrackbarName.at("cannyMin"), g_mapWinName.at("winControl"),
		&g_nTrackbarValue1, g_nTrackbarCount1, trackbarCallback1, (void*)&g_imgBlur);
	createTrackbar(g_mapTrackbarName.at("cannyMax"), g_mapWinName.at("winControl"),
		&g_nTrackbarValue2, g_nTrackbarCount2, trackbarCallback2, (void*)&g_imgBlur);
	/* morphology */
	createTrackbar(g_mapTrackbarName.at("morphOp"), g_mapWinName.at("winControl"),
		&g_nTrackbarValue3, g_nTrackbarCount3, trackbarCallback3, (void*)&g_imgCanny);
	/* adaptive threshold */
	createTrackbar(g_mapTrackbarName.at("thresBSize"), g_mapWinName.at("winControl"),
		&g_nTrackbarValue4, g_nTrackbarCount4, trackbarCallback4, (void*)&g_imgBlur);
	createTrackbar(g_mapTrackbarName.at("thresMeanC"), g_mapWinName.at("winControl"),
		&g_nTrackbarValue5, g_nTrackbarCount5, trackbarCallback5, (void*)&g_imgBlur);

	/* blur */
	trackbarCallback0(g_nTrackbarValue0, (void*)&g_imgSrc);
	/* canny *
	trackbarCallback1(g_nTrackbarValue1, (void*)&g_imgBlur);
	/* morphology *
	trackbarCallback3(g_nTrackbarValue3, (void*)&g_imgCanny); 
	/* threshold */
	trackbarCallback4(g_nTrackbarValue4, (void*)&g_imgBlur);
	/**/
	getDescriptorRoundnessRate(g_imgThreshod, g_v3fDescriptorRoundnessRate);
	/**/
	getMask(g_imgThreshod, g_imgMask);
	getRoundTarget(g_imgThreshod, g_imgRound, g_imgMask);
	/**
	getDescriptorArea1(g_imgRound, g_vvpContours, g_mArea);
	/**/
	getAnnularMask(g_imgMask);
	getAnnularTarget(g_imgRound, g_imgAnnular, g_imgMask);
	/**/
	getResult();
	/**
	leastSquaresFitting(g_mCentroid, g_v3fLeastSquaresFitting);
	/**
	getDescriptorPointer(g_imgRound, g_fAngle);
	imshow(g_mapWinName.at("imgSrc"), g_imgSrc);
	imshow(g_mapWinName.at("imgDst"), g_imgDst);
	/**/
}
/**/
template<typename T, typename T1> void getMinMax(T& src, int& minIndex, int& maxIndex){
	T::iterator it = src.begin();
	T1 max = (*it).second;
	T1 min = (*it).second;
	for( ; it!=src.end(); it++){
		if((*it).second >= max){
			max = (*it).second;
			maxIndex = (*it).first;
		}
		if((*it).second <= min){
			min = (*it).second;
			minIndex = (*it).first;
		}
	}
}
/* 检测 imgsrc 的轮廓中圆度率最大的轮廓，获得其圆心和半径 */
void getDescriptorRoundnessRate(Mat& imgsrc, Vec3f& descriptorroundnessrate){
	/* 检测轮廓中圆度率最大的轮廓，获得其圆心和半径 */
	double area = 0, arcLen = 0;
	map<int, double> roundnessRate;
	int maxRoundnessIndex = 0;
	int minRoundnessIndex = 0;
	Mat tempImg = Mat::zeros(imgsrc.size(), imgsrc.type());
	Vec3f v3fLeastSquaresFittingContour;
	vector<vector<Point>> contours;
	findContours(imgsrc, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	cout<<"\ngetDescriptorRoundnessRate contours.size():"<<contours.size()<<endl;
	/**/
	for(int i = 0; i < contours.size(); i++){
		area = contourArea(contours[i], false);
		arcLen = arcLength(contours[i], true);
		if(area < 300 || arcLen<200){
			continue;
		}
		roundnessRate.insert( pair<int, double>(i, 4*CV_PI*area/pow(arcLen, 2)));
	}
	/**/
	cout << "roundnessRate.size() = " << roundnessRate.size() << endl;
	/**/
	for(map<int, double>::iterator it=roundnessRate.begin(); it!= roundnessRate.end(); it++){
		cout<<(*it).first<<" 轮廓的圆度率: "<< (*it).second <<endl;
	}
	/**/
	getMinMax<map<int, double>, double>(roundnessRate, minRoundnessIndex, maxRoundnessIndex);
	cout<<"maxRoundnessIndex:"<<maxRoundnessIndex<<endl;
	cout << "minRoundnessIndex:" << minRoundnessIndex << endl;
	/**/
	drawContours(tempImg, contours, maxRoundnessIndex, Scalar(255, 255, 255));
	drawContours(tempImg, contours, minRoundnessIndex, Scalar(255, 255, 255));
	imshow("getDescriptorRoundnessRate", tempImg);
	/* 用hough检测最大圆度率的轮廓的圆心和半径，失败 *
	HoughCircles(tempImg, circles, HOUGH_GRADIENT, 4, 100, 100, 200, 100, 500);
	cout<<"circles.size():"<<circles.size()<<endl;
	for(int i = 0; i < circles.size(); i++){
		Point center1 = Point(circles[i][0], circles[i][1]);
		circle(tempImg, center1, circles[i][2], Scalar(255,255,255));
		imshow("getRoundnessCircles", tempImg);
		waitKey(10);
	}
	/**/
	leastSquaresFittingContour(contours[maxRoundnessIndex], v3fLeastSquaresFittingContour);
	cout<<"getDescriptorRoundnessRate v3fLeastSFContour"<<v3fLeastSquaresFittingContour<<endl;
	/* 显示由最大圆度率轮廓检测到的圆，与原轮廓对比 */
	 circle(tempImg, Point2f(v3fLeastSquaresFittingContour[0], v3fLeastSquaresFittingContour[1]),
		v3fLeastSquaresFittingContour[2], Scalar::all(100));
	circle(tempImg, Point2f(v3fLeastSquaresFittingContour[0], v3fLeastSquaresFittingContour[1]),
		3, Scalar::all(100));
	imshow("getDescriptorRoundnessRate", tempImg);
	/**/
	descriptorroundnessrate = v3fLeastSquaresFittingContour;
	/**/
}
/**/
void getMask(Mat& imgsrc, Mat& imgdst){
	Mat tempImg = Mat::zeros(imgsrc.size(), imgsrc.type());
	Point center = Point(g_v3fDescriptorRoundnessRate[0], g_v3fDescriptorRoundnessRate[1]);
	Scalar scalar = Scalar(255, 255, 255);
	circle(tempImg, center, g_v3fDescriptorRoundnessRate[2], scalar);
	floodFill(tempImg, center, scalar);
	imgdst = tempImg;
	/**/
	imshow("getMask", imgdst);
	/**/
}
/**/
void getRoundTarget(Mat& imgsrc, Mat& imgdst, Mat& imgmask){
	Mat tempImg = Mat::zeros(imgsrc.size(), imgsrc.type());
	imgsrc.copyTo(tempImg, imgmask);
	imgdst = tempImg;
	/**/
	imshow("getRoundTarget", imgdst);
	/**/
}
/**/
void myMorphologyEx(Mat& imgsrc, Mat& imgdst, int option){
	/**/
	int nKSize = 2*g_nKSize+1;
	if(nKSize < 3){
		nKSize = 3;
	}
	Size kSize = Size(nKSize, nKSize);
	Mat element = getStructuringElement(MORPH_RECT, kSize);
	morphologyEx(imgsrc, imgdst, option, element);
	/**/
	imshow("morphology", imgdst);
	/**/
}
/**/
void drawHistogram(const Mat& imgsrc, MatND& hist, int histSize[], Mat& imgHist){
	/* 清理imgHist，避免上次的干扰 */
	imgHist = Mat(Scalar(0));
	int histScale = 1;
	int nImg = 1;
	int channel[] = {0};
	int histDim = 1;
	float histRange1[] = {0, 256};
	const float* histRange[] = {histRange1};
	// create(rows, cols, type)
	imgHist.create(histRange[0][1], histSize[0] * histScale, CV_8U);
	calcHist(&imgsrc, nImg, channel, Mat(), hist, histDim, histSize, histRange, true, false);
	double histMinValue = 0, histMaxValue = 0;
	minMaxLoc(hist, &histMinValue, &histMaxValue);
	for(int i = 0; i < histSize[0]; i++){
		/**
		int binValue = *(hist.data + hist.step[0]*i + hist.step[1]*0);
		/**/
		int binValue = hist.at<float>(i);
		int binIntensity = histRange[0][1] * binValue / histMaxValue;
		rectangle(imgHist, Point(i*histScale, histRange[0][1] - 1), 
			Point((i + 1)*histScale - 1, histRange[0][1] - binIntensity), Scalar(123, 234, 99));
	}
	imshow("hist", imgHist);
}
/**/
template<typename T, typename Tf, typename Ti> void getTop3(T& src, Vec4i& top3Index, int size){
	Ti top0 = src.at<Tf>(0), top1 = src.at<Tf>(0), top2 = src.at<Tf>(0), top3 = src.at<Tf>(0);
	int index0 = 0, index1 = 0, index2 = 0, index3 = 0;
	Ti element;
	for(int i = 0; i < size; i++){
		element = src.at<Tf>(i);
		if(element > top0 && element > top1 && element > top2 && element>top3){
			top0 = element;
			index0 = i;
		}
		else if(element < top0 && element > top1 && element > top2 && element>top3){
			top1 = element;
			index1 = i;
		}
		else if(element < top0 && element < top1 && element > top2 && element>top3){
			top2 = element;
			index2 = i;
		}
		else if(element < top0 && element < top1 && element < top2 && element>top3){
			top3 = element;
			index3 = i;
		}
	}
	top3Index[0] = top0;
	top3Index[1] = top1;
	top3Index[2] = top2;
	top3Index[3] = top3;
	cout<<"top0("<<index0<<") = "<<top3Index[0]<<"\ntop1("<<index1<<") = "<<top3Index[1]<<"\ntop2("<<index2<<") = "<<top3Index[2]
		<< "\ntop3(" << index3 << ") = " << top3Index[3]<<endl;
}
/**/
void getDescriptorArea(Mat& imgsrc, Vec2f& range){
	int area = 0;
	int maxAreaIndex = 0;
	int minAreaIndex = 0;
	Mat tempImg = Mat::zeros(imgsrc.size(), imgsrc.type());
	cout<<"imgsrc.type() = "<< imgsrc.type()<<endl;
	cout << "imgsrc.step[0] = " << imgsrc.step[0] << endl;
	cout << "imgsrc.step[1] = " << imgsrc.step[1] << endl;
	cout << "imgsrc.cols = " << imgsrc.cols << endl;
	Mat tempImgShowHist;
	Mat tempImgArea;
	map<int, int> mapArea;
	/**/
	vector<vector<Point>> contours;
	findContours(imgsrc, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	/**/
	cout<<"contours.size() = "<<contours.size()<<endl;
	for(int i = 0; i < contours.size(); i++){
		area =round( arcLength(contours[i], false) );
		mapArea.insert(pair<int, int>(i, area));
		/**/
		cout<<"ContoursArea("<<i<<") = "<<area<<endl;
		/**/
	}
	cout<<"mapArea.size() = "<<mapArea.size()<<endl;
	/**/
	getMinMax<map<int, int>, int>(mapArea, minAreaIndex, maxAreaIndex);
	cout<<"maxAreaIndex = "<<maxAreaIndex<<endl;
	tempImgArea.create(1, mapArea.size(), CV_8U);
	unsigned char* pData = tempImgArea.data;
	for(int i = 0; i<mapArea.size(); i++){
		pData[i] = mapArea[i];
	}
	/**
	cout<< "tempImgArea: \n" <<tempImgArea<<endl<<endl;
	/**/
	MatND hist = MatND(2, 1, CV_8U);
	int histSize[] = {ceil(mapArea[maxAreaIndex])};
	Vec4i top3Index;
	/**/
	drawHistogram(tempImgArea, hist, histSize, tempImgShowHist);
	getTop3<MatND, float, int>(hist, top3Index, histSize[0]);
	/**/
	cout << "hist.type() = " << hist.type() << endl;
	cout << "hist.rows = " << hist.rows << endl;
	cout << "hist.cols = " << hist.cols << endl;
	cout << "hist.step[0] = " << hist.step[0] << endl;
	cout << "hist.step[1] = " << hist.step[1] << endl << endl;
	/**
	cout<<"hist:\n"<<hist<<endl<<endl;
	/**/
	int histScale = 1;
	float histRange1[] = {0, 255};
	const float* histRange[] = {histRange1};
	double histMinValue = 0, histMaxValue = 0;
	minMaxLoc(hist, &histMinValue, &histMaxValue);
	tempImgShowHist = Mat(Scalar(0));
	tempImgShowHist.create(histRange[0][1], histSize[0] * histScale, CV_8U);
	imshow("hist", tempImgShowHist);
	srand(time(0));
	/**/
	for(int i = 0; i < histSize[0]; i++){
		for(int j = 0; j < contours.size(); j++){
			if(i == mapArea[j]){
				cout << "area(" << j << ") = " << i << endl;
				drawContours(tempImg, contours, j, Scalar(255, 255, 255));
				imshow("descriptorArea:", tempImg);
				waitKey(200);
			}
		}
		/**/
		float binValue = hist.at<float>(i);
		cout<<"binValue hist.at<float>("<<i<<") = "<<binValue<<endl;
		int binIntensity = histRange[0][1] * binValue / histMaxValue;
		rectangle(tempImgShowHist, Point(i*histScale, histRange[0][1] - 1),
			Point((i + 1)*histScale - 1, histRange[0][1] - binIntensity), Scalar(rand() % 255, rand() % 255, rand() % 255));
		imshow("hist", tempImgShowHist);
		/**/
		waitKey(1);
	}
	/**/
}
/* 删除其他质心，获得刻度的质心 */
void getDescriptorArea1(Mat& imgsrc, vector<vector<Point>>& contours, map<int, int>& mapArea){
	/* 删除其他质心，获得刻度的质心 */
	int area = 0;
	int maxAreaIndex = 0;
	int minAreaIndex = 0;
	Mat tempImg = Mat::zeros(imgsrc.size(), imgsrc.type());
	findContours(imgsrc, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	cout << "\ngetDescriptorArea1 contours.size() = " << contours.size() << endl;
	for(int i = 0; i < contours.size(); i++){
		area = round(arcLength(contours[i], false));
		mapArea.insert(pair<int, int>(i, area));
		/**
		cout << "ContoursArea(" << i << ") = " << area << endl;
		/**/
	}
	cout << "getDescriptorArea1 mapArea.size() = " << mapArea.size() << endl;
	/**/
	for(map<int, int>::iterator it = mapArea.begin(); it != mapArea.end(); it++){
		drawContours(tempImg, contours, (*it).first, Scalar(255, 255, 255));
		imshow("getDescriptorArea1", tempImg);
		waitKey(5);
	}
	/**/
	getMinMax<map<int, int>, int>(mapArea, minAreaIndex, maxAreaIndex);
	cout << "getDescriptorArea1 maxAreaIndex = " << maxAreaIndex << endl;
	/**
	for(int i = 0; i < histSize[0]; i++){
		for(int j = 0; j < contours.size(); j++){
			if(i == mapArea[j]){
				cout << "area(" << j << ") = " << i << endl;
				drawContours(tempImg, contours, j, Scalar(255, 255, 255));
				imshow("descriptorArea:", tempImg);
				waitKey(5);
			}
		}
		waitKey(1);
	}
	/**/
	for(map<int, int>::iterator it = mapArea.begin(); it != mapArea.end(); ){
		if((*it).second > 250){
			cout<<"---erase mapArea["<<(*it).first<<"]"<< mapArea[(*it).first]<<endl;
			mapArea.erase(it++);
		}
		else{
			/**
			cout << "mapArea[" << (*it).first << "]" << mapArea[(*it).first] << endl;
			/* 注意不要使用过期的 iterator ,比如将 it++ 放在for循环的小括号内 */
			it++;
		}
	}
	cout<<"getDescriptorArea1 erase, and mapArea.size() = "<<mapArea.size()<<endl;
	/**/
	tempImg = Scalar::all(0);
	for(map<int, int>::iterator it=mapArea.begin(); it!=mapArea.end(); it++){
		drawContours(tempImg, contours, (*it).first, Scalar(255, 255, 255));
		imshow("getDescriptorArea1", tempImg);
		/**/
		waitKey(5);
		/**/
	}
	/**/
	getCentroid(contours, mapArea, g_mCentroid);
	/**
	Mat imgCentroid = Mat::zeros(imgsrc.size(), imgsrc.type());
	Mat imgCentroidErase = Mat::zeros(imgsrc.size(), imgsrc.type());
	/**/
	tempImg = Scalar::all(0);
	/**/
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end(); it++){
		circle(tempImg, (*it).second, 0, Scalar::all(255));
		imshow("getDescriptorArea1", tempImg);
		waitKey(1);
	}
	/* erase Ri < g_v3fDescriptorRoundnessRate[2]*0.8 */
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end();){
		double powRi2 = pow((*it).second.x - g_v3fDescriptorRoundnessRate[0], 2)
			+ pow((*it).second.y - g_v3fDescriptorRoundnessRate[1], 2);
		double Ri = sqrt(powRi2);
		if(Ri < g_v3fDescriptorRoundnessRate[2]*0.8){
			g_mCentroid.erase(it++);
		}
		else{
			it++;
		}
	}
	cout<<"getDescriptorArea1 erase Ri < g_v3fDescriptorRoundnessRate[2]*0.8 \nand g_mCentroid.size() = "<<g_mCentroid.size()<<endl;
	/**
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end(); it++){
		cout<<"g_mCentroid["<<(*it).first<<"] = "<<(*it).second<<endl;
	}
	/**/
	tempImg = Scalar::all(0);
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end(); it++){
		circle(tempImg, (*it).second, 1, Scalar::all(255));
		imshow("getDescriptorArea1", tempImg);
		waitKey(5);
	}
	/* 删除靠近圆心的质心后，对剩下的刻度质心点集进行最小二乘拟合圆 */
	cout<<"getDescriptorArea1 g_v3fLeastSquaresFitting = "<< g_v3fLeastSquaresFitting<<endl;
	leastSquaresFittingCircle(g_mCentroid, g_v3fLeastSquaresFitting);
	cout << "getDescriptorArea1 g_v3fLeastSquaresFitting = " << g_v3fLeastSquaresFitting << endl;
	/**/
	circle(tempImg, Point(g_v3fLeastSquaresFitting[0], g_v3fLeastSquaresFitting[1]),
		g_v3fLeastSquaresFitting[2], Scalar::all(150));
	circle(tempImg, Point(g_v3fLeastSquaresFitting[0], g_v3fLeastSquaresFitting[1]), 2, Scalar::all(255));
	imshow("getDescriptorArea1", tempImg);
	/* 画出最初的圆形目标，对比效果图 */
	waitKey(200);
	circle(tempImg, Point(g_v3fDescriptorRoundnessRate[0], g_v3fDescriptorRoundnessRate[1]),
		g_v3fDescriptorRoundnessRate[2], Scalar::all(100));
	circle(tempImg, Point(g_v3fDescriptorRoundnessRate[0], g_v3fDescriptorRoundnessRate[1]), 2, Scalar::all(100));
	imshow("getDescriptorArea1", tempImg);
	/* 删除大刻度的质心 */
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end();){
		double powRi2 = pow((*it).second.x - g_v3fLeastSquaresFitting[0], 2) 
			+ pow((*it).second.y - g_v3fLeastSquaresFitting[1], 2);
		double powR2 = pow(g_v3fLeastSquaresFitting[2], 2);
		if(powRi2+10 < powR2){
			g_mCentroid.erase(it++);
		}
		else{
			it++;
		}
	}
	waitKey(200);
	tempImg = Scalar::all(0);
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end(); it++){
		circle(tempImg, (*it).second, 0, Scalar::all(255));
		waitKey(5);
		imshow("getDescriptorArea1", tempImg);
	}
	/**
	leastSquaresFitting(g_mCentroid, g_v3fLeastSquaresFitting);
	circle(imgCentroidErase, Point2f(g_v3fLeastSquaresFitting[0], g_v3fLeastSquaresFitting[1]),
		g_v3fLeastSquaresFitting[2], Scalar::all(255));
	circle(imgCentroidErase, Point2f(g_v3fLeastSquaresFitting[0], g_v3fLeastSquaresFitting[1]), 4, Scalar::all(255));
	/**/
	
}
/**/
void getCentroid(vector<vector<Point>>& contours, map<int, int>& mapArea, map<int, Point>& centroid){
	Mat tempImgShow = Mat::zeros(g_imgSrc.size(), g_imgSrc.type());
	/**
	for(map<int, int>::iterator it=mapArea.begin(); it!=mapArea.end(); it++){
		drawContours(tempImgShow, contours, (*it).first, Scalar::all(255));
		imshow("getCentroid show contours", tempImgShow);
		waitKey(20);
	}
	/**/
	Moments moment;
	map<int, Moments> mapMoment;
	map<int, Point> mapCentroid;
	for(map<int, int>::iterator it = mapArea.begin(); it != mapArea.end(); it++){
		moment = moments(contours[(*it).first]);
		mapMoment.insert(pair<int, Moments>((*it).first, moment));
		mapCentroid.insert(pair<int, Point>((*it).first,
			Point(mapMoment[(*it).first].m10 / mapMoment[(*it).first].m00, 
				mapMoment[(*it).first].m01 / mapMoment[(*it).first].m00)));
	}
	centroid = mapCentroid;
	cout << "getCentroid: centroid.size() = " << centroid.size() << endl << endl;
}
/**
void test(){
	Mat tempImg = Mat(100, 100, CV_8U);
	Mat tempShowContours = tempImg.clone();
	Mat imgCanny = tempImg.clone();
	unsigned char* pData = tempImg.data;
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 100; j++){
			if(20 <= i && i < 80 && 20 <= j && j < 80){
				*(pData + tempImg.step[0] * i + tempImg.step[1] * j) = 255;
			}
			else{
				*(pData + tempImg.step[0] * i + tempImg.step[1] * j) = 0;
			}
		}
	}
	Canny(tempImg, imgCanny, 100, 200, 3);
	vector<vector<Point>> contours;
	findContours(imgCanny, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	for(int i = 0; i < contours.size(); i++){
		drawContours(tempShowContours, contours, i, Scalar(255, 255, 255));
		waitKey(500);
	}
	int area = contourArea(contours[0], false);
	int length = arcLength(contours[0], false);
	cout<<"area = "<<area<<endl;
	cout << "arcLength = " << length << endl;
	imshow("rect", tempImg);
	imshow("canny", imgCanny);

	MatND hist;
	int histSize[] = {256};
	drawHistogram(tempImg, hist, histSize, Mat());
	/**
	cout<<"hist:\n"<<hist<<endl;
	/**
	cout<<"hist.type() = "<<hist.type()<<endl;
	cout << "hist.rows = " << hist.rows << endl;
	cout << "hist.cols = " << hist.cols << endl;
	cout << "hist.step[0] = " << hist.step[0] << endl;
	cout << "hist.step[1] = " << hist.step[1] << endl;
	/**
	for(int i = 0; i < histSize[0]; i++){
		cout<<"hist.at<float>("<<i<<") = "<<hist.at<float>(i)<<endl;
		cout <<(float)(*(hist.data + hist.step[0]*i + hist.step[1]*0))<<" "
			/**
			<< (char)(*(hist.data + hist.step[0] * i + hist.step[1] * 1)) << " "
			<< (char)(*(hist.data + hist.step[0] * i + hist.step[1] * 2)) << " "
			<< (float)(*(hist.data + hist.step[0] * i + hist.step[1] * 3)) << " "
			/**
			<<endl;	
	}
	/**
	float* p = (float*)hist.data;
	for(int i = 0, j=0; i < histSize[0]; i++, j++){
		cout << "hist.at<float>(" << i << ") = " << hist.at<float>(i) << endl;
		cout << (*(p+i))<<" "
			/**
			<< endl;
	}
	waitKey();
}
/**/
void testGetContours(){
	Mat tempImg = g_imgRound.clone();
	Mat tempImgShow = Mat::zeros(g_imgRound.size(), g_imgRound.type());
	Mat element = getStructuringElement(MORPH_RECT, Size(3,3));
	/**
	erode(g_imgRound, tempImg, element);
	/**
	tempImg = g_imgRound - tempImg;
	/**/
	vector<vector<Point>> contours;
	findContours(tempImg, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	cout<<"testGetContours.size() = "<<contours.size()<<endl;
	for(int i = 0; i < contours.size(); i++){
		drawContours(tempImgShow, contours, i, Scalar(255, 255, 255));
		imshow("drawMorphErode", tempImgShow);
		waitKey(200);
	}
}
/* least squares fitting */
void leastSquaresFitting(map<int, Point>& mapcentroid, Vec3f& leastsquaresfitting){
	/**
	Mat tempImg = Mat::zeros(g_imgSrc.size(), g_imgSrc.type());
	for(map<int, Point>::iterator it = mapcentroid.begin(); it != mapcentroid.end(); it++){
		circle(tempImg, (*it).second, 0, Scalar::all(255));
		imshow("leastSquaresFitting showCentroid", tempImg);
		waitKey(20);
	}
	/**/
	if(mapcentroid.size()<3){
		return;
	}
	cout<<"leastSquaresFitting mapcentroid.size = "<<mapcentroid.size()<<endl;
	double X1 = 0;
	double Y1 = 0;
	double X2 = 0;
	double Y2 = 0;
	double X3 = 0;
	double Y3 = 0;
	double X1Y1 = 0;
	double X1Y2 = 0;
	double X2Y1 = 0;
	int n =0;
	for(map<int, Point>::iterator it= mapcentroid.begin(); it!= mapcentroid.end(); it++){
		X1 = X1 + (*it).second.x;
		Y1 = Y1 + (*it).second.y;
		X2 = X2 + ((*it).second.x) * ((*it).second.x);
		Y2 = Y2 + ((*it).second.y) * ((*it).second.y);
		X3 = X3 + ((*it).second.x) * ((*it).second.x) * ((*it).second.x);
		Y3 = Y3 + ((*it).second.y) * ((*it).second.y) * ((*it).second.y);
		X1Y1 = X1Y1 + ((*it).second.x) * ((*it).second.y);
		X1Y2 = X1Y2 + ((*it).second.x) * ((*it).second.y) * ((*it).second.y);
		X2Y1 = X2Y1 + ((*it).second.x) * ((*it).second.x) * ((*it).second.y);
		n++;
		cout << "leastSquaresFitting  n = " << n << endl;
	}

	double C, D, E, G, H, N;
	double a, b, c;
	N = mapcentroid.size();
	C = N*X2 - X1*X1;
	D = N*X1Y1 - X1*Y1;
	E = N*X3 + N*X1Y2 - (X2 + Y2)*X1;
	G = N*Y2 - Y1*Y1;
	H = N*X2Y1 + N*Y3 - (X2 + Y2)*Y1;
	a = (H*D - E*G) / (C*G - D*D);
	b = (H*C - E*D) / (D*D - G*C);
	c = -(a*X1 + b*Y1 + X2 + Y2) / N;

	double A, B, R;
	A = a / (-2);
	B = b / (-2);
	R = sqrt(a*a + b*b - 4 * c) / 2;
	leastsquaresfitting[0] = A;
	leastsquaresfitting[1] = B;
	leastsquaresfitting[2] = R;
	/**/
	cout<<"A = "<<A<<" B = "<<B<<" C = "<<C<<endl;
	/**/
}
/* !!! 这个需要修改 */
void leastSquaresFittingContour(vector<Point>& contour, Vec3f& v3fleastsquaresfittingcontour){
	/**
	map<int, Point> mapLightWeightContour;
	cout<<"leastSquaresFittingContour contour.size() = "<<contour.size()<<endl;
	for(int i = 0; i < contour.size(); i+=5){
		mapLightWeightContour.insert(pair<int, Point>(i, contour[i]));
	}
	cout<<"leastSquaresFittingContour mapLightWeightContour.size() = "<< mapLightWeightContour.size()<<endl;
	/**
	leastSquaresFitting(mapLightWeightContour, v3fleastsquaresfittingcontour);
	/**/
	Moments momentsContour;
	momentsContour = moments(contour, false);
	v3fleastsquaresfittingcontour[0] = momentsContour.m10/momentsContour.m00;
	v3fleastsquaresfittingcontour[1] = momentsContour.m01 / momentsContour.m00;
	double powRi2 = pow( contour[0].x - v3fleastsquaresfittingcontour[0], 2) 
		+ pow(contour[0].y - v3fleastsquaresfittingcontour[1], 2);
	v3fleastsquaresfittingcontour[2] = sqrt(powRi2);
	/**
	cout<<"leastSquaresFittingContour get X,Y,R = "
		<< v3fleastsquaresfittingcontour[0]<<" "
		<< v3fleastsquaresfittingcontour[1]<<" "
		<< v3fleastsquaresfittingcontour[2]<<" "
		<<endl;
	/**/
}
/**/
void leastSquaresFittingCircle(map<int, Point>& mapcentroid, Vec3f& v3fLeastsquaresfittingcircle){
	double center_x, center_y,  radius;
	center_x = 0.0f;
	center_y = 0.0f;
	radius = 0.0f;
	if(mapcentroid.size() < 3){
		cout<<"mapcentroid.size() = "<< mapcentroid.size()<<endl;
		return;
	}

	double sum_x = 0.0f, sum_y = 0.0f;
	double sum_x2 = 0.0f, sum_y2 = 0.0f;
	double sum_x3 = 0.0f, sum_y3 = 0.0f;
	double sum_xy = 0.0f, sum_x1y2 = 0.0f, sum_x2y1 = 0.0f;

	int N = mapcentroid.size();
	double x = 0.0;
	double y = 0.0;
	double x2 = 0.0;
	double y2 = 0.0;
	for(map<int, Point>::iterator it=mapcentroid.begin(); it!=mapcentroid.end(); it++){
		x = (*it).second.x;
		y = (*it).second.y;
		x2 = x * x;
		y2 = y * y;
		sum_x += x;
		sum_y += y;
		sum_x2 += x2;
		sum_y2 += y2;
		sum_x3 += x2 * x;
		sum_y3 += y2 * y;
		sum_xy += x * y;
		sum_x1y2 += x * y2;
		sum_x2y1 += x2 * y;
	}

	double C, D, E, G, H;
	double a, b, c;

	C = N * sum_x2 - sum_x * sum_x;
	D = N * sum_xy - sum_x * sum_y;
	E = N * sum_x3 + N * sum_x1y2 - (sum_x2 + sum_y2) * sum_x;
	G = N * sum_y2 - sum_y * sum_y;
	H = N * sum_x2y1 + N * sum_y3 - (sum_x2 + sum_y2) * sum_y;
	a = (H * D - E * G) / (C * G - D * D);
	b = (H * C - E * D) / (D * D - G * C);
	c = -(a * sum_x + b * sum_y + sum_x2 + sum_y2) / N;

	center_x = a / (-2);
	center_y = b / (-2);
	radius = sqrt(a * a + b * b - 4 * c) / 2;
	v3fLeastsquaresfittingcircle[0] = center_x;
	v3fLeastsquaresfittingcircle[1] = center_y;
	v3fLeastsquaresfittingcircle[2] = radius;
	cout<<"center_x = "<<center_x
		<< "\ncenter_y = " << center_y 
		<< "\nradius = " << radius 
		<<endl;
}
/* 获得指针轮廓 */
void getDescriptorPointer(Mat& imgsrc, Mat& imgdst){
	/* 获得指针轮廓 */
	vector<vector<Point>> contours;
	map<int, double> mapRoundnessRate;
	int minRoundnessRateIndex = 0;
	int maxRoundnessRateIndex = 0;
	double roundnessRate = 0;
	Mat tempImg = Mat::zeros(imgsrc.size(), imgsrc.type());
	Scalar scalar = Scalar::all(255);
	int arcLen = 0;
	double area = 0;
	findContours(imgsrc, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	for(int i = 0; i < contours.size(); i++){
		drawContours(tempImg, contours, i, scalar);
		imshow("getDescriptorPointer", tempImg);
		waitKey(50);

		arcLen = arcLength(contours[i], true);
		area = contourArea(contours[i], false);
		if(arcLen < 200 || area < 300){
			continue;
		}
		roundnessRate = 4*CV_PI*area/pow(arcLen, 2);
		mapRoundnessRate.insert(pair<int, double>(i, roundnessRate));
	}
	cout<<"getDescriptorPointer mapRoundnessRate.size() = "<<mapRoundnessRate.size()<<endl;
	getMinMax<map<int, double>, double>(mapRoundnessRate, minRoundnessRateIndex, maxRoundnessRateIndex);
	cout<<"minRoundnessRateIndex = "<< minRoundnessRateIndex<<"\nmaxRoundnessRateIndex = "<< maxRoundnessRateIndex<<endl;
	tempImg = Scalar::all(0);
	drawContours(tempImg, contours, minRoundnessRateIndex, scalar);
	imshow("getDescriptorPointer", tempImg);
	waitKey();
	drawContours(tempImg, contours, maxRoundnessRateIndex, scalar);
	imshow("getDescriptorPointer", tempImg);
	imgdst = tempImg;
}
/* 通过两点获得角度值 */
void getAngleFromTwoPoints(float& angle, Point& pointStart, Point& pointEnd){
	/* 通过两点获得角度值 */
	float xx = 0, yy = 0;
	xx = pointEnd.x - pointStart.x;
	yy = pointEnd.y - pointStart.y;
	if(xx == 0){
		angle = CV_PI / 2;
	}
	else{
		angle = atan(abs(yy / xx));
	}
	if(xx<0 && yy>0){
		angle = CV_PI - angle;
	}
	else if(xx < 0 && yy < 0){
		angle = CV_PI + angle;
	}
	else if(xx > 0 && yy < 0){
		angle = 2 * CV_PI - angle;
	}
}
/* 获得环形 mask */
void getAnnularMask(Mat& mask){
	/* 获得环形mask */
	Mat imgCircleLarge = Mat::zeros(g_imgSrc.size(), g_imgSrc.type());
	Mat imgCircleSmall = Mat::zeros(g_imgSrc.size(), g_imgSrc.type());
	Point center = Point(g_v3fDescriptorRoundnessRate[0], g_v3fDescriptorRoundnessRate[1]);
	int radiusLarge = g_v3fDescriptorRoundnessRate[2];
	Scalar scalar = Scalar::all(255);
	circle(imgCircleLarge, center, radiusLarge, scalar);
	circle(imgCircleSmall, center, radiusLarge*0.4, scalar);
	floodFill(imgCircleLarge, center, scalar);
	floodFill(imgCircleSmall, center, scalar);
	mask = imgCircleLarge - imgCircleSmall;
	/**/
	imshow("getAnnularMask", mask);
	/**/
}
/* 获得环形 target */
void getAnnularTarget(Mat& imgsrc, Mat& imgdst, Mat& mask){
	/* 获得环形 target */
	imgsrc.copyTo(imgdst, mask);
	/**/
	imshow("getAnnularTarget", imgdst);
	/**/
}
/* 将 imgsrc 向 center 投影 */
void project(Mat& imgsrc, map<int, int>& mapProj, Point& center, int pcount){
	/* 将 imgsrc 向 center 投影 */
	int countPerDegree = pcount/360;
	float angle = 0;
	int angleIndex = 0;
	for(int i = 0; i < pcount; i++){
		mapProj[i] = 0;
		cout<<"mapProj["<<i<<"]="<<mapProj[i]<<" ";
		if(i % 10 == 9){
			cout<<endl;
		}
	}
	for(int i = 0; i < imgsrc.rows; i++){
		for(int j = 0; j < imgsrc.cols; j++){
			if(imgsrc.at<uchar>(i, j)){
				/* point(col, row) */
				/**
				circle(tempImg, Point(j, i), 6, Scalar::all(222));
				/**/
				getAngleFromTwoPoints(angle, center, Point(j, i));
				/* 注意将弧度转化为角度 */
				angle = angle * 180/CV_PI;
				/* 将angle转换为对应的index， 比如angle=1.32， countPerDegree=5, 
				那么直观上index=6，实际计算上每0.2单位angle使得index加1， */
				angleIndex = floor(angle*countPerDegree);
				mapProj[angleIndex]++;
				/**
				cout<<"mapProj["<<angleIndex<<"]="<<mapProj[angleIndex]<<endl;;
				/**
				imshow("testDrawHistogramFromUserData", tempImg);
				/**/
			}
		}
	}
	cout<<endl;
	for(int i = 0; i < pcount; i++){
		cout << "mapProj[" << i << "]=" << mapProj[i] << " ";
		if(i % 10 == 9){
			cout << endl;
		}
	}
}
/**/
void drawHistogramFromUserData(MatND& imgsrc, Mat& imgshow){
	imgshow = Scalar::all(0);
	int histScale = 1;
	int histSize = imgsrc.rows;
	float histRange = 200;
	// create(rows, cols, type)
	imgshow.create(histRange, histSize * histScale, CV_8U);
	double histMinValue = 0, histMaxValue = 0;
	minMaxLoc(imgsrc, &histMinValue, &histMaxValue);
	for(int i = 0; i < histSize; i++){
		/* 这样好像不行 *
		int binValue = *(hist.data + hist.step[0]*i + hist.step[1]*0);
		/* 因为 MatND 的 type 是 CV_32F，所以用 at<float> */
		int binValue = imgsrc.at<float>(i);
		int binIntensity = histRange * binValue / histMaxValue;
		rectangle(imgshow, Point(i*histScale, histRange - 1),
			Point((i + 1)*histScale - 1, histRange - binIntensity), Scalar::all(255));
	}
	imshow("drawHistogramFromUserData", imgshow);
}
/**/
void getResult(){
	Mat imgShow;
	map<int, int> mapProj;
	int nCountPerDegree = 2;
	int nProjCount = 360* nCountPerDegree;
	MatND userData = Mat(nProjCount, 1, CV_32F);
	project(g_imgAnnular, mapProj, Point(g_v3fDescriptorRoundnessRate[0], g_v3fDescriptorRoundnessRate[1]), nProjCount);
	for(int i = 0; i < nProjCount; i++){
		userData.at<float>(i) = mapProj[i];
	}
	drawHistogramFromUserData(userData, imgShow);

	int nStep = 90/2;
	int everage = 0;
	int sum = 0;
	/* 用于计算everage */
	int count = 0;
	int max=0, maxIndex = 0;
	Vec2i projPointer;
	map<int, int> mapMax;
	map<int, int> mapEverage;
	map<int, int>::iterator itFront, itBack;
	int i=0, kFront=0, kBack=0;
	projPointer[0] = 0;
	projPointer[1] = mapProj[0];
	for(map<int, int>::iterator it = mapProj.begin(); it != mapProj.end(); it++){
		if((*it).second > projPointer[1]){
			projPointer[1] = (*it).second;
			projPointer[0] = (*it).first;
		}
	}
	cout<<"getResult projPointer: "<<projPointer<<endl;
	for(i = 0; i < mapProj.size(); i += nStep*nCountPerDegree){
		max = mapProj[i];
		maxIndex = i;
		sum = 0;
		count = 0;
		everage = 0;
		for(itFront = mapProj.find(i);
			itFront != mapProj.find(i +1+ nStep*nCountPerDegree / 2) && itFront!=mapProj.end();
			itFront++){
			count++;
			if( abs((*itFront).first - projPointer[0])<4 ){
				sum += (*itFront).second/3;
				continue;
			}
			else if((*itFront).second > max){
				max = (*itFront).second;
				maxIndex = (*itFront).first;
			}
			sum += (*itFront).second;
		}
		for(itBack = mapProj.find(i); 
			itBack != mapProj.find(i - nStep*nCountPerDegree / 2) && itBack != mapProj.begin();
			itBack--){
			count++;
			if( abs((*itBack).first - projPointer[0])<4 ){
				sum += (*itBack).second/3;
				continue;
			}
			if((*itBack).second > max){
				max = (*itBack).second;
				maxIndex = (*itBack).first;
			}
			sum += (*itBack).second;
		}
		mapMax.insert(pair<int ,int>(maxIndex, max));
		sum -= mapProj[i];
		count--;
		mapEverage.insert(pair<int, int>(maxIndex, sum/count));
		cout<<"i = "<<i<<"  count = "<<count<<"  everage = "<<everage<<endl;
		/**
		for(kFront = 0; kFront < nStep / 2; kFront++){
			if(i + kFront > mapProj.size()){
				kFront--;
				break;
			}
			sum += mapProj[i+ kFront];
			if(mapProj[i + kFront] > max){
				max = mapProj[i+ kFront];
				maxIndex = i+ kFront;
			}
		}
		for(kBack = 0; kBack < nStep / 2; kBack++){
			if(i - kBack < 0){
				kBack++;
				break;
			}
			sum += mapProj[i-kBack];
			if(mapProj[i - kBack] > max){
				max = mapProj[i-kBack];
				maxIndex = i-kBack;
			}
		}
		/**/
	}
	for(map<int, int>::iterator it = mapMax.begin(); it != mapMax.end(); it++){
		cout << "getResult mapMax[" << (*it).first << "] = " << (*it).second << endl;
	}
	cout << endl;
	for(map<int, int>::iterator it = mapEverage.begin(); it != mapEverage.end(); it++){
		cout << "getResult mapEverage[" << (*it).first << "] = " << (*it).second << endl;
	}
	/* 直接求出均值最小处，其两边是起始刻度和终止刻度， 有问题 *
	map<int, int>::iterator it, itMinMapEverageVaule, itMinMapMaxVaule;
	for(it = mapEverage.begin(), itMinMapEverageVaule =it; it != mapEverage.end(); it++){
		if((*it).second < (*itMinMapEverageVaule).second){
			itMinMapEverageVaule = it;
		}
	}
	itMinMapMaxVaule = mapMax.find((*itMinMapEverageVaule).first);
	itMinMapMaxVaule++;
	int startAngle = (*itMinMapMaxVaule).first;
	itMinMapMaxVaule--;
	itMinMapMaxVaule--;
	int endAngle = (*itMinMapMaxVaule).first;
	cout<<"startAngle = "<<startAngle<<"  endAngle = "<<endAngle<<endl;

	/* 利用二阶导数求起始刻度和终止刻度，但不能区分起始和终止 *
	map<int, int> mapTheSecondDerivative;
	int f0 = 0, f1 = 0, f2 = 0;
	int theSecondDerivative = 0;
	map<int, int>::iterator tempIt;
	for(map<int, int>::iterator it = mapEverage.begin(); it != mapEverage.end(); it++){
		if(it == mapEverage.begin()){
			tempIt = mapEverage.end();
			f0 = (*tempIt).second;
		}
		else{
			tempIt = it;
			tempIt--;
			f0 = (*tempIt).second;
		}
		f1 = (*it).second;
		if(it == mapEverage.end()){
			tempIt = mapEverage.begin();
			f2 = (*tempIt).second;
		}
		else{
			tempIt = it;
			tempIt++;
			f2 = (*tempIt).second;
		}
		theSecondDerivative = f0 + f2 -f1;
		mapTheSecondDerivative.insert(pair<int, int>((*it).first, theSecondDerivative));
	}
	map<int, int>::iterator it1, it2;
	for(map<int, int>::iterator it = mapTheSecondDerivative.begin(), it1 = it, it2 = it;
		it != mapTheSecondDerivative.end(); 
		it++){
		
	}
	/* 利用一阶导数 */
	map<int, int> mapDerivative;
	int f0 = 0, f1 = 0;
	int derivative = 0;
	map<int, int>::iterator tempIt;
	for(map<int, int>::iterator it = mapEverage.begin(); it != mapEverage.end(); it++){
		f0 = (*it).second;
		tempIt = it;
		tempIt++;
		if(tempIt == mapEverage.end()){
			tempIt = mapEverage.begin();
			f1 = (*tempIt).second;
		}
		else{
			f1 = (*tempIt).second;
		}
		derivative = f1 - f0;
		mapDerivative.insert(pair<int, int>((*it).first, derivative));
	}
	/**/
	map<int, int>::iterator it1, it2, itDerivative;
	/**/
	for(itDerivative = mapDerivative.begin(), it1 = itDerivative, it2 = itDerivative;
		itDerivative != mapDerivative.end();
		itDerivative++){
		/* it1 指向最大一阶导数，表示开始刻度的前一个整刻度 */
		if((*itDerivative).second > (*it1).second){
			it1 = itDerivative;
		 }
		/* it2 指向最小一阶导数，表示结束刻度 */
		if((*itDerivative).second < (*it2).second){
			it2 = itDerivative;
		}
	}
	/**/
	it1++;
	/**/
	int angleIndexStart = (*it1).first;
	int angleIndexEnd = (*it2).first;
	/**/
	int range = 60;
	float angleStart = 1.0 * angleIndexStart/nCountPerDegree;
	float angleEnd = 1.0 * angleIndexEnd / nCountPerDegree;
	float anglePointer = 1.0 * projPointer[0] / nCountPerDegree;
	cout << "angleIndexStart = " << angleIndexStart << "  angleIndexEnd = " << angleIndexEnd << endl;
	cout << "angleStart = " << angleStart << "  angleEnd = " << angleEnd << endl;
	float pointerRange = anglePointer - angleStart;
	if(pointerRange < 0){
		pointerRange += 360;
	}
	float fullRange = angleEnd - angleStart;
	if(fullRange < 0){
		fullRange += 360;
	}
	float result = range * pointerRange/ fullRange;
	cout<<"result = "<<result<<endl;
}
/**/
void getTemplate(){

}
void getDescriptorAndMatch(Mat& imgTemplate, Mat& imgSample){
	Mat imgShow;
	Ptr<ORB> orb = ORB::create();
	vector<KeyPoint> vKeyPoint1, vKeyPoint2, vKeyPoint3;
	Mat descriptors1, descriptors2, descriptors3;
	FlannBasedMatcher flannBasedMatcher;
	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
	vector<DMatch> vMatches, vGoodMatches;;
	vector<vector<DMatch>> vvMatches;
	/**/
	orb->detectAndCompute(imgTemplate, Mat(), vKeyPoint1, descriptors1);
	orb->detectAndCompute(imgSample, Mat(), vKeyPoint2, descriptors2);
	drawKeypoints(imgTemplate, vKeyPoint1, imgShow);
	imshow("getDescriptorAndMatch", imgShow);
	drawKeypoints(imgSample, vKeyPoint2, imgShow);
	imshow("getDescriptorAndMatch", imgShow);
	/* knnmatch *
	flannBasedMatcher.knnMatch(descriptors1, descriptors2, vvMatches, 2);
	for(int i = 0; i < vvMatches.size(); i++){
		if(vvMatches[i][0].distance < vvMatches[i][1].distance*0.8){
			vGoodMatches.push_back(vvMatches[i][0]);
		}
	}
	/* drawMatches */
	double minDistance = 100.0, maxDistance = 0, distance = 0;
	descriptorMatcher->match(descriptors1, descriptors2, vMatches);
	for(int i = 0; i < descriptors1.rows; i++){
		distance = vMatches[i].distance;
		if(distance < minDistance){
			minDistance = distance;
		}
		if(distance > maxDistance){
			maxDistance = distance;
		}
	}
	cout<<"minDistance = "<<minDistance<<"  maxDistance = "<<maxDistance<<endl;
	for(int i = 0; i < vMatches.size(); i++){
		if(vMatches[i].distance < g_nGoodMatchDistance*0.01 * minDistance){
			vGoodMatches.push_back(vMatches[i]);
		}
	}
	imgShow = Scalar::all(0);
	drawMatches(imgTemplate, vKeyPoint1, imgSample, vKeyPoint2, vGoodMatches, imgShow);
	imshow("getDescriptorAndMatch", imgShow);
	cout<<"getDescriptorAndMatch match"<<endl;

	/* perspecticeTransform */
	vector<Point2f> vpMatchSample, vpMatchTemplate;
	vector<Point2f> vpCornerSample(4), vpCornerTemplate(4);
	Mat imgHSampleToTemplate, imgHTemplateToSample, imgH;
	Point2f pointOffset = Point2f( (float)imgTemplate.cols, 0 );
	for(int i = 0; i < vGoodMatches.size(); i++){
		vpMatchTemplate.push_back(vKeyPoint1[vGoodMatches[i].queryIdx].pt);
		vpMatchSample.push_back(vKeyPoint2[vGoodMatches[i].trainIdx].pt);
	}
	if(vpMatchSample.size() < 1){
		cout<<"!!! error vpMatchSample.size() < 1"<<endl;
		return;
	}
	/* 寻找矩阵 imgHomography，将 vpMatchTemplate(src) 中的点变换到 vpMatchSample(dst) 中 */
	imgHSampleToTemplate = findHomography(vpMatchSample, vpMatchTemplate, RANSAC);
	/**/
	imgHTemplateToSample = findHomography(vpMatchTemplate, vpMatchSample, RANSAC);
	/**/
	if(!imgHTemplateToSample.data){
		cout<<"!!! error imgHomography empty\npress anykey"<<endl;
		return;
	}
	/**
	vpCornerSample[0] = Point2f(0, 0);
	vpCornerSample[1] = Point2f(imgSample.cols-1, 0);
	vpCornerSample[2] = Point2f(imgSample.cols-1, imgSample.rows-1);
	vpCornerSample[3] = Point2f(0, imgSample.rows-1);
	/**/
	vpCornerTemplate[0] = Point2f(0, 0);
	vpCornerTemplate[1] = Point2f(imgTemplate.cols-1, 0);
	vpCornerTemplate[2] = Point2f(imgTemplate.cols-1, imgTemplate.rows-1);
	vpCornerTemplate[3] = Point2f(0, imgTemplate.rows-1);
	/**
	perspectiveTransform(vpCornerSample, vpCornerTemplate, imgHomography);
	/* 利用矩阵 imgHomography 将 vpCornerTemplate(src) 中的点变换到 vpCornerSample(dst) 中 */
	perspectiveTransform(vpCornerTemplate, vpCornerSample, imgHTemplateToSample);
	/**
	line(imgShow, vpCornerTemplate[0]+pointOffset, vpCornerTemplate[1]+ pointOffset, Scalar::all(255), 1);
	line(imgShow, vpCornerTemplate[1] + pointOffset, vpCornerTemplate[2] + pointOffset, Scalar::all(255), 1);
	line(imgShow, vpCornerTemplate[2] + pointOffset, vpCornerTemplate[3] + pointOffset, Scalar::all(255), 1);
	line(imgShow, vpCornerTemplate[3] + pointOffset, vpCornerTemplate[0] + pointOffset, Scalar::all(255), 1);
	/**/
	line(imgShow, vpCornerSample[0] + pointOffset, vpCornerSample[1] + pointOffset, Scalar::all(255), 1);
	line(imgShow, vpCornerSample[1] + pointOffset, vpCornerSample[2] + pointOffset, Scalar::all(255), 1);
	line(imgShow, vpCornerSample[2] + pointOffset, vpCornerSample[3] + pointOffset, Scalar::all(255), 1);
	line(imgShow, vpCornerSample[3] + pointOffset, vpCornerSample[0] + pointOffset, Scalar::all(255), 1);
	/**/
	imshow("getDescriptorAndMatch", imgShow);
	cout<<"getDescriptorAndMatch perspectiveTransform"<<endl;

	/* sample to template */
	Mat imgSampleToTemplate = Mat::zeros(imgTemplate.size(), CV_8U);
	Mat imgTarget;
	Mat mask = Mat::zeros(imgSample.size(), imgSample.type());
	vector<Point2f> vPointSrc, vPointDst;
	Point2f seed;
	float seedX = 0, seedY = 0;
	/**
	line(mask, vpCornerSample[0] , vpCornerSample[1] , Scalar::all(255), 1);
	line(mask, vpCornerSample[1] , vpCornerSample[2] , Scalar::all(255), 1);
	line(mask, vpCornerSample[2] , vpCornerSample[3] , Scalar::all(255), 1);
	line(mask, vpCornerSample[3] , vpCornerSample[0] , Scalar::all(255), 1);
	seedX = (vpCornerSample[0].x + vpCornerSample[1].x + vpCornerSample[2].x + vpCornerSample[3].x)/4;
	seedY = (vpCornerSample[0].y + vpCornerSample[1].y + vpCornerSample[2].y + vpCornerSample[3].y) / 4;
	seed = Point2f(seedX, seedY);
	floodFill(mask, seed, Scalar::all(255));
	imgSample.copyTo(imgTarget, mask);
	imshow("getDescriptorAndMatch imgTarget", imgTarget);
	/**/
	imshow("imgSampleToTemplate", imgSampleToTemplate);
	for(int i=0; i<imgSample.rows; i++){
		for(int j = 0; j < imgSample.cols; j++){
			vPointSrc.push_back(Point2f(j, i));
		}
	}
	perspectiveTransform(vPointSrc, vPointDst, imgHSampleToTemplate);
	int count = 0, 
		step0src = imgSample.step[0],
		step1src = imgSample.step[1],
		step0dst = imgSampleToTemplate.step[0],
		step1dst = imgSampleToTemplate.step[1];
	cout<<" step0dst = "<<step0dst<< "  step1dst = "<<step1dst
		<<" step0src = " <<step0src<< " step1src = " <<step1src
		<<endl;
	for(int i = 0; i < imgSample.rows; i++){
		/**/
		uchar* pSrc = imgSample.ptr<uchar>(i);
		/**
		uchar* pSrc = imgSample.data;
		/**/
		for(int j = 0; j < imgSample.cols; j++){
			int x = vPointDst[count].x;
			int y = vPointDst[count].y;
			if(x<0 || x>imgSample.cols || y<0 || y>imgSample.rows){
				count++;
				continue;
			}
			/**/
			uchar* pDst = imgSampleToTemplate.ptr<uchar>(y);
			/**
			uchar* pDst = imgSampleToTemplate.data;
			/**/
			pDst[x*imgSampleToTemplate.step[1] + 0] = pSrc[j*imgSample.step[1] + 0];
			/**/
			pDst[x*imgSampleToTemplate.step[1] + 1] = pSrc[j*imgSample.step[1] + 1];
			pDst[x*imgSampleToTemplate.step[1] + 2] = pSrc[j*imgSample.step[1] + 2];
			/**
			pDst[x] = pSrc[j];
			/**
			*(pDst+step0dst*y + step1dst*x + 0) = *(pSrc+step0src*i + step1src*j +0);
			/**/
			count++;
		}
	}
	if(!imgSampleToTemplate.data){
		imshow("imgSampleToTemplate", imgSampleToTemplate);
	}
	else{
		cout << "!!! warning imgSampleToTemplate.data" << endl;
	}
	
	/**
	warpPerspective(imgTarget, imgSampleToTemplate, imgHSampleToTemplate, Size(imgTemplate.cols, imgTemplate.rows));
	imshow("imgHomography", imgSampleToTemplate);
	/* template 和 矫正后的图像进行特征匹配 *
	Mat imgShowHow = Mat::zeros(imgTemplate.size(), imgTemplate.type());
	vMatches.clear();
	vGoodMatches.clear();
	orb->detectAndCompute(imgSampleToTemplate, Mat(), vKeyPoint3, descriptors3);
	descriptorMatcher->match(descriptors1, descriptors3, vMatches);
	for(int i = 0; i < vMatches.size(); i++){
		if(vMatches[i].distance < g_nGoodMatchDistance*0.01 * minDistance){
			vGoodMatches.push_back(vMatches[i]);
		}
	}
	drawMatches(imgTemplate, vKeyPoint1, imgSampleToTemplate, vKeyPoint3, vGoodMatches, imgShowHow);
	imshow("how", imgShowHow);
	/**/
}
void registration(Mat& imgsrc, Mat& imgdst, Mat& imgTemplate){
	Mat imgShow;
	Ptr<ORB> orb = ORB::create();
	vector<KeyPoint> vKeyPoint1, vKeyPoint2;
	Mat imgDescriptor1, imgDescriptor2;
	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
	vector<DMatch> vDMatch, vDMatchGood;
	vector<vector<DMatch>> vvDMatch;
	/**/
	vector<Point2f> vpGoodTemplate, vpGoodSample;
	Mat imgHSampleToTemplate, imgHTemplateToSample;
	vector<Point2f> vpCornerTemplate(4), vpCornerSample(4);
	Point2f offset(imgTemplate.cols-1, 0);
	Scalar scalar(255);
	/* 特征匹配 */
	orb->detectAndCompute(imgTemplate, Mat(), vKeyPoint1, imgDescriptor1);
	orb->detectAndCompute(imgsrc, Mat(), vKeyPoint2, imgDescriptor2);
	descriptorMatcher->match(imgDescriptor1, imgDescriptor2, vDMatch);
	sort(vDMatch.begin(), vDMatch.end());
	for(int i = 0; i < vDMatch.size()*g_nGoodMatchDistance*0.001; i++){
		vDMatchGood.push_back(vDMatch[i]);
		vpGoodTemplate.push_back( vKeyPoint1[vDMatchGood[i].queryIdx].pt );
		vpGoodSample.push_back( vKeyPoint2[vDMatchGood[i].trainIdx].pt );
	}
	drawMatches(imgTemplate, vKeyPoint1, imgsrc, vKeyPoint2, vDMatchGood, imgShow);
	imshow("registration drawMatches", imgShow);
	cout<<"registration drawMatches"<<endl;
	/* 透视变换 sample to template */
	imgHSampleToTemplate = findHomography(vpGoodSample, vpGoodTemplate, RANSAC);
	warpPerspective(imgsrc, imgdst, imgHSampleToTemplate, Size(imgsrc.cols, imgsrc.rows));
	imshow("registration", imgdst);
	cout << "registration warpPerspective" << endl;
	/* 目标寻找 template to sample */
	imgHTemplateToSample = findHomography(vpGoodTemplate, vpGoodSample, RANSAC);
	vpCornerTemplate[0] = Point2f(0, 0);
	vpCornerTemplate[1] = Point2f(imgTemplate.cols - 1, 0);
	vpCornerTemplate[2] = Point2f(imgTemplate.cols - 1, imgTemplate.rows - 1);
	vpCornerTemplate[3] = Point2f(0, imgTemplate.rows - 1);
	perspectiveTransform(vpCornerTemplate, vpCornerSample, imgHTemplateToSample);
	line(imgShow, vpCornerSample[0]+offset, vpCornerSample[1]+offset, scalar);
	line(imgShow, vpCornerSample[1] + offset, vpCornerSample[2] + offset, scalar);
	line(imgShow, vpCornerSample[2] + offset, vpCornerSample[3] + offset, scalar);
	line(imgShow, vpCornerSample[3] + offset, vpCornerSample[0] + offset, scalar);
	/* 从template 到 sample 透视后，template 中的点P(x, y)映射为对应坐标点P'(x', y')，
	因此，可以在显示图像上的坐标点P'(x', y')处，将其灰度值赋值为template的P(x, y)处的灰度值 */
	circle(imgShow, vpCornerSample[0]+offset, 2, scalar);
	circle(imgShow, vpCornerSample[1] + offset, 4, scalar);
	circle(imgShow, vpCornerSample[2] + offset, 6, scalar);
	circle(imgShow, vpCornerSample[3] + offset, 8, scalar);
	imshow("registration drawMatches", imgShow);
	/**/
}
/**/
void testPerspective(){
		Mat img = imread("template.jpg", 1);
		int img_height = img.rows;
		int img_width = img.cols;
		vector<Point2f> corners(4);
		corners[0] = Point2f(0, 0);
		corners[1] = Point2f(img_width - 1, 0);
		corners[2] = Point2f(0, img_height - 1);
		corners[3] = Point2f(img_width - 1, img_height - 1);
		vector<Point2f> corners_trans(4);
		corners_trans[0] = Point2f(200, 200);
		corners_trans[1] = Point2f(img_width - 1, 0);
		corners_trans[2] = Point2f(0, img_height - 1);
		corners_trans[3] = Point2f(img_width - 100, img_height - 1);

		Mat transform = getPerspectiveTransform(corners, corners_trans);
		vector<Point2f> ponits, points_trans;
		for(int i = 0; i<img_height; i++){
			for(int j = 0; j<img_width; j++){
				ponits.push_back(Point2f(j, i));
			}
		}
		perspectiveTransform(ponits, points_trans, transform);
		Mat img_trans = Mat::zeros(img_height, img_width, CV_8UC3);
		int count = 0;
		for(int i = 0; i<img_height; i++){
			uchar* p = img.ptr<uchar>(i);
			for(int j = 0; j<img_width; j++){
				int y = points_trans[count].y;
				int x = points_trans[count].x;
				uchar* t = img_trans.ptr<uchar>(y);
				t[x * 1] = p[j * 1];
				/**
				t[x * 3 + 1] = p[j * 3 + 1];
				t[x * 3 + 2] = p[j * 3 + 2];
				/**/
				count++;
			}
		}
		imshow("testPerspective", img_trans);
		imwrite("testPerspective.jpg", img_trans);
}

/**/
void test(){
	Mat tempImg = imread(g_imgName[0], 0);
	Vec3f v3fLeastSquaresFitting;
	/**/
	tempImg = Scalar::all(0);
	/**/
	map<int, Point> mapCentroid;
	Point center = Point(250, 250);
	int R = 200;
	double Ri=0, powRi2=0;
	for(int i = 0; i < tempImg.rows; i++){
		for(int j = 0; j < tempImg.cols; j++){
			powRi2 = pow(j-center.x, 2) + pow(i-center.y, 2);
			Ri = sqrt(powRi2);
			if(Ri - R <= 0){
				tempImg.at<uchar>(i, j) = 255;
			}
		}
	}
	/**/
	imshow("test", tempImg);
	vector<vector<Point>> contours;
	findContours(tempImg, contours, RETR_CCOMP, CHAIN_APPROX_NONE);
	cout<<"contours.size() = "<<contours.size()<<endl;
	waitKey(1000);
	tempImg = Scalar::all(0);
	drawContours(tempImg, contours, 0, Scalar::all(255));
	imshow("test", tempImg);
	waitKey(1000);
	for(int i = 0; i < contours[0].size(); i+=10){
		if(i < 200){
			continue;
		}
		mapCentroid.insert(pair<int, Point>(i, contours[0][i]));
	}
	cout<<"mapCentroid.size() = "<<mapCentroid.size()<<endl;
	tempImg = Scalar::all(0);
	for(map<int, Point>::iterator it = mapCentroid.begin(); it != mapCentroid.end(); it++){
		circle(tempImg, (*it).second, 0, Scalar::all(255));
		imshow("test", tempImg);
		waitKey(20);
	}
	leastSquaresFittingCircle(mapCentroid, v3fLeastSquaresFitting);
	/**/
	circle(tempImg, Point(v3fLeastSquaresFitting[0], v3fLeastSquaresFitting[1]),
		v3fLeastSquaresFitting[2], Scalar::all(100));
	circle(tempImg, Point(v3fLeastSquaresFitting[0], v3fLeastSquaresFitting[1]),
		5, Scalar::all(255));
		/**/
	imshow("test", tempImg);
	waitKey();
}
void testDrawHistogramFromUserData(){
	Mat imgCentroid = Mat::zeros(g_imgSrc.size(), g_imgSrc.type());
	map<int, int> mapProj;
	for(map<int, Point>::iterator it = g_mCentroid.begin(); it != g_mCentroid.end(); it++){
		circle(imgCentroid, (*it).second, 0, Scalar::all(255));
		imshow("testDrawHistogramFromUserData", imgCentroid);
		waitKey(20);
	}
	project(imgCentroid, mapProj, Point(g_v3fLeastSquaresFitting[0], g_v3fLeastSquaresFitting[1]), 360);
	waitKey(500);

	Mat imgShow;
	MatND userData = Mat(360, 1, CV_32F);
	for(int i = 0; i < 360; i++){
		userData.at<float>(i) = mapProj[i];
	}
	cout<<"testDrawHistogramFromUserData\n"<<userData<<endl;
	drawHistogramFromUserData(userData, imgShow);

}
void testProjectDirectly(){
	Mat imgShow;
	map<int, int> mapProj;
	MatND userData = Mat(360, 1, CV_32F);
	project(g_imgAnnular, mapProj, Point(g_v3fLeastSquaresFitting[0], g_v3fLeastSquaresFitting[1]), 360);
	for(int i = 0; i < 360; i++){
		userData.at<float>(i) = mapProj[i];
	}
	drawHistogramFromUserData(userData, imgShow);
}

/* ksize */
void trackbarCallback0(int trackbarValue, void* userdata){
	/* ksize */
	cout << "trackbarCallback0" << endl;
	Mat& userDataImg = *(Mat*)userdata;
	trackbar0(trackbarValue, userDataImg, g_imgBlur);
}
void trackbar0(int value, Mat& userdataimg, Mat& imgdst){
	int nKSize = value*2+1;
	medianBlur(userdataimg, imgdst, nKSize);
	imshow(g_mapWinName.at("imgBlur"), imgdst);
}


/* canny */
void trackbarCallback1(int trackbarValue, void* userdata){
	/* canny min */
	cout << "trackbarCallback1" << endl;
	Mat& userDataImg = *(Mat*)userdata;
	trackbar1(trackbarValue, userDataImg, g_imgCanny);
}
void trackbar1(int value, Mat& userdataimg, Mat& imgdst){
	int apertureSize = g_nKSize*2+1;
	if(apertureSize < 3){
		apertureSize = 3;
	}
	Canny(userdataimg, imgdst, g_nCannyMinThreshold, g_nCannyMaxThreshold, apertureSize);
	imshow(g_mapWinName.at("imgCanny"), imgdst);
}
void trackbarCallback2(int trackbarValue, void* userdata){
	/* canny max */
	cout << "trackbarCallback2" << endl;
	Mat& userDataImg = *(Mat*)userdata;
	trackbar2(trackbarValue, userDataImg, g_imgCanny);
}
void trackbar2(int value, Mat& userdataimg, Mat& imgdst){
	trackbar1(value, userdataimg, imgdst);
}

/* morphology */
void trackbarCallback3(int trackbarValue, void* userdata){
	/* morphology */
	cout << "trackbarCallback3" << endl;
	Mat& userDataImg = *(Mat*)userdata;
	trackbar3(trackbarValue, userDataImg, g_imgMorph);
}
void trackbar3(int value, Mat& userdataimg, Mat& imgdst){
	cout<<"trackbar3"<<endl;
	if(value > 6){
		return;
	}
	/**/
	myMorphologyEx(userdataimg, imgdst, value);
	/**/
}

/* threshold */
void trackbarCallback4(int trackbarValue, void* userdata){
	/* threshold */
	cout << "trackbarCallback4" << endl;
	Mat& userDataImg = *(Mat*)userdata;
	trackbar4(trackbarValue, userDataImg, g_imgThreshod);
}
void trackbar4(int value, Mat& userdataimg, Mat& imgdst){
	cout << "trackbar4" << endl;
	int nBSize = g_nThresholdBSize*2+1;
	if(nBSize < 3){
		userdataimg.copyTo(imgdst);
	}
	else{
		adaptiveThreshold(userdataimg, imgdst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, nBSize, g_nThresholdMeanC*0.01);
	}
	imshow(g_mapWinName.at("imgThreshold"), imgdst);
}
void trackbarCallback5(int trackbarValue, void* userdata){
	/* threshold */
	cout << "trackbarCallback5" << endl;
	Mat& userDataImg = *(Mat*)userdata;
	trackbar5(trackbarValue, userDataImg, g_imgMorph);
}
void trackbar5(int value, Mat& userdataimg, Mat& imgdst){
	cout << "trackbar5" << endl;
	trackbar4(value, userdataimg, imgdst);
}

/* good match */
void trackbarCallback6(int trackbarvalue, void* userdata){
	/**
	cout<<"trackbarCallback6"<<endl;
	/**/
	Mat& img = *(Mat*)userdata;
	trackbar6(trackbarvalue, img);
}
void trackbar6(int value, Mat& img){
	/**
	cout << "trackbar6" << endl;
	/**/
}
/**/
void mouseCallback(int event, int x, int y, int flags, void* userdata){
	VideoCapture& videoCapture = *(VideoCapture*)userdata;
	switch(event){
		case EVENT_LBUTTONDOWN:
			char key = 0;
			key = waitKey(2);
			cout<<" mouseCallback key = "<<(int)key<<" that's "<<key<<endl;
			if(key == '1'){
				Mat frame;
				videoCapture>>frame;
				imshow("template", frame);
				imwrite("./tempate.jpg", frame);
				cout << "get tempate" << endl;
			}
			break;
	}
}