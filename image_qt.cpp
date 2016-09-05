#include "image_qt.h"
#include "ui_image_qt.h"

Image_Qt::Image_Qt(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::Image_Qt)
{
	ui->setupUi(this);
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_LoadImage_clicked()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_Process_clicked()));
}

Image_Qt::~Image_Qt()
{
	delete ui;
}
void Image_Qt::on_LoadImage_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
	if (fileName != NULL)
	{
		char fileName_[256];
		sprintf(fileName_, "%s", fileName.toStdString().data());
		ori_image = load_image(fileName_,0,0,3);
		//ori_image = load_image_color("1.jpg", 0, 0);
		//rgbgr_image(ori_image);
		label->move(10, 10);
		showimg(ori_image);
	}
}
void Image_Qt::on_Process_clicked()
{
    flip_image(ori_image);
	showimg(ori_image);
}
#ifdef OPENCV

IplImage *image_to_Ipl(image p)
{
	int x, y, k;
	image copy = copy_image(p);
	constrain_image(copy);
	if (p.c == 3) rgbgr_image(copy);
	IplImage *disp = cvCreateImage(cvSize(p.w, p.h), IPL_DEPTH_8U, p.c);
	int step = disp->widthStep;
	for (y = 0; y < p.h; ++y){
		for (x = 0; x < p.w; ++x){
			for (k = 0; k < p.c; ++k){
				disp->imageData[y*step + x*p.c + k] = (unsigned char)(get_pixel(copy, x, y, k) * 255);
			}
		}
	}
	free_image(copy);
	return disp;
}
//QImage->IplImage *
static IplImage *ConvertToIplImage(const QImage &img)
{
	int nChannel = 0;
	if (img.format() == QImage::Format_RGB888)nChannel = 3;
	if (img.format() == QImage::Format_ARGB32)nChannel = 4;
	if (nChannel == 0)return false;

	IplImage *iplImg = cvCreateImageHeader(cvSize(img.width(), img.height()), 8, nChannel);
	iplImg->imageData = (char*)img.bits();

	if (nChannel == 3)
		cvConvertImage(iplImg, iplImg, CV_CVTIMG_SWAP_RB);

	return iplImg;
}

//Mat->QImage
static QImage ConvertToQImage(cv::Mat &mat)
{
	QImage img;
	int nChannel = mat.channels();
	if (nChannel == 3)
	{
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		img = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format_RGB888);
	}
	else if (nChannel == 4 || nChannel == 1)
	{
		img = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, QImage::Format_ARGB32);
	}

	return img;
}

//IplImage *->QImage 
static QImage ConvertToQImage(IplImage *iplImg)
{
	QImage img;
	int nChannel = iplImg->nChannels;
	if (nChannel == 3)
	{
		cvConvertImage(iplImg, iplImg, CV_CVTIMG_SWAP_RB);
		img = QImage((const unsigned char*)iplImg->imageData, iplImg->width, iplImg->height, QImage::Format_RGB888);
	}
	else if (nChannel == 4 || nChannel == 1)
	{
		img = QImage((const unsigned char*)iplImg->imageData, iplImg->width, iplImg->height, QImage::Format_ARGB32);
	}

	return img;
}

#endif // OPENCV
static QImage ConvertToQImage(image p)
{
	QImage img;
	int x, y, k;
	image copy = copy_image(p);
	int nChannel = p.c;
	if (nChannel == 3)
	{
		rgbgr_image(p);
		unsigned char* data=(unsigned char*)malloc(p.w*p.h*p.c*sizeof(unsigned char));
		int step = (p.w*p.c/4+p.w*p.c%4)*4;
		for (y = 0; y < p.h; ++y){
			for (x = 0; x < p.w; ++x){
				for (k = 0; k < p.c; ++k){
					data[y*step + x*p.c + k] = (unsigned char)(get_pixel(copy, x, y, k) * 255);
				}
			}
		}
		img = QImage((const unsigned char*)data, p.w,p.h, QImage::Format_RGB888);
	}
	else if (nChannel == 1)
	{
		unsigned char* data = (unsigned char*)malloc(p.w*p.h*p.c*sizeof(unsigned char));
		for (y = 0; y < p.h; ++y){
			for (x = 0; x < p.w; ++x){
				for (k = 0; k < p.c; ++k){
					data[y*p.w + x] = (unsigned char)(get_pixel(copy, x, y, k) * 255);
				}
			}
		}
		img = QImage((const unsigned char*)data, p.w, p.h, QImage::Format_Indexed8);
	}
	free_image(copy);
	return img;
}
void Image_Qt::showimg(image ori_image)
{
	//IplImage *dst = image_to_Ipl(ori_image);
	///*cvShowImage("dst", dst);
	//cvWaitKey(0);*/
	//img = ConvertToQImage(dst);
	img = ConvertToQImage(ori_image);
	//img = QImage((const unsigned char*)(dst->imageData), ori_image.w, ori_image.h, QImage::Format_ARGB32);
	label->setPixmap(QPixmap::fromImage(img));
	label->resize(label->pixmap()->size());
	label->show();
	//free_image(ori_image);
}