/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Исходный файл вспомогательного класса, применяющегося для работы с информацией,
 * которая помещена в базу данных. В данном случае формирование SQL запросов прячется
 * по максимуму в данный класс, чтобы уменьшить количество связей в программном коде.
 */
#include <QtGui>
#include <QProgressDialog>
#include <QMessageBox>
#include <cstdlib>
#include <sys/time.h>

#include "mainform.h"
#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
}

void DataBase::connectToDataBase()
{
    bool existingData = QFile::exists("personnel.dat");
    if (createConnection())
        if (!existingData)
            createFakeData();
}

bool DataBase::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("personnel.dat");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;
}

//void DataBase::closeDataBase()
//{
    //db.close();
//}

void DataBase::createFakeData()
{
    QStringList names;
    names << "Eabha Biddell_2" << "Prentice Hutchison_1"
          << "Rameesha Davidge_2" << "Digby Roson_1" << "Nettah Newarch_1"
          << "Lewisha Middleton_2" << "Ahmed Hanmer_1"
          << "Jordyn-Leigh Lamant_2" << "Lindsay Bigham_2"
          << "Kaylay Weir_1" << "Sofia Weir_2" << "Coel Spurlock_1"
          << "Youcef Culpan_1" << "Lucy-Jasmine Blanchard_2"
          << "Ally Hodgkin_2" << "Ara Collinge_2" << "Luka Dempster_1"
          << "Samanta Winster_2" << "Keri Palin_1" << "Ruiridh Bisset_1"
          << "Norman Epworth_1" << "Kezia Raw_2"
          << "Kaylan-Thomas Swynford_1" << "Kashaf Benning_1"
          << "Norma Yair_2" << "Edan Bassett_1" << "Akshat Mcglasson_1"
          << "Philippa Upton_2" << "Tylor Rockliff_1" << "Aqdas Buckman_1"
          << "Briana Dowell_2" << "Querida North_2" << "Chelsay Botts_2"
          << "Kishanth Calloway_1" << "Jan Covington_2"
          << "Teighan Monson_1" << "Claudia Mendel_2" << "Kerra Doe_2"
          << "Kara Depp_2" << "Harlie Soole_2" << "Viggo Streeter_1"
          << "Ava Cofel_2" << "Catherine Balderston_2"
          << "Brendan Gosnay_1" << "Zhaoyun Haygarth_1" << "Deri Pepler_1"
          << "Vicki Hopwood_2" << "Amitra Bindless_2" << "Cerhys Hayton_2"
          << "Gwendoline Westall_2";

    QProgressDialog progress;
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle(QObject::tr("Staff Department"));
    progress.setLabelText(QObject::tr("Creating database..."));
    progress.setMinimum(0);
    progress.setMaximum(names.count() + 8);
    progress.setValue(1);
    qApp->processEvents();
    QSqlQuery query;
    query.exec("DROP TABLE department");
    query.exec("DROP TABLE employee");
    query.exec("DROP TABLE location");
    query.exec("DROP TABLE staff");
    query.exec("DROP TABLE sex");

    progress.setValue(2);
    qApp->processEvents();
    query.exec("CREATE TABLE location ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(40) NOT NULL)");
    progress.setValue(3);
    qApp->processEvents();
    query.exec("CREATE TABLE sex ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(7) NOT NULL)");
    progress.setValue(4);
    qApp->processEvents();
    query.exec("CREATE TABLE department ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(40) NOT NULL, "
               "locationid INTEGER NOT NULL, "
               "FOREIGN KEY (locationid) REFERENCES location)");
    progress.setValue(5);
    qApp->processEvents();
    query.exec("CREATE TABLE staff ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name VARCHAR(40) NOT NULL)");

    progress.setValue(6);
    qApp->processEvents();
    query.exec("CREATE TABLE employee ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "staffid INTEGER NOT NULL, "
               "departmentid INTEGER NOT NULL, "
               "sexid INTEGER NOT NULL, "
               "salary REAL NOT NULL, "
               "startdate DATE NOT NULL, "
               "FOREIGN KEY (staffid) REFERENCES staff, "
               "FOREIGN KEY (departmentid) REFERENCES department, "
               "FOREIGN KEY (sexid) REFERENCES sex)");

    progress.setValue(7);
    qApp->processEvents();
    query.exec("INSERT INTO location (name) VALUES ("
               "'Floor 18, 1129 Evanston Heights, New York, NY')");
    query.exec("INSERT INTO location (name) VALUES ("
               "'The Shed, Elmtree Drive, Boston, MA')");
    query.exec("INSERT INTO location (name) VALUES ("
               "'14 Valentine Buildings, Amor Street, Cambridge, MA')");
    query.exec("INSERT INTO location (name) VALUES ("
               "'Bunker Building, Silo Avenue, Los Angeles, CA')");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Sales', 1)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Marketing', 3)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Processing', 2)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Support', 4)");
    query.exec("INSERT INTO department (name, locationid) VALUES ("
               "'Research', 1)");
    query.exec("INSERT INTO sex (name) VALUES ('male')");
    query.exec("INSERT INTO sex (name) VALUES ('female')");

    progress.setValue(8);
    qApp->processEvents();
    query.prepare("INSERT INTO employee (staffid, departmentid, "
                  "sexid, salary, startdate) "
                  "VALUES (:staffid, :departmentid, :sexid, "
                  ":salary, :startdate)");

    //srand((unsigned)time(NULL)); способ 1
    //srand(clock()); способ 2 - измерение времени процессором в тактах от начала выполнения программы
    timeval t1; //способ 3
    gettimeofday (& t1, NULL);
    srand ((unsigned)(t1.tv_usec * t1.tv_sec));
    for (int i = 0; i < names.size(); ++i) {
        int n =  1 + (rand() % 49);
        query.bindValue(":staffid", n);
        query.bindValue(":departmentid", 1 + (rand() % 5));
        QString name = names.at(n);
        query.bindValue(":sexid", (name.remove(0, name.indexOf('_') + 1)).toInt());
        query.bindValue(":salary", (double)(50000 + (rand() % 100000)) / 100);
        query.bindValue(":startdate",
                QDate::currentDate().addDays(-(rand() % 3600)));
        query.exec();
        n = 0;
    }
    progress.setValue(9);
    qApp->processEvents();
    int count = 0;
    query.prepare("INSERT INTO staff (name) VALUES (:name)");
    foreach (QString name, names) {
        name.chop(2);
        query.bindValue(":name", name);
        query.exec();
        ++count;
        progress.setValue(count + 9);
    }
    progress.setValue(progress.maximum());
    qApp->processEvents();
}
