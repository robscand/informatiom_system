/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Создание диалогового окно для редактирования информации об отделах.
 * При вызове этой формы она отображает данные об указанном отделе, если
 * ей был предоставлен допустимый идентификатор отдела, а в противном случае
 * отображает информацию о первом отделе.
 */
#include <QtGui>
#include <QtSql/QtSql>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "departform.h"

DepartForm::DepartForm(int id, QWidget *parent)
    : QDialog(parent)
{
    departmentEdit = new QLineEdit;

    departmentLabel = new QLabel(tr("Depar&tment:"));
    departmentLabel->setBuddy(departmentEdit);

    locationComboBox = new QComboBox;

    locationLabel = new QLabel(tr("&Location:"));
    locationLabel->setBuddy(locationComboBox);

    firstButton = new QPushButton(tr("<< &First"));
    previousButton = new QPushButton(tr("< &Previous"));
    nextButton = new QPushButton(tr("&Next >"));
    lastButton = new QPushButton(tr("&Last >>"));

    addButton = new QPushButton(tr("&Add"));
    deleteButton = new QPushButton(tr("&Delete"));
    closeButton = new QPushButton(tr("&Close"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);

    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("department");
    tableModel->setRelation(Department_LocationId,
                            QSqlRelation("location", "id", "name"));
    tableModel->setSort(Department_Name, Qt::AscendingOrder);
    tableModel->select();

    QSqlTableModel *relationModel =
            tableModel->relationModel(Department_LocationId);
    locationComboBox->setModel(relationModel);
    locationComboBox->setModelColumn(
                relationModel->fieldIndex("name"));

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    mapper->addMapping(departmentEdit, Department_Name);
    mapper->addMapping(locationComboBox, Department_LocationId);

    if (id != -1) {
        for (int row = 0; row <tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Department_Id).toInt() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }

    connect(firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addDepartment()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteDepartment()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    QHBoxLayout *topButtonLayout = new QHBoxLayout;
    //Установка правого верхнего и левого нижнего угла слоя
    topButtonLayout->setContentsMargins(20, 0, 20, 5);
    topButtonLayout->addStretch();
    topButtonLayout->addWidget(firstButton);
    topButtonLayout->addWidget(previousButton);
    topButtonLayout->addWidget(nextButton);
    topButtonLayout->addWidget(lastButton);
    topButtonLayout->addStretch();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topButtonLayout, 0, 0, 1, 3);
    mainLayout->addWidget(departmentLabel, 1, 0);
    mainLayout->addWidget(departmentEdit, 1, 1, 1, 2);
    mainLayout->addWidget(locationLabel, 2, 0);
    mainLayout->addWidget(locationComboBox, 2, 1, 1, 2);

    mainLayout->addWidget(buttonBox, 3, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 9);
    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    if (id == -1) {
        nextButton->setFocus();
    } else {
        departmentEdit->setFocus();
    }

    setWindowTitle(tr("Edit Departments"));
}


void DepartForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

void DepartForm::addDepartment()
{
    int row = mapper->currentIndex(); //Извлечение текущей строки
    mapper->submit(); //Вызов submit(), чтобы изменения текущей записи не были потеряны.
    tableModel->insertRow(row); //Вставка новой пустой строки
    mapper->setCurrentIndex(row); //mapper переходит к строке
    departmentEdit->clear();
    departmentEdit->setFocus();
}

void DepartForm::deleteDepartment()
{
    int row = mapper->currentIndex();
    QSqlDatabase::database().transaction();
    QSqlRecord record = tableModel->record(row);
    int id = record.value(Department_Id).toInt();
    int numEmployees = 0;

    QSqlQuery query(QString("SELECT COUNT(*) FROM employee "
                            "WHERE departmentid = %1").arg(id));
    if (query.next())
        //запрос, с целью определения сотрудников в подразделении
        numEmployees = query.value(0).toInt();
    if (numEmployees > 0) {
        int r = QMessageBox::warning(this, tr("Delete Department"),
                                     tr("Delete %1 and all its employees?")
                                     .arg(record.value(Department_Name).toString()),
                                     QMessageBox::Yes | QMessageBox::No);
        //Если пользователь нажимает НЕТ, выполняется откат транзакции и возврат из функции
        if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
            return;
        }
        //Удаление сотрудников подразделения
        query.exec(QString("DELETE FROM employe "
                           "WHERE departmentid = %1").arg(id));
    }
    tableModel->removeRow(row);
    mapper->submit();
    QSqlDatabase::database().commit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}
