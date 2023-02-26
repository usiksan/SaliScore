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
#include "SvQmlJsonFile.h"
#include "SvDir.h"
#include "SvQmlJsonValue.h"
#include "SvQmlUtils.h"
#include "SvQmlJsonModel.h"
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QCoreApplication>
#include <QJsonArray>
#include <QDebug>


SvQmlJsonFile::SvQmlJsonFile(QObject *parent) :
  QObject(parent),
  mDirty(false)
  {
  //По умолчанию - храним в домашнем каталоге
  mPath = SvQmlUtils::getHomePath();

  //Задаем интервал сканирования по умолчанию, но таймер не запускаем
  //т.е. по умолчанию автосохранение отключено
  mAutoSave.setInterval( 1000 );

  //Настроить реакцию таймера
  connect( &mAutoSave, &QTimer::timeout, this, [=] () {
    //Если грязный, то сохраняем
    if( mDirty )
      save();
    });

  }




SvQmlJsonFile::~SvQmlJsonFile()
  {
  //Копия множества элементов
  SvQmlJsonValueSet tmp = mSet;
  //Для всех исходных элементов множества
  // установить файл в нуль, при этом элемент удаляется из множества
  for( SvQmlJsonValuePtr ptr : tmp )
    ptr->setFile( nullptr );

  SvQmlJsonModelSet tmod = mModels;
  for( SvQmlJsonModelPtr ptr : tmod )
    ptr->setFile( nullptr );
  }




//Добавление-удаление элементов
void SvQmlJsonFile::append(SvQmlJsonValue *val)
  {
  //Вставить элемент во множество
  mSet.insert( val );
  }

void SvQmlJsonFile::remove(SvQmlJsonValue *val)
  {
  mSet.remove( val );
  }




//Добавление-удаление моделей
void SvQmlJsonFile::appendModel(SvQmlJsonModel *model)
  {
  mModels.insert( model );
  }

void SvQmlJsonFile::removeModel(SvQmlJsonModel *model)
  {
  mModels.remove( model );
  }



void SvQmlJsonFile::setFile(const QString fl)
  {
  //Установить новое имя
  mFileName = fl;
  //Загрузить файл с новым именем
  load();
  }




//Доступ к пути для файла
void SvQmlJsonFile::setPath(const QString pt)
  {
  SvDir dir( pt );
  mPath = dir.slashedPath();
  //Построить этот полный путь
  QDir( mPath ).mkpath( mPath );
  //qDebug() << mPath;

  if( !mFileName.isEmpty() )
    //Загрузить файл с новым путем
    load();
  }





//Доступ к свойству автоматического сохранения
int SvQmlJsonFile::autoSave() const
  {
  return mAutoSave.isActive() ? mAutoSave.interval() : 0;
  }



void SvQmlJsonFile::setAutoSave(int period)
  {
  if( period ) {
    //Если ненулевой период, то установить период и запустить таймер
    mAutoSave.setInterval( period );
    mAutoSave.start();
    }
  else
    //Если период нулевой, то остановить таймер
    mAutoSave.stop();
  //Сигнал об измении системы автосохранения
  emit autoSaveChanged();
  }





//Сохранить JSON файл
bool SvQmlJsonFile::save()
  {
  return saveTo( mPath + mFileName );
  }



//Сохранить JSON файл под другим именем
bool SvQmlJsonFile::saveTo(const QString filePath)
  {
  //Если имя файла нулевое, то выход с ошибкой
  if( filePath.isEmpty() ) {
    emit saveComplete( false );
    return false;
    }

  //Чтобы выделить путь к файлу
  QFileInfo info( filePath );

  //Построить этот полный путь
  QDir dir( info.absoluteDir() );
  dir.mkpath( info.absolutePath() );

  //Сообщить всем моделям о сохранении (чтобы они обновили значения)
  for( SvQmlJsonModelPtr ptr : mModels )
    ptr->onSaveJson();

  //Сообщить всем значениям о сохранении (чтобы они обновили значения)
  for( SvQmlJsonValuePtr ptr : mSet )
    ptr->onSaveJson();

  //Открыть файл
  QFile file( filePath );
  if( file.open( QIODevice::WriteOnly) ) {
    QJsonDocument doc( mContens );
    file.write( doc.toJson() );
    file.close();
    emit saveComplete( true );
    mDirty = false;
    emit isDirtyChanged();
    return true;
    }
  emit saveComplete( false );
  return false;
  }




//Загрузить JSON файл
bool SvQmlJsonFile::load()
  {
  //qDebug() << mPath + mFileName;
  return loadFrom( mPath + mFileName );
  }




//Загрузить JSON файл под другим именем
bool SvQmlJsonFile::loadFrom(const QString filePath)
  {
  //Открыть файл
  QFile file( filePath );
  if( file.open( QIODevice::ReadOnly) ) {
    mContens = QJsonDocument::fromJson( file.readAll() ).object();
    file.close();

    //Сообщить всем элементам о загрузке (чтобы они обновили значения)
    for( SvQmlJsonValuePtr ptr : mSet )
      ptr->onLoadJson();

    //Сообщить всем моделям о загрузке чтобы они обновили свои базы
    for( SvQmlJsonModelPtr ptr : mModels )
      ptr->onLoadJson();

    mDirty = false;
    emit isDirtyChanged();

    //Отправить сигнал об успешной загрузке
    emit loadComplete( true );
    return true;
    }
  emit loadComplete( false );
  return false;
  }






//Получить строку из головного объекта
QString SvQmlJsonFile::asString(const QString name) const
  {
  if( mContens.contains( name ) )
    return mContens.value( name ).toString();
  return QString();
  }




//Установить строку в головной объект
void SvQmlJsonFile::setString(const QString name, const QString val)
  {
  mContens.insert( name, val );
  setDirty();
  }





//Получить массив строк из головного объекта
QStringList SvQmlJsonFile::asStringList(const QString name)
  {
  //Получить из объекта массив
  QJsonArray ar = mContens.value( name ).toArray();
  //Преобразовать массив json значений в массив строк
  QStringList list;
  for( QJsonValue val : ar )
    list.append( val.toString() );
  return list;
  }




//Установить массив строк в головной объект
void SvQmlJsonFile::setStringList(const QString name, QStringList list)
  {
  QJsonArray ar;
  for( QString &str : list )
    ar.append( QJsonValue(str) );
  setJsonArray( name, ar );
  }





//Получить массив произвольных объектов
QJsonArray SvQmlJsonFile::asJsonArray(const QString name)
  {
  return mContens.value( name ).toArray();
  }




void SvQmlJsonFile::setJsonArray(const QString name, QJsonArray ar)
  {
  mContens.insert( name, ar );
  setDirty();
  }



double SvQmlJsonFile::asReal(const QString name) const
  {
  //Заменить запятые на точки
  QString str = asString( name );
  str = str.replace( QChar(','), QString(".") );
  return str.toDouble();
  }







//Установить флаг грязности
void SvQmlJsonFile::setDirty()
  {
  mDirty = true;
  emit isDirtyChanged();
  }




//Сбросить флаг грязности
void SvQmlJsonFile::clearDirty()
  {
  mDirty = false;
  emit isDirtyChanged();
  }




//Удалить файл из текущего каталога
void SvQmlJsonFile::fileDelete(const QString fname)
  {
  QFile::remove( mPath + fname );
  }




//Удалить все файлы из текущего каталога
void SvQmlJsonFile::fileDeleteAll()
  {
  QDir dir( mPath );
  QFileInfoList list = dir.entryInfoList();
  for( QFileInfo info : list )
    fileDelete( info.fileName() );
  }



