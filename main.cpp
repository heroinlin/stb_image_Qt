#include "image_qt.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Image_Qt w;
    w.show();

    return a.exec();
}
