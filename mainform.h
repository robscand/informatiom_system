/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса, обеспечивающего обработку
 * всей базы данных.
 */
#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget> ///< Включение определения класса QWidget

//! Предварительные объявления классов Qt, использующихся для реализации главной формы.
class QDialogButtonBox;
class QLabel;
class QModelIndex;
class QPushButton;
class QSplitter;
class QSqlRelationalTableModel;
class QSqlTableModel;
class QTableView;
class DataBase;

/*!
 * \brief The MainForm class
 *
 * Класс MainForm состоит из трех представлений QTable View,
 * находящихся в отношенийях "главное - подчиненное".
 * Главные представления - список отделов и список персонала.
 * Подчиненное представление  - список сотрудников в
 * текущих отделах.
 * Во всех представлениях  используются модели QSqlRelationalTableModel,
 * поскольку таблицы базы данных, которые они представляют, имеют поля
 * внешних ключей.
 */
class MainForm : public QWidget
{
    Q_OBJECT

public:
    /*!
     * \brief MainForm
     *
     * Конструктор вызывается при создании объекта.
     */
    MainForm();
    /*!
     * \brief retranslateStrings
     *
     * В функции MainForm::retranslateStrings() выполняется перевод строк виджетов.
     * Она вызывается в конце конструктора MainForm
     * и при каждом изменении пользователем языка приложения в MainWindow
     * при помощи пункта меню Language.
     */
    void retranslateStrings();

    QAction *copyEmpAction; ///< Указатель на действие копирования
    QAction *pasteEmpAction; ///< Указатель на действие вставки
    QAction *cutEmpAction; ///< Указатель на действие вырезания

public slots:
    /*!
     * \brief editEmployees
     *
     * Cлот editEmployees() вызывается, когда пользователь
     * нажимает кнопку EditEmployees.
     */
    void editEmployees();
    /*!
     * \brief editDepartments
     *
     * Cлот editDepartments() вызывается, когда пользователь
     * нажимает кнопку EditDepartments.
     */
    void editDepartments();
    /*!
     * \brief editStaff
     *
     * Слот editStaff() вызывается, когда пользователь
     * нажимает кнопку EditStaff.
     */
    void editStaff();

private slots:
    /*!
     * \brief cutEmp
     *
     * Слот cutEmp() соответствует пункту меню Edit | Cut.
     * Используется для выполнения операции вырезания строки.
     * Также может вызываться из контекстного меню.
     */
    void cutEmp();
    /*!
     * \brief copyEmp
     *
     * Слот copyEmp() соответствует пункту меню Edit | Copy.
     * Используется для выполнения операции копирования строки.
     * Также может вызываться из контекстного меню.
     */
    void copyEmp();
    /*!
     * \brief pasteEmp
     *
     * Слот pasteEmp() соответствует пункту меню Edit | Paste.
     * Используется для выполнения операции вставки строки в конец таблицы.
     * Также может вызываться из контекстного меню.
     */
    void pasteEmp();

protected:
    /*!
     * \brief changeEvent
     *
     * Переопределение базовой виртуальной функции changeEvent класса QWidget
     * для вызова retranslateStrings() при генерации события
     * LanguageChange.
     * \param event Указатель на изменение состояния в этом событии
     */
    void changeEvent(QEvent *event);

private:
    /*!
     * \brief createDepartmentPanel
     *
     * Вспомогательная функция конструктора, которая
     * настраивает для отделов модель и представление.
     * Представление - стандартный объект QTableView.
     */
    void createDepartmentPanel();
    /*!
     * \brief createStaffPanel
     *
     * Вспомогательная функция конструктора, которая
     * настраивает для персонала модель и представление.
     * Представление - стандартный объект QTableView.
     */
    void createStaffPanel();
    /*!
     * \brief createEmployeePanel
     *
     * Вспомогательная функция конструктора, которая
     * настраивает для списка сотрудников модель и представление.
     * Представление - стандартный объект QTableView.
     */
    void createEmployeePanel();
    /*!
     * \brief createContextMenu
     *
     * Для обеспечения в приложении контекстного меню добавляются
     * действия Cut, Copy, Paste в виджет employeeView и устанавливается
     * политика контекстного меню виджета на отображение контекстного меню
     * с этими действиями.
     */
    void createContextMenu();
    /*!
     * \brief createActions
     *
     * Создение действий для копирования, вырезания и вставки.
     */
    void createActions();

    //! \details Закрытый указатель на объект класса DataBase, использующийся для установки связи между формой и базой данных.
    DataBase *db;
    /*! \details Список указателей типа QVariant. Используется для записи, транспортировки и извлечения
     *  данных при выполнении функций копирования и вставки.
     */
    QList<QVariant*> bufferData;
    //! \details Создание указателя на объект класса QSqlRelationalTableModel для доступа к внешним ключам таблицы "location".
    QSqlRelationalTableModel *departmentModel;
    //! \details Создание указателя на объект класса QSqlRelationalTableModel для доступа к внешним ключам таблиц "staff", "department", "sex".
    QSqlRelationalTableModel *employeeModel;
    //! \details Создание указателя на табличную модель данных для персонала.
    QSqlTableModel *staffModel;
    QWidget *departmentPanel;
    QWidget *employeePanel;
    QWidget *staffPanel;
    QLabel *departmentLabel;
    QLabel *employeeLabel;
    QLabel *staffLabel;
    //! \details Создание указателя на представление для отображения модели данных отделов.
    QTableView *departmentView;
    //! \details Создание указателя на представление для отображения модели данных сотрудников.
    QTableView *employeeView;
    //! \details Создание указателя на представление для отображения модели данных персонала.
    QTableView *staffView;
    QSplitter *lowSplitter;
    QSplitter *upSplitter;
};

#endif // MAINFORM_H
