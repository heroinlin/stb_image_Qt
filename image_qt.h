#ifndef IMAGE_QT_H
#define IMAGE_QT_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#ifdef OPENCV
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
#endif // OPENCV
extern "C" {
#include "src/image.h";  
}
//#include "Cimage.h"
namespace Ui {
class Image_Qt;
}

class Image_Qt : public QMainWindow
{
    Q_OBJECT

public:
    explicit Image_Qt(QWidget *parent = 0);
    ~Image_Qt();

private:
    Ui::Image_Qt *ui;
	image ori_image;   
	QLabel *label = new QLabel(this);
	QImage img;
	private slots:
	void on_LoadImage_clicked();
	void on_Process_clicked();
	void showimg(image ori_image);
};

#endif // IMAGE_QT_H
