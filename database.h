/*!
  \file
  \brief Заголовочный файл.

  Данный файл содержит объявление класса, обеспечивающего работу с базой данных.
 */
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject> ///< Включение определения класса QDialog
#include <QtSql/QtSql> ///< Включение определения всех классов и методов QtSql

/*!
 * \brief The DataBase class
 *
 * Класс содержит методы, которые используются для взаимодействия с базой данных,
 * а также предоставления интерфейсов для этого взаимодействия
 * с более высокоуровневыми прослойками приложения.
 */
class DataBase : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief DataBase
     *
     * Конструктор с параметром. Вызывается при создании объекта.
     * \param parent Указатель на родительский класс типа QObject.
     */
    DataBase(QObject *parent = 0);

    /*!
     * \brief connectToDataBase
     *
     * Подключение к базе данных
     */
    void connectToDataBase();

private:
    //!Внутренние методы для работы с базой данных

    /*!
     * \brief createConnection
     *
     * Установка соединения с базой данных
     * \return
     */
    bool createConnection();
    //void closeDataBase();
    /*!
     * \brief createFakeData
     *
     * Создание базы данных послеустановки соединения.
     * Для этого применяется QSqlQuery для выполнения
     * любой инструкции SQL, поддерживаемой используемой базой данных.
     */
    void createFakeData();
    //! Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase db;

};

#endif // DATABASE_H
