
#include <QApplication>
#include "converterwizard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    ConverterWizard w;
    w.show();
    
    return a.exec();
}
