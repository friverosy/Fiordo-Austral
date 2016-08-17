#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QtDebug>
#include <QString>
#include "logger.h"
#include <QMessageBox>
#include <qsqldatabase.h>

extern QString rutSignin;
extern QString error0;
extern QString error1;
extern QString rtPort;

class connection
{

public:
    QSqlDatabase mydb;
    bool debug;
    bool isOpenDB(){
        if(mydb.isOpen())
            return true;
        else
            return false;
        qDebug() <<"boolean base de datos "+debug;
    }


    void connClose()
    {
        if(mydb.isOpen())
        {
            mydb.close();
            mydb.removeDatabase(QSqlDatabase::defaultConnection);
            Logger::insert2Logger(rutSignin," ERROR ", "Base de datos cerrada correctamente");

        }
        else
            Logger::insert2Logger(rutSignin," ERROR ", "Tratando de cerrar DB ya cerrada...");
    }

    bool connOpen()
    {
        QFile file("./conexion.conf");
        QString ip;
        if(file.open(QFile::ReadOnly|QFile::Truncate)){
            QTextStream in(&file);
            ip = in.readAll();
            file.close();
            qDebug() <<"setear puerto "+ip;

        }
        else
        {
        }

//        QString serverName = "192.168.244.132\\SQLEXPRESS";
        QString serverName = ip+"\\SQLEXPRESS";
        QString dbName = "fiordo_austral";
        QString dbID = "admin";
        QString dbPW = "Axxezo2016";

        mydb = QSqlDatabase::addDatabase("QODBC");
        QString dsn = QString("DRIVER={SQL Server};SERVER=%1;DATABASE=%2;UID=%3;PWD=%4;").arg(serverName).arg(dbName).arg(dbID).arg(dbPW);

        mydb.setDatabaseName(dsn);
        mydb.setPort(1433);

        if(mydb.open()){
            qDebug() <<"Conexion al Servidor Realizada Exitosamente";
        }
        else
        {
            qDebug() <<"Error= " << mydb.lastError().text();
        }


        if(!mydb.open())
        {
            Logger::insert2Logger(rutSignin," ERROR ", mydb.lastError().text());
            return false;
        }
        else
            return true;
    }
    static bool insert2Db(const QString rut_user, const QString insert)
    {
        if(!insert.isEmpty()){
            connection conn;
            QSqlQuery* qry=new QSqlQuery(conn.mydb);
            qry->prepare(insert);
            if(qry->exec())
            {
                Logger::insert2Logger(rut_user," DEBUG ", qry->executedQuery());
                return true;
            }
            else
            {
                Logger::insert2Logger(rut_user," ERROR ", qry->lastError().text()+" -> "+qry->executedQuery());
                return false;
            }
        }
        else
            return false;
    }

    static QString getFirstFromDb(const QString rut_user, const QString query)
    {
        connection conn;
        QSqlQuery* qry=new QSqlQuery(conn.mydb);
        qry->prepare(query);
        if(qry->exec())
        {
            qry->first();
            if(!(qry->value(0).toString().isEmpty() or qry->value(0).toString().isNull()))
            {
                Logger::insert2Logger(rut_user," DEBUG ", query);
                return qry->value(0).toString();
            }
            else
            {
                //                Logger::insert2Logger(rut_user," EMPTY ", qry->executedQuery());
                return "";
            }
        }
        else
        {
            Logger::insert2Logger(rut_user," ERROR ", qry->lastError().text()+" -> "+qry->executedQuery());
            return "";
        }
    }

    static QString getLastFromDb(const QString rut_user, const QString query)
    {
        connection conn;
        QSqlQuery* qry=new QSqlQuery(conn.mydb);
        qry->prepare(query);
        if(qry->exec())
        {
            qry->last();
            if(qry->value(0).toString().isEmpty() or qry->value(0).toString().isNull())
                return qry->value(0).toString();
            else
                return "";
        }
        else
        {
            Logger::insert2Logger(rut_user," ERROR ", qry->lastError().text()+" -> "+qry->executedQuery());
            return "";
        }
    }

    static void updateDb(const QString rut_user, const QString query)
    {
        connection conn;
        QSqlQuery* qry=new QSqlQuery(conn.mydb);
        qry->prepare(query);
        if(!qry->exec())
            Logger::insert2Logger(rut_user," ERROR ", qry->lastError().text()+" -> "+qry->executedQuery());
        else
            Logger::insert2Logger(rut_user," DEBUG ", qry->executedQuery());
    }

public:
    connection();
};

#endif // CONNECTION_H
