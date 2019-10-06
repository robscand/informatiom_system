/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Создание формы MainForm, которая обеспечивает представление отделов, персонала и его сотрудников
 * по принципу "главные-подчиненные". Форма обеспечивает просмотр таблиц базы данных с помощью форм или таблиц
 * пользовательского интерфейса.
 */
#include <QtGui>
#include <QApplication>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QModelIndex>
#include <QSplitter>
#include <QTableView>

#include "employeeform.h"
#include "departform.h"
#include "staffform.h"
#include "mainform.h"
#include "database.h"

MainForm::MainForm()
{
    //Создание объекта, который будет использоваться для работы с данными базы данных
    //и инициализировать подключение к базе данных
    db = new DataBase();
    db->connectToDataBase();
    //Функция создает и настраивает модель и представление, связанные с подразделениями
    createDepartmentPanel();
    createStaffPanel();
    createEmployeePanel();
    createActions();
    createContextMenu();
    
    //Настройка разделителя, в который входят три табличных представления + настройка кнопок формы
    lowSplitter = new QSplitter(Qt::Horizontal);
    lowSplitter->setFrameStyle(QFrame::StyledPanel);
    lowSplitter->addWidget(staffPanel);
    lowSplitter->addWidget(departmentPanel);

    upSplitter = new QSplitter(Qt::Vertical);
    upSplitter->setFrameStyle(QFrame::StyledPanel);
    upSplitter->addWidget(employeePanel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(upSplitter);
    mainLayout->addWidget(lowSplitter);
    setLayout(mainLayout);

    //Установка текущим элементом первого подразделения.
    departmentView->setCurrentIndex(departmentModel->index(0, 0));

    employeeView->setAcceptDrops(true); //Конструктор обеспечивает возможность приема переносимого объекта в виджете со списком.
    retranslateStrings();
}

void MainForm::createDepartmentPanel()
{
    departmentPanel = new QWidget;
    departmentModel = new QSqlRelationalTableModel(this);
    departmentModel->setTable("department");
    departmentModel->setRelation(Department_LocationId,
                                 QSqlRelation("location", "id", "name"));
    departmentModel->setSort(Department_Name, Qt::AscendingOrder);
    departmentModel->setHeaderData(Department_Name, Qt::Horizontal,
                                   tr("Dept."));
    departmentModel->setHeaderData(Department_LocationId,
                                   Qt::Horizontal, tr("Location"));
    departmentModel->select();

    departmentView = new QTableView;
    departmentView->setModel(departmentModel);
    departmentView->setItemDelegate(new QSqlRelationalDelegate(this));
    departmentView->setSelectionMode(QAbstractItemView::SingleSelection);
    departmentView->setSelectionBehavior(QAbstractItemView::SelectRows);
    departmentView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    departmentView->setColumnHidden(Department_Id, true);
    //Растяжение последнего поля, чтобы оно заняло все доступное пространство по горизонтали
    departmentView->horizontalHeader()->setStretchLastSection(true);
    departmentView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    departmentLabel = new QLabel(tr("Depar&tments"));
    departmentLabel->setBuddy(departmentView);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(departmentLabel);
    layout->addWidget(departmentView);
    departmentPanel->setLayout(layout);
}

void MainForm::createEmployeePanel()
{
    employeePanel = new QWidget;
    employeeModel = new QSqlRelationalTableModel(this);
    employeeModel->setTable("employee");
    employeeModel->setRelation(Employee_StaffId,
                               QSqlRelation("staff", "id", "name"));
    employeeModel->setRelation(Employee_DepartmentId,
                               QSqlRelation("department", "id", "name"));
    employeeModel->setRelation(Employee_SexId,
                               QSqlRelation("sex", "id", "name"));
    employeeModel->setHeaderData(Employee_StaffId, Qt::Horizontal,
                                 tr("Staff"));
    employeeModel->setHeaderData(Employee_DepartmentId, Qt::Horizontal,
                                 tr("Dept."));
    employeeModel->setHeaderData(Employee_SexId, Qt::Horizontal,
                                 tr("Sex"));
    employeeModel->setHeaderData(Employee_Salary, Qt::Horizontal,
                                 tr("Salary"));
    employeeModel->setHeaderData(Employee_StartDate, Qt::Horizontal,
                                 tr("Start"));
    employeeModel->select();

    employeeView = new QTableView;
    employeeView->setModel(employeeModel);
    employeeView->setSelectionMode(QAbstractItemView::SingleSelection);
    employeeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    employeeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    employeeView->horizontalHeader()->setStretchLastSection(true);
    employeeView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    employeeView->setColumnHidden(Employee_Id, true);
    employeeLabel = new QLabel(tr("E&mployees"));
    employeeLabel->setBuddy(employeeView);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(employeeLabel);
    layout->addWidget(employeeView);
    employeePanel->setLayout(layout);
}

void MainForm::createStaffPanel()
{
    staffPanel = new QWidget;
    staffModel = new QSqlTableModel(this);
    staffModel->setTable("staff");

    staffModel->setSort(Staff_Name, Qt::AscendingOrder);
    staffModel->setHeaderData(Staff_Name, Qt::Horizontal,
                                   tr("Staff."));
    staffModel->select();

    staffView = new QTableView;
    staffView->setModel(staffModel);
    //staffView->setItemDelegate(new QSqlRelationalDelegate(this));
    staffView->setSelectionMode(QAbstractItemView::SingleSelection);
    staffView->setSelectionBehavior(QAbstractItemView::SelectRows);
    staffView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    staffView->setColumnHidden(Staff_Id, true);
    //Растяжение последнего поля, чтобы оно заняло все доступное пространство по горизонтали
    staffView->horizontalHeader()->setStretchLastSection(true);
    staffView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    staffLabel = new QLabel(tr("Sta&ff"));
    staffLabel->setBuddy(staffView);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(staffLabel);
    layout->addWidget(staffView);
    staffPanel->setLayout(layout);
}

void MainForm::createActions()
{
    cutEmpAction = new QAction(employeePanel);
    cutEmpAction->setIcon(QIcon(":/images/cut.png"));
    cutEmpAction->setShortcut(QKeySequence::Cut);
    connect(cutEmpAction, SIGNAL(triggered()), this, SLOT(cutEmp()));

    copyEmpAction = new QAction(employeePanel);
    copyEmpAction->setIcon(QIcon(":/images/copy.png"));
    copyEmpAction->setShortcut(QKeySequence::Copy);
    connect(copyEmpAction, SIGNAL(triggered()), this, SLOT(copyEmp()));

    pasteEmpAction = new QAction(employeePanel);
    pasteEmpAction->setIcon(QIcon(":/images/paste.png"));
    pasteEmpAction->setShortcut(QKeySequence::Paste);
    connect(pasteEmpAction, SIGNAL(triggered()), this, SLOT(pasteEmp()));

}

void MainForm::createContextMenu()
{
    employeeView->addAction(cutEmpAction);
    employeeView->addAction(copyEmpAction);
    employeeView->addAction(pasteEmpAction);
    employeeView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainForm::retranslateStrings()
{ 
    cutEmpAction->setText(tr("Cu&t"));
    cutEmpAction->setStatusTip(tr("Cut the current selection's contents "
                               "to the clipboard"));

    copyEmpAction->setText(tr("&Copy"));
    copyEmpAction->setStatusTip(tr("Copy the current selection's contents "
                                "to the clipboard"));

    pasteEmpAction->setText(tr("&Paste"));
    pasteEmpAction->setStatusTip(tr("Paste the clipboard's contents into "
                                 "the current selection"));
}

void MainForm::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateStrings();
    QWidget::changeEvent(event);
}

void MainForm::editDepartments()
{
    int departId = -1;
    QModelIndex index = departmentView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = departmentModel->record(index.row());
        //Заменяем значение employeeId идентификатором текущего сотрудника, если возможно
        departId = record.value(Department_Id).toInt();
    }
    //Конструирование формы EmployeeForm
    DepartForm form(departId, this);
    //Отображение формы в модальном режиме
    form.exec();
    departmentModel->select();
    employeeModel->select();
}

