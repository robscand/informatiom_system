/*!
 * \file
 * \brief Заголовочный файл.
 *
 * Данный файл содержит объявление класса, обеспечивающего
 * работу графического пользовательского интерфейса.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> ///< Включение определения класса QMainWindow.

//! Предварительные объявления классов Qt, использующихся для реализации главной формы.
class QAction;
class QActionGroup;
class QLabel;
class QTranslator;
class FindDialog;
class DataBase;
class MainForm;

/*!
 * \brief The MainWindow class
 *
 * Класс главного окна приложения, который создается в виде подкласса QMainWindow.
 * Этот класс реализует интерфейс программы с помощью методов:
 *  - отображающих и редактирующих данные в tableView;
 *  - формирующих главное меню и реализуюoих его функции;
 *  - читающих и сохраняющих настройки программы;
 *  - реализующих переключение интерфейса программы на другой язык.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainWindow
     *
     * Конструктор вызывается при создании объекта.
     */
    MainWindow();

protected:
    /*!
     * \brief closeEvent
     *
     * Виртуальная функция closeEvent() класса QWidget
     * вызывается при закрытии окна пользователем.
     * Она переопределяется в MainWindow для того,
     * чтобы сохранить на диске пользовательские настройки.
     * \param event Указатель на изменение состояния в этом событии
     */
    void closeEvent(QCloseEvent *event);
    /*!
     * \brief changeEvent
     *
     * Переопределение базовой виртуальной функции changeEvent класса QWidget
     * для вызова retranslateStrings() при генерации события
     * LanguageChange.
     * \param event Указатель на изменение состояния в этом событии
     */
    void changeEvent(QEvent *event);

signals:
    /*!
     * \brief groupLanguage
     *
     * Сигнал, подаваемый при активации действия языка из меню Language.
     */
    void groupLanguage(QAction *);

private slots:
    /*!
     * \brief newFile
     *
     * Слот newFile() вызывается при выборе пользователем пункта меню File | New
     * или при нажатии кнопки New на панели инструментов.
     */
    void newFile();
    /*!
     * \brief about
     *
     * Диалоговое окно About получается путем вызова удобной статической
     * функции QMessageBox::about().
     */
    void about();
    /*!
     * \brief setLanguage
     *
     * Слот, вызываемый при выборе пользователем действия смены языка.
     * \param language Переменная, принимающая действие, устанавливающее язык.
     */
    void setLanguage(QAction *language);
    /*!
     * \brief retranslateStrings
     *
     * В функции MainWindow::retranslateStrings() выполняются почти все вызовы tr()
     * для класса MainWindow. Она вызывается в конце конструктора MainWindow
     * и при каждом изменении пользователем языка приложения в
     * при помощи пункта меню Language.
     */
    void retranslateStrings();
    /*!
     * \brief setMyFont
     *
     * Слот setMyFont() соответствует пункту меню Options | Font.
     */
    void setMyFont();
    /*!
     * \brief setMyColor
     *
     * Слот setMyColor() соответствует пункту меню Options | Color.
     */
    void setMyColor();

private:
    /*!
     * \brief createActions
     *
     * Закрытая функция создает объекты QAction для элементов меню.
     * Текст пунктов меню устанавливается в retranslateStrings().
     */
    void createActions();
    /*!
     * \brief createMenus
     *
     * Закрытая функция создает пункты меню.
     */
    void createMenus();
    /*!
     * \brief createToolBars
     *
     * Закрытая функция создает панель инструментов.
     */
    void createToolBars();
    /*!
     * \brief readSettings
     *
     * Функция readSettings() загружает настройки,
     * которые были сохранены функцией writeSettings().
     */
    void readSettings();
    /*!
     * \brief writeSettings
     *
     * Функция writeSettings() используется для сохранения настроек.
     */
    void writeSettings();

    MainForm *mainForm;
    //! \details Путь к подготовленым файлам перевода
    QString qmPath;
    //! Объекты обеспечивающие перевод при отображении текста
    //! appTranslator хранит текущий перевод предложения
    QTranslator *appTranslator;
    //! qtTranslator хранит перевод библиотеки Qt
    QTranslator *qtTranslator;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *editSubMenu;
    QMenu *optionsMenu;
    QMenu *languageMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *newAction;
    QAction *exitAction;
    QAction *closeAction;

    QAction *editDepartmentAction;
    QAction *editStaffAction;
    QAction *editEmployeeAction;

    //! Указатель на объект QActionGroup (группы меню)
    QActionGroup *actionGroupLanguage;

    QAction *actionRussian;
    QAction *actionEnglish;

    QAction *fontAction;
    QAction *colorAction;

    QAction *aboutAction;
    /*! \details Для действия About Qt используется слот aboutQt() объекта
     * QApplication, который доступен через глобальную переменную qApp.
     */
    QAction *aboutQtAction;
};

#endif // MAINWINDOW_H
