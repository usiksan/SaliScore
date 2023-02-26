/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Базовое свойство для JSON файлов
*/
#ifndef SVQMLJSONVALUE_H
#define SVQMLJSONVALUE_H

#include "SvQmlJsonFile.h"

#include <QObject>

class SvQmlJsonValue : public QObject
  {
    Q_OBJECT

  protected:
    SvQmlJsonFile *mFile;     //Файл, в котором расположено данное значение
    QString        mJsonName; //Имя свойства в файле JSON

    Q_PROPERTY(SvQmlJsonFile* file READ getFile WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString jsonName READ jsonName WRITE setJsonName NOTIFY jsonNameChanged)
  public:
    explicit SvQmlJsonValue(QObject *parent = nullptr);
    ~SvQmlJsonValue();

    //Доступ к файлу, к которому относится это значение
    SvQmlJsonFile *getFile() const { return mFile; }
    void           setFile( SvQmlJsonFile *fl );

    //Доступ к имени в файле JSON
    QString        jsonName() const { return mJsonName; }
    void           setJsonName( const QString nm );

  signals:
    //Вызывается при изменении файла, к которому подключено свойство
    void fileChanged();

    //Вызывается при изменении имени
    void jsonNameChanged();

  public slots:
    //Вызывается из JSON после загрузки объекта, чтобы скорректировать значение в соответствии с загруженным
    virtual void onLoadJson() = 0;

    //Вызывается из JSON перед сохранением объекта, чтобы обновить в модели файла все значения
    virtual void onSaveJson() = 0;

  protected:
    //Выдает истину, если json-файл присутствует, а также задано имя объекта
    bool isJsonPresent() const;
  };

#endif // SVQMLJSONVALUE_H
