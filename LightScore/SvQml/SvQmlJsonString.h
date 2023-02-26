/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Доступ к свойству сохраняемому в json в виде строки. При чтении файла json значение строки обновляется из файла.
  При изменении строки автоматически обновляется значение и в json-файле.
  Обеспечено автоматическое преобразование строки в bool и int
*/
#ifndef SVQMLJSONSTRING_H
#define SVQMLJSONSTRING_H

#include "SvQmlJsonValue.h"

class SvQmlJsonString : public SvQmlJsonValue
  {
    Q_OBJECT

    QString mValue; //Текущее строковое значение

    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool asBool READ asBool WRITE setAsBool NOTIFY asBoolChanged)
    Q_PROPERTY(int asInt READ asInt WRITE setAsInt NOTIFY asIntChanged)
    Q_PROPERTY(double asReal READ asReal WRITE setAsReal NOTIFY asRealChanged)
  public:
    explicit SvQmlJsonString(QObject *parent = nullptr);

    //Доступ к значению в виде строки
    QString value() const { return mValue; }
    void    setValue( const QString val );

    //Доступ к значению в виде bool
    bool    asBool() const { return mValue == QString("true"); }
    void    setAsBool( bool b );

    //Доступ к значению в виде int
    int     asInt() const { return mValue.toInt(); }
    void    setAsInt( int i );

    //Доступ к значению в виде double
    double  asReal() const { return mValue.toDouble(); }
    void    setAsReal( double v );

  signals:
    void valueChanged();
    void asBoolChanged();
    void asIntChanged();
    void asRealChanged();

  public slots:
    //Вызывается из JSON после загрузки объекта, чтобы скорректировать значение в соответствии с загруженным
    virtual void onLoadJson();

    //Вызывается из JSON перед сохранением объекта, чтобы обновить в модели файла все значения
    virtual void onSaveJson();
  };

#endif // SVQMLJSONSTRING_H
