/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Создание диалогового окно для редактирования информации о составе персонала.
 * При вызове этой формы она отображает данные об указанном члене персонала, если
 * ей был предоставлен допустимый идентификатор сотруднике из персонала, а в противном случае
 * отображает информацию о первом члене персонала.
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

#include "staffform.h"

StaffForm::StaffForm(int id, QWidget *parent)
    : QDialog(parent)
{
    nameEdit = new QLineEdit;

    nameLabel = new QLabel(tr("Na&me:"));
    nameLabel->setBuddy(nameEdit);

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

    tableModel = new QSqlTableModel(this);
    tableModel->setTable("staff");
    tableModel->setSort(Staff_Name, Qt::AscendingOrder);
    tableModel->select();

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    //mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    mapper->addMapping(nameEdit, Staff_Name);

    if (id != -1) {
        for (int row = 0; row <tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Staff_Id).toInt() == id) {
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
    connect(addButton, SIGNAL(clicked()), this, SLOT(addStaff()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteStaff()));
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
    mainLayout->addWidget(nameLabel, 1, 0);
    mainLayout->addWidget(nameEdit, 1, 1, 1, 2);

    mainLayout->addWidget(buttonBox, 2, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 9);
    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    if (id == -1) {
        nextButton->setFocus();
    } else {
        nameEdit->setFocus();
    }

    setWindowTitle(tr("Edit Staff"));
}

void StaffForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

void StaffForm::addStaff()
{
    int row = mapper->currentIndex(); //Извлечение текущей строки
    mapper->submit(); //Вызов submit(), чтобы изменения текущей записи не были потеряны.
    tableModel->insertRow(row); //Вставка новой пустой строки
    mapper->setCurrentIndex(row); //mapper переходит к строке
    nameEdit->clear();
    nameEdit->setFocus();
}

void StaffForm::deleteStaff()
{
    int row = mapper->currentIndex();
    QSqlDatabase::database().transaction();
    QSqlRecord record = tableModel->record(row);
    int id = record.value(Staff_Id).toInt();
    int numEmployees = 0;

    QSqlQuery query(QString("SELECT COUNT(*) FROM employee "
                            "WHERE staffid = %1").arg(id));
    if (query.next())
        //запрос, с целью определения сотрудников в подразделении
        numEmployees = query.value(0).toInt();
    if (numEmployees > 0) {
        int r = QMessageBox::warning(this, tr("Delete Staff member?"),
                                     tr("Delete %1 and all information about him?")
                                     .arg(record.value(Staff_Name).toString()),
                                     QMessageBox::Yes | QMessageBox::No);
        //Если пользователь нажимает НЕТ, выполняется откат транзакции и возврат из функции
        if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
            return;
        }
        //Удаление сотрудников подразделения
        query.exec(QString("DELETE FROM employe "
                           "WHERE staffid = %1").arg(id));
    }
    tableModel->removeRow(row);
    mapper->submit();
    QSqlDatabase::database().commit();
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}
