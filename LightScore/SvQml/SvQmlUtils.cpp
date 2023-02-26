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
#include "SvQmlUtils.h"
#include "SvDir.h"

#include <QCoreApplication>
#include <QDir>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QFile>
#include <QDateTime>

SvQmlUtils::SvQmlUtils(QObject *parent) :
  QObject(parent)
  {

  }




//Доступ к платформе, на которой исполняется программа
SvQmlUtils::Platform SvQmlUtils::platform() const
  {
#ifdef PLATFORM_ARM
  return ARM;
#else
  return Desktop;
#endif
  }




//Получить путь к исполняемому файлу (каталог установки программы)
QString SvQmlUtils::getApplicationPath()
  {
  SvDir dir( QCoreApplication::applicationDirPath() );
  return dir.slashedPath();
  }



//Получить путь к домашнему каталогу
QString SvQmlUtils::getHomePath()
  {
  SvDir dir( QDir::homePath() );
  return dir.slashedPath();
  }



//Получить путь к файлу из url
QString SvQmlUtils::getPathFromUrl(const QString url)
  {
  if( url.startsWith(QString("file://")) )
    return url.mid(7);
  return url;
  }




//Обеспечить расширение
QString SvQmlUtils::getWithExtension(const QString path, const QString extension)
  {
  if( path.endsWith(extension) )
    return path;
  return path + extension;
  }




//Преобразовать целое, выраженное в мкм к строке
QString SvQmlUtils::mcmToString(int mcm)
  {
  double value = mcm;
  value /= 1000.0;
  return QString::number( value, 'f', 3 );
  }





//Преобразовать строку, выраженную в мм к целым мкм
int SvQmlUtils::stringToMcm(const QString mm)
  {
  double dv = mm.toDouble();
  return static_cast<int>(dv * 1000.0);
  }







//Получить путь к файлу только если он существует
QString SvQmlUtils::path(const QString fname ) const
  {
  //Если файл существует, то вернуть путь
  if( QFile::exists( fname ) )
    return fname;
  //Если файла нету, то вернуть пустую строку
  return QString();
  }




//Проверить путь к файлу
QString SvQmlUtils::pathDefault(const QString fname, const QString def) const
  {
  //Если файл существует, то вернуть путь
  if( QFile::exists( fname ) )
    return fname;
  //Если файла нету, то вернуть путь по умолчанию
  return def;
  }




//Преобразовать произвольную строку к имени файла
//преобразование заключается в удалении всех знаков кроме букв, цифр, пробелов и подчеркиваний
QString SvQmlUtils::stringToFileName(const QString str) const
  {
  QString res;
  int i = 0;
  for( QChar ch : str ) {
    if( i++ > 127 ) break;
    if( ch.isLetterOrNumber() || ch == QChar('_') )
      res.append( ch );
    else
      res.append( QChar('_') );
    }
  return res;
  }




//очистка памяти
void SvQmlUtils::purgeMemory()
  {
  QQmlEngine *engine = QQmlApplicationEngine::contextForObject(this)->engine();
  engine->collectGarbage();
  engine->trimComponentCache();
  }




//Получить относительный путь к файлу
QString SvQmlUtils::pathRef(const QString fullPath, const QString basePath) const
  {
  QString path = pathFromUrl( fullPath );
  //Если базовый путь содержится в полном пути к файлу, то вычесть и вернуть часть
  if( path.startsWith(basePath) )
    return path.mid( basePath.length() );
  //иначе вернуть полный путь
  return path;
  }




//Выделить путь к файлу из url
QString SvQmlUtils::pathFromUrl(const QString url) const
  {
  return getPathFromUrl( url );
  }




//Обеспечить расширение
QString SvQmlUtils::needExtension(const QString path, const QString extension) const
  {
  return getWithExtension( path, extension );
  }




//Ограничение значений
int SvQmlUtils::getILimit(int val, int minVal, int maxVal) const
  {
  if( val < minVal ) return minVal;
  if( val > maxVal ) return maxVal;
  return val;
  }




QDateTime SvQmlUtils::dateTime(int year, int month, int day, int hour, int minute, int second) const
  {
  return QDateTime( QDate(year,month,day), QTime(hour,minute,second) );
  }




//Преобразовать из time2x в double, представляющий собой количество мс от начала эпохи
double SvQmlUtils::time2xToReal(int tm) const
  {
  double ep = SvTime2x::toEpoch(tm);
  return ep * 1000.0;
  }
