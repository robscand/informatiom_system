/*!
 * \file
 * \brief Исполняемый файл.
 *
 * В файле строится каркас графического пользовательского интерфейса приложения,
 * который имеет меню, панели инструментов и строку состояния;
 * имеет необходимые диалоговые окна для обработки виджетов главной формы
 * Основная часть кода связана с созданием пользовательского интерфейса
 * и настройки соответствующих соединений сигнал-слот.
 */
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QActionGroup>
#include <QTranslator>
#include <QMdiArea>
#include <QColor>
#include <QColorDialog>
#include <QFont>
#include <QFontDialog>

#include "mainwindow.h"
#include "mainform.h"

MainWindow::MainWindow()
{
    mainForm = new MainForm;
    setCentralWidget(mainForm);

    /* Создание системы меню */
    createActions();
    createMenus();
    createToolBars();

    readSettings();

    // Установка двух объектов QTranslator на объект QApplication.
    qmPath = qApp->applicationDirPath() + "/tr";
    appTranslator = new QTranslator;
    qtTranslator = new QTranslator;
    statusBar()->showMessage(tr("Path ")+qmPath);

    // устанавливаем атрибут Qt::WA_DeleteOnClose для предотвращения утечек памяти
    // при закрытии пользователем окна.
    // Приводит к полному разрушению окна при закрытии
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowIcon(QIcon(":/images/icon.png"));

    retranslateStrings();
}

void MainWindow::createActions()
{
    newAction = new QAction(this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    closeAction = new QAction(this);
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    exitAction = new QAction(this);
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    editDepartmentAction = new QAction(this);
    editDepartmentAction->setIcon(QIcon(":/images/dep.png"));
    connect(editDepartmentAction, SIGNAL(triggered()), mainForm, SLOT(editDepartments()));

    editStaffAction = new QAction(this);
    editStaffAction->setIcon(QIcon(":/images/staff.png"));
    connect(editStaffAction, SIGNAL(triggered()), mainForm, SLOT(editStaff()));

    editEmployeeAction = new QAction(this);
    editEmployeeAction->setIcon(QIcon(":/images/emp.png"));
    connect(editEmployeeAction, SIGNAL(triggered()), mainForm, SLOT(editEmployees()));

    fontAction = new QAction(this);
    connect(fontAction, SIGNAL(triggered()), this, SLOT(setMyFont()));

    colorAction = new QAction(this);
    connect(colorAction, SIGNAL(triggered()), this, SLOT(setMyColor()));

    aboutAction = new QAction(this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    actionGroupLanguage = new QActionGroup(this);
    connect(actionGroupLanguage, SIGNAL(triggered(QAction*)), SIGNAL(groupLanguage(QAction*)));
    connect(this, SIGNAL(groupLanguage(QAction*)), SLOT(setLanguage(QAction*)));

    actionRussian = new QAction(actionGroupLanguage);
    actionRussian->setCheckable(true);

    actionEnglish = new QAction(actionGroupLanguage);
    actionEnglish->setCheckable(true);
    actionEnglish->setChecked(true);

}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(this);
    fileMenu->addAction(newAction);
    fileMenu->addAction(exitAction);
    menuBar()->addMenu(fileMenu);

    editMenu = new QMenu(this);

    editSubMenu = new QMenu(this);
    editSubMenu->addAction(editEmployeeAction);
    editSubMenu->addAction(editDepartmentAction);
    editSubMenu->addAction(editStaffAction);

    editMenu->addMenu(editSubMenu);
    editMenu->addSeparator();
    editMenu->addAction(mainForm->cutEmpAction);
    editMenu->addAction(mainForm->copyEmpAction);
    editMenu->addAction(mainForm->pasteEmpAction);
    menuBar()->addMenu(editMenu);

    languageMenu = new QMenu(this);
    languageMenu->addActions(actionGroupLanguage->actions());
    menuBar()->addMenu(languageMenu);

    optionsMenu = new QMenu(this);
    optionsMenu->addAction(fontAction);
    optionsMenu->addAction(colorAction);
    menuBar()->addMenu(optionsMenu);

    menuBar()->addSeparator();

    helpMenu = new QMenu(this);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, fileToolBar);
    fileToolBar->addAction(newAction);

    editToolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, editToolBar);
    editToolBar->addAction(editEmployeeAction);
    editToolBar->addAction(editDepartmentAction);
    editToolBar->addAction(editStaffAction);
    editToolBar->addSeparator();
    editToolBar->addAction(mainForm->copyEmpAction);
    editToolBar->addAction(mainForm->pasteEmpAction);
    editToolBar->addAction(mainForm->cutEmpAction);
}