void MainForm::editStaff()
{
    int staffId = -1;
    QModelIndex index = staffView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = staffModel->record(index.row());
        //Заменяем значение employeeId идентификатором текущего сотрудника, если возможно
        staffId = record.value(Staff_Id).toInt();
    }
    //Конструирование формы EmployeeForm
    StaffForm form(staffId, this);
    //Отображение формы в модальном режиме
    form.exec();
    staffModel->select();
    employeeModel->select();
}

void MainForm::editEmployees()
{
    //Присвоение недопустимого ID сотрудника
    int employeeId = -1;
    QModelIndex index = employeeView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = employeeModel->record(index.row());
        //Заменяем значение employeeId идентификатором текущего сотрудника, если возможно
        employeeId = record.value(Employee_Id).toInt();
    }
    //Конструирование формы EmployeeForm
    EmployeeForm form(employeeId, this);
    //Отображение формы в модальном режиме
    form.exec();
    //Обновление подчиненного табличного представления
    employeeModel->select();
}

void MainForm::cutEmp()
{
    QModelIndex index = employeeView->currentIndex();
    if (!index.isValid())
        return;

    copyEmp();

    QSqlRecord record = employeeModel->record(index.row());
    int id = record.value("id").toInt();

    QSqlQuery query;
    query.exec(QString("SELECT * FROM employee "
                "WHERE id = %1").arg(id));

    if (query.next())
        query.exec(QString("DELETE FROM employee "
                           "WHERE id = %1").arg(id));

    employeeModel->removeRow(index.row());
    employeeModel->submitAll();
    employeeModel->select();
    employeeView->setFocus();
}

