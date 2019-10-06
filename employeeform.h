/*!
  \file
  \brief Заголовочный файл.

  Данный файл содержит объявление класса, обеспечивающего вывод формы,
  обрабатывающей информацию о сотрудниках.
 */
#ifndef EMPLOYEEFORM_H
#define EMPLOYEEFORM_H

#include <QDialog> ///< Включение определения класса QDialog

//! Предварительные объявления классов Qt, использующихся для реализации диалогового окна.
class QComboBox;
class QDataWidgetMapper;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlRelationalTableModel;

/*!
 * Для простоты обслуживания используется перечислимое значение
 * для присваивания индексам столбцов осмысленных имен
 */
enum {
    Employee_Id = 0, ///< Идентификатор строки
    Employee_StaffId = 1, ///< Идентификатор сотрудника
    Employee_DepartmentId = 2, ///< Идентификатор отдела
    Employee_SexId = 3, ///< Идентификатор пола сотрудника
    Employee_Salary = 4, ///< Заработная плата
    Employee_StartDate = 5 ///< Дата начала работы
};

/*!
 * \brief The EmployeeForm class
 *
 * Класс EmployeeForm позволяет перейти от общей информации о сотрудниках главной формы
 * к полной информации о конкретном сотруднике.
 */
class EmployeeForm : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief EmployeeForm
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param id Текущий идентификатор сотрудника.
     * \param parent Указатель на родительский класс типа QWidget.
     */
    EmployeeForm(int id, QWidget *parent = 0);
    /*!
     * \brief done
     *
     * Переопределение виртуальной функции QDialog::done(int r).
     * Вызывается при закрытии окна. Сохраняет все изменения,
     * сделланые пользоваетелем в виджете.
     * \param result Переменная кода результата работы окна.
     */
    void done(int result);

private slots:
    /*!
     * \brief addDepartment
     *
     * Слот addDepartment вызывается,
     * когда пользователь нажимает кнопку Add.
     */
    void addEmployee();
    /*!
     * \brief deleteDepartment
     *
     * Слот deleteDepartment вызывается,
     * когда пользователь нажимает кнопку Delete.
     */
    void deleteEmployee();

private:
    /// \details Для доступа к базе данных используем модель QSqlRelationalTableModel, тк нужно работать с внешними ключами.
    QSqlRelationalTableModel *tableModel;
    /// \details Класс QDataWidgetMapper позволяет связывать виджеты формы с соответствующими столбцами модели данных.
    QDataWidgetMapper *mapper;
    QLabel *staffLabel;
    QLabel *departmentLabel;
    QLabel *sexLabel;
    QLabel *salaryLabel;
    QLabel *startDateLabel;
    QComboBox *staffComboBox;
    QComboBox *departmentComboBox;
    QComboBox *sexComboBox;
    QLineEdit *salaryEdit;
    QDateEdit *startDateEdit;
    QPushButton *firstButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *lastButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *closeButton;
    QDialogButtonBox *buttonBox;
};

#endif // EMPLOYEEFORM_H
