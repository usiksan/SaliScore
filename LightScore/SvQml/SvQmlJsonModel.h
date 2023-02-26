/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Данные из JsonArray воспринимает как модель прямоугольных данных
*/
#ifndef SVQMLJSONMODEL_H
#define SVQMLJSONMODEL_H

#include "SvQmlJsonValue.h"

#include <QAbstractListModel>
#include <QStringList>
#include <QJsonArray>

class SvQmlJsonModel : public QAbstractListModel
  {
    Q_OBJECT

  protected:
    SvQmlJsonFile *mFile;     //Файл, в котором расположено данное значение
    QString        mJsonName; //Имя свойства в файле JSON
    QJsonArray     mTable;    //Таблица данных
    QStringList    mFields;   //Список имен полей таблицы
    QSet<QString>  mIgnored;  //Список игнорируемых полей, т.е. таких полей, которые не грязнят файл

    Q_PROPERTY(SvQmlJsonFile* file READ getFile WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString jsonName READ jsonName WRITE setJsonName NOTIFY jsonNameChanged)
    Q_PROPERTY(QStringList fields READ fields WRITE setFields NOTIFY fieldsChanged)
    Q_PROPERTY(QStringList ignoredFields READ ignoredFields WRITE setIgnoredFields NOTIFY ignoredFieldsChanged)
  public:
    SvQmlJsonModel( QObject *parent = nullptr );
    ~SvQmlJsonModel() override;

    //Доступ к файлу, к которому относится это значение
    SvQmlJsonFile *getFile() const { return mFile; }
    void           setFile( SvQmlJsonFile *fl );

    //Доступ к имени в файле JSON
    QString        jsonName() const { return mJsonName; }
    void           setJsonName( const QString nm );

    //Доступ к списку полей
    QStringList    fields() const { return mFields; }
    void           setFields( QStringList f );

    //Доступ к списку игнорируемых полей
    QStringList    ignoredFields() const { return mIgnored.toList(); }
    void           setIgnoredFields( QStringList f );

  signals:
    //Вызывается при изменении файла, к которому подключено свойство
    void fileChanged();

    //Вызывается при изменении имени
    void jsonNameChanged();

    //Вызывается при изменении полей
    void fieldsChanged();

    //Вызывается при изменении списка игнорируемых полей
    void ignoredFieldsChanged();

    //При изменении значения поля
    void fieldChanged( int row, const QString fieldName, const QString value );

    //После изменения модели (загрузка, добавление и удаление рядов)
    void afterModelChanged();

    // QAbstractItemModel interface
  public:
    virtual int           rowCount(const QModelIndex &parent) const override;
    virtual QVariant      data(const QModelIndex &index, int role) const override;
    virtual bool          setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool          insertRows(int row, int count, const QModelIndex &parent) override;
    virtual bool          removeRows(int row, int count, const QModelIndex &parent) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    // SvQmlJsonValue interface
  public slots:
    //Вызывается после загрузки файла для обновления данных
    void    onLoadJson();

    //Вызывается перед сохранением для внесения изменений в файл
    void    onSaveJson();

    //Заменить всю json таблицу
    void    setJson( const QJsonArray &ar );

    //Добавить запись к таблице
    void    addRecord();

    //Вставить запись в модель
    void    insertRecord( int row );

    //Удалить запись из модели
    void    removeRecord( int row );

    //Доступ к данным модели
    QString asString( int row, const QString nm ) const;
    QString asStringByIndex( int row, int column ) const;
    void    setString( int row, const QString nm, const QString str );
    void    setStringByIndex( int row, int column, const QString str );
    int     asInt( int row, const QString nm ) const;
    void    setInt( int row, const QString nm, int val );
    double  asReal(int row, const QString nm) const;
    int     asFloat1000Int( int row, const QString name ) const { return static_cast<int>(asReal( row, name ) * 1000.0); }
    void    setIntAsReal( int row, const QString nm, int val, int factor );

    //Количество записей в базе
    int     count() const { return mTable.count(); }

    //Очистить базу данных
    void    clear();

    //Сортировка по заданному полю
    void    sort( const QString nm );

  };

#endif // SVQMLJSONMODEL_H
