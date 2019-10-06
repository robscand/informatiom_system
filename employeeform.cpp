/*!
 * \file
 * \brief Исполняемый файл.
 *
 * Создание диалогового окно для редактирования информации о сотрудниках.
 * При вызове этой формы она отображает данные об указанном сотруднике, если
 * ей был предоставлен допустимый идентификатор сотрудника, а в противном случае
 * отображает информацию о первом сотруднике.
 */
#include <QtGui>
#include <QtSql/QtSql>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QDataWidgetMapper>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "employeeform.h"

EmployeeForm::EmployeeForm(int id, QWidget *parent)
    : QDialog(parent)
{
    //Создание виджетов для редактирования полей. По одному для каждого поля.
    //Также создается метка, которая будет помещена около редактирующего виджета
    staffComboBox = new QComboBox;

    staffLabel = new QLabel(tr("Na&me:"));
    staffLabel->setBuddy(staffComboBox);

    departmentComboBox = new QComboBox;

    departmentLabel = new QLabel(tr("Depar&tment:"));
    departmentLabel->setBuddy(departmentComboBox);

    sexComboBox = new QComboBox;

    sexLabel = new QLabel(tr("S&ex:"));
    sexLabel->setBuddy(sexComboBox);

    salaryEdit = new QLineEdit;
    //С помощью класса QDoubleValidator строка редактирования Extension принимает числа в диапазоне от 500 до 1000
    salaryEdit->setValidator(new QDoubleValidator(500.0, 1000.0, 2, this));

    salaryLabel = new QLabel(tr("Sal&ary:"));
    salaryLabel->setBuddy(salaryEdit);

    startDateEdit = new QDateEdit;
    //Устанавливаем режим всплывающего окна календаря
    startDateEdit->setCalendarPopup(true);
    QDate today = QDate::currentDate();
    //Установка диапазона дат
    startDateEdit->setDateRange(today.addDays(-90), today.addDays(90));

    startDateLabel = new QLabel(tr("&Start Date:"));
    startDateLabel->setBuddy(startDateEdit);

    //Cоздание кнопок навигации(<<First, <Previous, Next>, Last>>)
    firstButton = new QPushButton(tr("<< &First"));
    previousButton = new QPushButton(tr("< &Previous"));
    nextButton = new QPushButton(tr("&Next >"));
    lastButton = new QPushButton(tr("&Last >>"));

    //Cоздание кнопок и помещение их в поле QDialogButtonBox
    addButton = new QPushButton(tr("&Add"));
    deleteButton = new QPushButton(tr("&Delete"));
    closeButton = new QPushButton(tr("&Close"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(addButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::AcceptRole);

    //Благодаря классу QSqlRelationalTableModel в поле с раскрывающимся списком
    //будут отображаться названия подразделений, а не их идентификаторы
    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("employee");
    //Функция setRelation() принимает индекс поля внешнего ключа и объект QSqlRelation
    //Конструктор QSqlRelation принимает имя таблицы (таблицы внешнего ключа), имя поля
    //внешнего ключа и имя поля для отображения значения, находящегося в поле внешнего ключа.
    tableModel->setRelation(Employee_StaffId,
                            QSqlRelation("staff", "id", "name"));
    tableModel->setRelation(Employee_DepartmentId,
                            QSqlRelation("department", "id", "name"));
    tableModel->setRelation(Employee_SexId,
                            QSqlRelation("sex", "id", "name"));
    //tableModel->setSort(Employee_StaffId, Qt::AscendingOrder);
    tableModel->select();

    QSqlTableModel *staffRelModel =
            tableModel->relationModel(Employee_StaffId);
    //Заменяем модель хранения данных QComboBox на свою реляционную модель,
    //используемую в QSqlRelationalTableModel
    staffComboBox->setModel(staffRelModel);
    //Тк связь имеет два столбца, нужно выбрать, какой будет отображаться в выпадающем списке
    //Индекс столбца name неизвестен после вызова функции setRelation()
    //fieldIndex() принимает имя поля, а возвращает нужный индекс
    staffComboBox->setModelColumn(
                staffRelModel->fieldIndex("name"));

    QSqlTableModel *depRelModel =
            tableModel->relationModel(Employee_DepartmentId);
    departmentComboBox->setModel(depRelModel);
    departmentComboBox->setModelColumn(
                depRelModel->fieldIndex("name"));

    QSqlTableModel *sexRelModel =
            tableModel->relationModel(Employee_SexId);
    sexComboBox->setModel(sexRelModel);
    sexComboBox->setModelColumn(
                sexRelModel->fieldIndex("name"));

    //Класс QDataWidgetMapper отображает поля одной записи базы данных в виджетах,
    //с которыми она связана, а также вносит изменения, сделанные в этих виджетах,
    //в базу данных
    mapper = new QDataWidgetMapper(this);
    //Установка автоматической фиксации изменений
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    //Передача модели для работы
    mapper->setModel(tableModel);
    //Передача объекта QSqlRelationalDelegate. Необходим для модели с внешними ключами
    //Делегат делает так, чтобы отображались значения из столбца QSqlRelation, а не идентификаторы
    //Делегат делает так, что при редактировании поля с выпадающим списком будет выводиться значение,
    //но в базу данных будет записываться значение индекса.
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    //Установка индексов полей на виджеты формы
    mapper->addMapping(staffComboBox, Employee_StaffId);
    mapper->addMapping(departmentComboBox, Employee_DepartmentId);
    mapper->addMapping(sexComboBox, Employee_SexId);
    mapper->addMapping(salaryEdit, Employee_Salary);
    mapper->addMapping(startDateEdit, Employee_StartDate);

    //Если при вызове диалогового окна был указан допустимый идентификатор сотрудника,
    //ищем запись с этим идентификатором и делаем ее текущей записью объекта mapper
    //В противном случае переходим к первой
    if (id != -1) {
        for (int row = 0; row <tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Employee_Id).toInt() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }
    //Объект mapper позволяет редактировать и осуществлять навигацию
    //Навигационные кнопки соединены напрямую со слотами объекта mapper.
    connect(firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addEmployee()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteEmployee()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));

    //Компановка
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
    mainLayout->addWidget(staffLabel, 1, 0);
    mainLayout->addWidget(staffComboBox, 1, 1, 1, 2);
    mainLayout->addWidget(departmentLabel, 2, 0);
    mainLayout->addWidget(departmentComboBox, 2, 1, 1, 2);
    mainLayout->addWidget(sexLabel, 3, 0);
    mainLayout->addWidget(sexComboBox, 3, 1);
    mainLayout->addWidget(salaryLabel, 4, 0);
    mainLayout->addWidget(salaryEdit, 4, 1, 1, 2);
    mainLayout->addWidget(startDateLabel, 5, 0);
    mainLayout->addWidget(startDateEdit, 5, 1);
    mainLayout->addWidget(buttonBox, 7, 0, 1, 3);
    mainLayout->setRowMinimumHeight(6, 10);
    mainLayout->setRowStretch(6, 1);
    mainLayout->setColumnStretch(2, 1);
    setLayout(mainLayout);

    //Фукция setFocus() устанавливает ввод с клавиатуры для этого виджета
    if (id == -1) {
        nextButton->setFocus();
    } else {
        staffComboBox->setFocus();
    }

    setWindowTitle(tr("Edit Employees"));
}

void EmployeeForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

//Слот addEmployee вызывается, когда пользователь нажимает кнопку Add.
void EmployeeForm::addEmployee()
{
    //Здесь отправка выполняется вручную. AutoSubmit работает, когда пользователь переносит фокус с одного виджета на другой.
    //Пользователь отредактирует поле и нажмет кнопку Add, при этом фокус не переведется.
    int row = mapper->currentIndex(); //Извлечение текущей строки
    mapper->submit(); //Вызов submit(), чтобы изменения текущей записи не были потеряны.
    tableModel->insertRow(row); //Вставка новой пустой строки
    mapper->setCurrentIndex(row); //mapper переходит к строке

    salaryEdit->clear();
    startDateEdit->setDate(QDate::currentDate());
    staffComboBox->setFocus(); //Устанавливаем фокус на первый виджет для ввода данных
}

void EmployeeForm::deleteEmployee()
{
    int row = mapper->currentIndex(); //Извлекаем текущую строку
    tableModel->removeRow(row); //Удаляем строку
    //Отправка изменения вручную, тк автоматическая отправка применяется только к изменениям записей.
    mapper->submit(); //Отправляем изменение
    //Устанавливае текущий индекс для строки, следующей за удаленной
    //или последнюю строку, если была удалена последняя строка.
    mapper->setCurrentIndex(qMin(row, tableModel->rowCount() - 1));
}
