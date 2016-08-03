#include "dashboard.h"
#include <QApplication>
#include <login.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*   Login log;
    log.show();
  if(loginWindow==true){
        Dashboard w(this);
        w.show();
   }*/
    Dashboard w;
    w.show();
    return a.exec();
}
