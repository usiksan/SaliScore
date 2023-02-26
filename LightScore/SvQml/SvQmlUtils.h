/*
Project "Интерфейс специальных инструментов с QML"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Утилиты общего назначения. Предоставляют различные информационные функции.
*/
#ifndef SVQMLUTILS_H
#define SVQMLUTILS_H

#include <QObject>
#include "SvTime2x.h"

///
/// \brief The SvQmlUtils class - различные функции, необходимые, но недоступные в QML
///
class SvQmlUtils : public QObject
  {
    Q_OBJECT
  public:
    enum Platform  { Desktop, ARM };
  private:
    Q_ENUMS(Platform)
    Q_PROPERTY(Platform platform READ platform CONSTANT)
    Q_PROPERTY(QString applicationPath READ applicationPath CONSTANT)
    Q_PROPERTY(QString homePath READ homePath CONSTANT)
  public:
    explicit SvQmlUtils(QObject *parent = nullptr);

    //Доступ к платформе, на которой исполняется программа
    Platform       platform() const;

    //Получить путь к исполняемому файлу (каталог установки программы)
    static QString getApplicationPath();

    //Получить путь к домашнему каталогу
    static QString getHomePath();

    //Получить путь к файлу из url
    static QString getPathFromUrl( const QString url );

    //Обеспечить расширение
    static QString getWithExtension( const QString path, const QString extension );

    //Преобразовать целое, выраженное в мкм к строке
    static QString mcmToString( int mcm );

    //Преобразовать строку, выраженную в мм к целым мкм
    static int     stringToMcm( const QString mm );

    //Получить путь к исполняемому файлу (каталог установки программы)
    QString        applicationPath() const { return getApplicationPath(); }

    //Получить путь к домашнему каталогу
    QString        homePath() const { return getHomePath(); }

  public slots:
    //Получить путь к файлу только если он существует
    QString        path(const QString fname) const;

    //Проверить путь к файлу
    QString        pathDefault( const QString fname, const QString def ) const;

    //Преобразовать произвольную строку к имени файла
    //преобразование заключается в удалении всех знаков кроме букв, цифр, пробелов и подчеркиваний
    QString        stringToFileName( const QString str ) const;

    //очистка памяти
    void           purgeMemory();

    //Получить относительный путь к файлу
    QString        pathRef( const QString fullPath, const QString basePath ) const;

    //Выделить путь к файлу из url
    QString        pathFromUrl( const QString url ) const;

    //Обеспечить расширение
    QString        needExtension( const QString path, const QString extension ) const;

    //Получить максимальное-минимальное из двух чисел
    int            getIMax( int a, int b ) const { return qMax(a,b); }
    int            getIMin( int a, int b ) const { return qMin(a,b); }
    double         getFMax( double a, double b ) const { return qMax(a,b); }
    double         getFMin( double a, double b ) const { return qMin(a,b); }

    //Ограничение значений
    int            getILimit( int val, int minVal, int maxVal ) const;

    //Преобразовать целое, выраженное в мкм к строке
    QString        mcm2String( int mcm ) const { return mcmToString(mcm); }

    //Преобразовать строку, выраженную в мм к целым мкм
    int            string2Mcm( const QString mm ) const { return stringToMcm(mm); }

    QString        toLower( const QString str ) const { return str.toLower(); }

    //Получить текущий момент
    int            time2xCurrent() const { return SvTime2x::current(); }

    //Преобразовать из time2x в QDateTime
    QDateTime      time2xToLocal( int tm ) const { return SvTime2x::toLocal(tm); }

    int            time2xFromDateTime( const QDateTime &tm ) const { return SvTime2x::fromDateTime(tm); }

    QDateTime      dateTime( int year, int month, int day, int hour, int minute, int second ) const;

    //Преобразовать из time2x в double, представляющий собой количество мс от начала эпохи
    double         time2xToReal( int tm ) const;

  };

#endif // SVQMLUTILS_H
