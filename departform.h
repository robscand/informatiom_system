/*!
  \file
  \brief Заголовочный файл.

  Данный файл содержит объявление класса, обеспечивающего вывод формы,
  обрабатывающей информацию об отделах.
 */
#ifndef DEPARTFORM_H
#define DEPARTFORM_H

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
    Department_Id = 0, ///< Идентификатор строки
    Department_Name = 1, ///< Название отдела
    Department_LocationId = 2 ///< Идентификатор расположения
};

/*!
 * \brief The DepartForm class
 *
 * Класс DepartForm позволяет отредактировать информацию
 * об отделах.
 */
class DepartForm : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief DepartForm
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param id Текущий идентификатор сотрудника.
     * \param parent Указатель на родительский класс типа QWidget.
     */
    DepartForm(int id, QWidget *parent = 0);
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
    void addDepartment();
    /*!
     * \brief deleteDepartment
     *
     * Слот deleteDepartment вызывается,
     * когда пользователь нажимает кнопку Delete.
     */
    void deleteDepartment();

private:
    /// \details Для доступа к базе данных используем модель QSqlRelationalTableModel, тк нужно работать с внешними ключами.
    QSqlRelationalTableModel *tableModel;
    /// \details Класс QDataWidgetMapper позволяет связывать виджеты формы с соответствующими столбцами модели данных.
    QDataWidgetMapper *mapper;

    QLabel *departmentLabel;
    QLineEdit *departmentEdit;

    QLabel *locationLabel;
    QComboBox *locationComboBox;

    QPushButton *firstButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *lastButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *closeButton;
    QDialogButtonBox *buttonBox;
};

#endif // DEPARTFORM_H