/* В функции выполняются все вызовы tr() для класса MainWindow и при каждом изменении пользователем языка приложения при помощи меню Language */
void MainWindow::retranslateStrings()
{
    /* Для каждого пункта меню QAction устанавливаем текст */
    newAction->setText(tr("&New"));
    /* Установка комментария в строке состояния */
    newAction->setStatusTip(tr("Create a new data base file"));

    closeAction->setText(tr("&Close"));
    closeAction->setStatusTip(tr("Close this window"));

    exitAction->setText(tr("E&xit"));
    /* Установка клавиши быстрого вызова. Используется для команд, у которых нет стандартных клавиш быстрого вызова. */
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));

    editDepartmentAction->setText(tr("&Edit Departments..."));
    editDepartmentAction->setStatusTip(tr("Open the edit form guide \"Department\""));

    editStaffAction->setText(tr("&Edit Staff..."));
    editStaffAction->setStatusTip(tr("Open the edit form guide \"Staff\""));

    editEmployeeAction->setText(tr("&Edit Employees..."));
    editEmployeeAction->setStatusTip(tr("Open the edit form main data table"));

    fontAction->setText(tr("&Font"));
    fontAction->setStatusTip(tr("Change the font of the "
                                "program interface"));

    colorAction->setText(tr("&Color"));
    colorAction->setStatusTip(tr("Change the color of the "
                                 "program interface"));

    aboutAction->setText(tr("&About"));
    aboutAction->setStatusTip(tr("Show the application's About box"));

    aboutQtAction->setText(tr("About &Qt"));
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));

    actionRussian->setText(tr("Russian"));
    actionEnglish->setText(tr("English"));

    /* Установка заголовков для окон и QMenu и QToolBar. */
    fileMenu->setTitle(tr("&File"));
    editMenu->setTitle(tr("&Edit"));
    editSubMenu->setTitle(tr("O&pen form"));
    languageMenu->setTitle(tr("Language"));
    optionsMenu->setTitle(tr("&Options"));
    helpMenu->setTitle(tr("&Help"));
    fileToolBar->setWindowTitle(tr("&File"));
    editToolBar->setWindowTitle(tr("&Edit"));
    setWindowTitle(tr("Staff Department"));
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
        retranslateStrings();
    QMainWindow::changeEvent(event);
}

void MainWindow::setLanguage(QAction *language)
{
    qtTranslator->load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);

    if(language->text() == tr("Russian")) {
        appTranslator->load("lang_ru", qmPath);

        qDebug() << qmPath;
        qApp->installTranslator(appTranslator); //appTranslator хранит текущий перевод предложения

        retranslateStrings();
        qDebug() << language->text();
    }

    if(language->text() == tr("English")) {
        appTranslator->load("lang_en", qmPath);
        qApp->installTranslator(appTranslator); //qtTranslator хранит перевод библиотеки Qt

        retranslateStrings();
        qDebug() << language->text();
    }

}

void MainWindow::newFile()
{
    MainWindow *win = new MainWindow;
    static int winNum = 1;
    QString title = tr("Staff Department %1").arg(winNum++);
    win->setWindowTitle(title);
    win->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::setMyColor() //Слот setColor() соответствует пункту меню Options | Color.
{
    QColor myColor = QColorDialog::getColor(Qt::white, this, tr("Choose color")); // Вызывается диалоговое окно выбора нового цвета.
    if (myColor.isValid()) {
        setPalette(QPalette(myColor));
    }
}

void MainWindow::setMyFont() //Слот соответствует пункту меню Options | Font.
{
    bool bOk;
    QString tmp = tr("Choose font");
    QFont myFont = QFontDialog::getFont(&bOk, QFont("Calibri", 12), //Вызывается диалоговое окно выбора нового шрифта.
                                      this, tmp);
    if(bOk) {
        this->setFont(myFont);
        mainForm->setFont(myFont);
    }
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About Data Base \"Staff Department\""),
                       tr("<h2>Staff Department 1.1</h2>"
                          "<p>Copyright &copy; 2017 MGSU ISTAS III-2. "
                          "Creator - Kravtsov Leonid Sergeevich. "
                          "Variant - ???."
                          "<p>Program in C ++ / Qt with GUI, containing a database of employees of the firm. "
                          "The database includes the following fields: the order number of the record, the names "
                          "of the employees of the company (from the directory of staff), "
                          "the department of the firm (from the department directory), the sex of the employee, "
                          "his salary and also the time of commencement of work in the department. "
                          "<p>Also, the database contains tables that play the role of directories. "
                          "The directory is the building material of a database relevant to the company. "
                          "Each directory contains a set of all the options from which the database can consist."
                          "<p>This is a useful application that has "
                          "a complete graphical user interface for the application, "
                          "menus, toolbars, status bar and necessary dialog boxes. "
                          "<p>\"Staff Department\" is demonstrates QAction, QMainWindow, QMenuBar, "
                          "QStatusBar, QTableWidget, QToolBar, and many other "
                          "Qt classes. "
                          "<p>Also this program introduces the QtSql module of Qt development tools, "
                          "which provides a platform-independent and database-type interface for accessing "
                          "SQL databases to databases."));
}

void MainWindow::writeSettings() //Функция writeSettings() используется для сохранения настроек
{
    /*QSettings cохраняет настройки в системном реестре (для Windows).
     * QSettings хранит настройки в виде пары "ключ-значение".
     * Здесь ключ подобен пути файловой системы */
    QSettings settings("ISTAS of MGSU", "Staff Department"); //В конструктор передается название учебного заведения и имя приложения. Эта информация используется для определения местоположения настроек.
    settings.setValue("geometry", saveGeometry());      //Далее создаем подключи и помещаем в них настройки.

    QColor myColor = palette().background().color();
    settings.setValue("myColor", myColor);

    QFont myFont = font();
    settings.setValue("myFont", myFont);
}

void MainWindow::readSettings() //Функция readSettings() загружает настройки, которые были сохранены функцией writeSettings().
{
    QSettings settings("ISTAS of MGSU", "Staff Department");
    //Второй аргумент функции value() определяет значение, принимаемое по умолчанию в случае отсутствия запрашиваемого параметра.
    //Значения по умолчанию будут использованы при первом запуске приложения.
    restoreGeometry(settings.value("geometry").toByteArray()); //Второй агрумент не задан - окно произвольного размера.

    QColor myColor = settings.value("myColor", QColor(Qt::white)).value<QColor>();
    setPalette(QPalette(myColor));

    QFont myFont = settings.value("myFont").value<QFont>();
    setFont(myFont);
    mainForm->setFont(myFont);
}
