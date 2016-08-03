#include "login.h"
#include "ui_login.h"

#include "connection.h"
#include "logger.h"
#include "dashboard.h"
#include <QMessageBox>
#include <QDateTime>
#include <QSplashScreen>
#include <QTimer>
//bool loginWindow;
//connection conn2;
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
   // conn2.connOpen();
    loginWindow=false;
    ui->setupUi(this);
    this->setWindowTitle("Login - Patagonia Wings");
    QPixmap user(":/images/User-blue-icon.png");
    ui->label_pic->setPixmap(user);
    ui->lineEdit_rut->setFocus();
}

Login::~Login()
{
    delete ui;
    
}

void Login::on_pushButton_open_clicked()
{
    if (ui->lineEdit_rut->text()=="")
    {
        QMessageBox::warning(this,tr("Alerta"), tr("Ingrese Rut."));
        ui->lineEdit_rut->setFocus();
    }
    else if(ui->lineEdit_password->text()=="")
    {
        QMessageBox::warning(this,tr("Alerta"), tr("Ingrese Clave."));
        ui->lineEdit_password->setFocus();
    }
    else
    {
        connection conn;
        QSqlQuery* qry=new QSqlQuery(conn.mydb);
        if(qry->exec("select rut,password from users where rut='"+
                     ui->lineEdit_rut->text()+"' and password='"+
                     ui->lineEdit_password->text()+"'"))
        {
            int count=0;
            while(qry->next())
                count++;
            if(count==1)
            {
                //User and password is correct
                rutSignin = ui->lineEdit_rut->text();
                Logger::insert2Logger(ui->lineEdit_rut->text()," INFO ","Sign in recorded");

               this->hide();
              //  Login log(this);
              //  log.close();
                //Dashboard dash(this);
                //dash.show();
                loginWindow=true;
                QSplashScreen *splash=new QSplashScreen;
                splash->setPixmap(QPixmap(":images/logo.png"));
                splash->show();
                QTimer::singleShot(2500,splash,SLOT(close()));

            }
            if(count<1)
            {
                QPixmap user(":images/User-red-icon.png");
                ui->label_pic->setPixmap(user);
                QMessageBox::warning(this,tr("Alerta"), tr("Rut y/o contraseña incorrecta."));
                ui->lineEdit_rut->setFocus();
            }
        }
        else
        {
            qApp->beep();
            QMessageBox::critical(this,tr("Error:"), qry->lastError().text()+" "+qry->executedQuery());
            Logger::insert2Logger(ui->lineEdit_rut->text()," ERROR ",qry->lastError().text()+" -> "+qry->executedQuery());
        }
        delete qry;
    }

}

void Login::on_lineEdit_password_textChanged()
{
    QPixmap user(":images/User-blue-icon.png");
    ui->label_pic->setPixmap(user);
}

void Login::on_pushButton_cancel_clicked()
{
    rutSignin = "";
    //this->close();
    //QCoreApplication::quit();
    qApp->exit();

}
