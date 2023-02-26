/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Доступ к JSON-файлам из qml. Данный класс представляет собой один файл JSON.
  Этот класс напрямую предоставляет доступ к своим элементам через соответствующие функции.
  Объекты типа SvQmlJsonValue предоставляют доступ к отдельным элементам JSON-файла
*/
#ifndef SVQMLJSONFILE_H
#define SVQMLJSONFILE_H

#include <QObject>
#include <QSet>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QTimer>

class SvQmlJsonValue;
typedef SvQmlJsonValue *SvQmlJsonValuePtr;
typedef QSet<SvQmlJsonValuePtr> SvQmlJsonValueSet;

class SvQmlJsonModel;
typedef SvQmlJsonModel *SvQmlJsonModelPtr;
typedef QSet<SvQmlJsonModelPtr> SvQmlJsonModelSet;

class SvQmlJsonFile : public QObject
  {
    Q_OBJECT

    QTimer            mAutoSave;   //Автоматическое сохранение через определенный период
    QString           mFileName;   //Имя файла для настроек
    QString           mPath;       //Путь к файлу настроек
    SvQmlJsonValueSet mSet;        //Набор объектов типа SvQmlJsonValue завязанных на данный JSON-файл
    SvQmlJsonModelSet mModels;     //Набор моделей
    bool              mDirty;      //Обозначает, что объект изменился
    QJsonObject       mContens;    //Содержимое файла JSON

    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(int autoSave READ autoSave WRITE setAutoSave NOTIFY autoSaveChanged)
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY isDirtyChanged)
  public:
    explicit SvQmlJsonFile(QObject *parent = nullptr);
    ~SvQmlJsonFile();

    //Добавление-удаление элементов
    void         append( SvQmlJsonValue *val );
    void         remove( SvQmlJsonValue *val );

    //Добавление-удаление моделей
    void         appendModel( SvQmlJsonModel *model );
    void         removeModel( SvQmlJsonModel *model );

    //Доступ к имени файла
    QString      file() const { return mFileName; }
    void         setFile( const QString fl );

    //Доступ к пути для файла
    QString      path() const { return mPath; }
    void         setPath( const QString pt );

    //Доступ к свойству автоматического сохранения
    int          autoSave() const;
    void         setAutoSave( int period );

    //Доступ к свойству грязности
    bool         isDirty() const { return mDirty; }

  signals:
    void         saveComplete( bool success );
    void         loadComplete( bool success );
    void         fileChanged();
    void         pathChanged();
    void         autoSaveChanged();
    void         scanPeriodChanged();
    void         isDirtyChanged();
  public slots:
    //Сохранить JSON файл
    bool         save();

    //Сохранить JSON файл под другим именем
    virtual bool saveTo( const QString filePath );

    //Загрузить JSON файл
    bool         load();

    //Загрузить JSON файл под другим именем
    virtual bool loadFrom( const QString filePath );

    //Проверить наличие объекта в головном объекте
    bool         contains( const QString name ) const { return mContens.contains(name); }

    //Получить строку из головного объекта
    QString      asString( const QString name ) const;

    //Установить строку в головной объект
    void         setString( const QString name, const QString val );

    //Получить массив строк из головного объекта
    QStringList  asStringList( const QString name );

    //Установить массив строк в головной объект
    void         setStringList( const QString name, QStringList list );

    //Получить массив произвольных объектов
    QJsonArray   asJsonArray( const QString name );

    //Установить массив произвольных объектов в головной объект
    void         setJsonArray( const QString name, QJsonArray ar );

    //Доступ через целое
    int          asInt( const QString name ) const { return asString(name).toInt(); }
    void         setInt( const QString name, int val ) { setString( name, QString::number(val ) ); }

    //Доступ через double
    double       asReal( const QString name ) const;
    void         setReal( const QString name, double val ) { setString( name, QString::number( val, 'f', 3 ) ); }

    //Доступ через дробное, преобразованное к целым 0.001
    int          asFloat1000Int( const QString name ) const { return static_cast<int>(asReal(name) * 1000.0); }
    void         setFloat1000Int( const QString name, int val ) { setReal( name, static_cast<double>(val) / 1000.0 ); }

    //Установить флаг грязности
    void         setDirty();

    //Сбросить флаг грязности
    void         clearDirty();

    //Удалить файл из текущего каталога
    void         fileDelete( const QString fname );

    //Удалить все файлы из текущего каталога
    void         fileDeleteAll();


  };

#endif // SVQMLJSONFILE_H