void MainForm::copyEmp()
{
    QModelIndex index = employeeView->currentIndex();
    if (!index.isValid())
        return;

    QSqlRecord record = employeeModel->record(index.row());
    int id = record.value("id").toInt();

    QSqlQuery query;
    query.exec(QString("SELECT * FROM employee "
                   "WHERE id = %1").arg(id));

    if(query.next()) {
    bufferData << (new QVariant(query.value(Employee_StaffId)))
               << (new QVariant(query.value(Employee_DepartmentId)))
               << (new QVariant(query.value(Employee_SexId)))
               << (new QVariant(query.value(Employee_Salary)))
               << (new QVariant(query.value(Employee_StartDate)));
    }
        /*qDebug() << bufferData.at(0)->toInt() << bufferData.at(1)->toInt()
                 << bufferData.at(2)->toInt() << bufferData.at(3)->toDouble()
                 << bufferData.at(4)->toDate();*/
}

void MainForm::pasteEmp()
{
    if(bufferData.size() == 0) {
        QMessageBox::information(this, tr("Make copy"),
                             tr("The information cannot be pasted "
                                "because the copy is not made. "));
        return;
    }

    QModelIndex index = employeeView->currentIndex();
    if (index.isValid()) {
        int row = index.row();
        employeeModel->insertRow(row);
        employeeModel->setData(employeeModel->index(row, 1), bufferData.at(0)->toInt());
        employeeModel->setData(employeeModel->index(row, 2), bufferData.at(1)->toInt());
        employeeModel->setData(employeeModel->index(row, 3), bufferData.at(2)->toInt());
        employeeModel->setData(employeeModel->index(row, 4), bufferData.at(3)->toDouble());
        employeeModel->setData(employeeModel->index(row, 5), bufferData.at(4)->toDate());
        bufferData.clear();
        employeeModel->submitAll();
        employeeModel->select();
    }
}

