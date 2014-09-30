#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define CV_VERSION_NUMBER CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)


#ifdef _DEBUG
#pragma comment(lib, "opencv_video"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_videostab"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_ts"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_superres"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_stitching"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_photo"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_ocl"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_objdetect"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_nonfree"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_ml"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_legacy"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_imgproc"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_highgui"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_haartraining_engine.lib")
//#pragma comment(lib, "opencv_gpu"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_flann"CV_VERSION_NUMBER"d.lib")
//#pragma comment(lib, "opencv_features2d"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_core"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_contrib"CV_VERSION_NUMBER"d.lib")
#pragma comment(lib, "opencv_calib3d"CV_VERSION_NUMBER"d.lib")
#else
#pragma comment(lib, "opencv_video"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_videostab"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_ts"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_superres"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_stitching"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_photo"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_ocl"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_objdetect"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_nonfree"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_ml"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_legacy"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_imgproc"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_highgui"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_haartraining_engine.lib")
//#pragma comment(lib, "opencv_gpu"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_flann"CV_VERSION_NUMBER".lib")
//#pragma comment(lib, "opencv_features2d"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_core"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_contrib"CV_VERSION_NUMBER".lib")
#pragma comment(lib, "opencv_calib3d"CV_VERSION_NUMBER".lib")
#endif

void degradeJPEG(const Mat& src, Mat& dest, int q, int DCT_MODE, int& size, double& bpp);

void degradeJPEG16(const Mat& src, Mat& dest, int q, int DCT_MODE, int& size, double& bpp)
{
	double amp = 1;
	Mat src16;
	src.convertTo(src16,CV_16S,amp);

	Mat dest16 = Mat::zeros(src.size(),src16.type());
	
	int64 pre = getTickCount();
			
	degradeJPEG(src16, dest16, q,DCT_MODE,size, bpp);
	cout<<"time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	dest16.convertTo(dest,CV_8U, 1.0/amp);
}

void degradeJPEG8(const Mat& src, Mat& dest, int q, int& size, double& bpp)
{
	vector<uchar> buff;
	vector<int> param(2);
	param[0]=IMWRITE_JPEG_QUALITY;
	param[1]=q;
	imencode(".jpg",src, buff, param);
	size = (int)buff.size();
	bpp = 8.0 * (double)size/(double)src.size().area();

	dest = imdecode(buff,IMREAD_UNCHANGED);
}


int main()
{
	string wname = "jpeg";
	namedWindow(wname);

	int quality = 95; createTrackbar("q",wname,&quality,100);
	int sw = 0; createTrackbar("sw",wname,&sw,1);

	Mat src = imread("kodim01.png");
	Mat dest;

	int key = 0;
	while(key!='q')
	{
		int size;
		double bpp;

		if(sw==0)
		{
			int64 pre = getTickCount();
			degradeJPEG8(src, dest, quality, size, bpp); 
			cout<<"time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
			printf(" 8: %d %f %f\n",size,bpp,PSNR(src,dest));
		}
		else
		{			
			degradeJPEG16(src, dest, quality,2, size, bpp); 
			printf("16: %d %f %f\n",size,bpp,PSNR(src,dest));
		}

		imshow(wname,dest);
		key = waitKey(1);
	}

	return 0;
}