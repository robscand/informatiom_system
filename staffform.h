/*!
  \file
  \brief Заголовочный файл.

  Данный файл содержит объявление класса, обеспечивающего вывод формы,
  обрабатывающей информацию о составе персонала.
 */
#ifndef STAFFFORM_H
#define STAFFFORM_H

#include <QDialog> ///< Включение определения класса QDialog

//! Предварительные объявления классов Qt, использующихся для реализации диалогового окна.
class QComboBox;
class QDataWidgetMapper;
class QDateEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSqlTableModel;

/*!
 * Для простоты обслуживания используется перечислимое значение
 * для присваивания индексам столбцов осмысленных имен
 */
enum {
    Staff_Id = 0, ///< Идентификатор строки
    Staff_Name = 1 ///< Имя Фамилия сотрудника
};

/*!
 * \brief The StaffForm class
 *
 * Класс StaffForm позволяет отредактировать информацию
 * о составе персонала.
 */
class StaffForm : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief StaffForm
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param id Текущий идентификатор сотрудника.
     * \param parent Указатель на родительский класс типа QWidget.
     */
    StaffForm(int id, QWidget *parent = 0);
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
    void addStaff();
    /*!
     * \brief deleteDepartment
     *
     * Слот deleteDepartment вызывается,
     * когда пользователь нажимает кнопку Delete.
     */
    void deleteStaff();

private:
    /// \details Для доступа к базе данных используем модель QSqlRelationalTableModel, тк нужно работать с внешними ключами.
    QSqlTableModel *tableModel;
    /// \details Класс QDataWidgetMapper позволяет связывать виджеты формы с соответствующими столбцами модели данных.
    QDataWidgetMapper *mapper;

    QLabel *nameLabel;
    QLineEdit *nameEdit;

    QPushButton *firstButton;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QPushButton *lastButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *closeButton;
    QDialogButtonBox *buttonBox;
};

#endif // STAFFFORM_H
