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
#include "SvQmlJsonValue.h"

SvQmlJsonValue::SvQmlJsonValue(QObject *parent) :
  QObject(parent),
  mFile(nullptr)
  {

  }



SvQmlJsonValue::~SvQmlJsonValue()
  {
  if( mFile != nullptr )
    mFile->remove( this );
  }



//Доступ к файлу, к которому относится это значение
void SvQmlJsonValue::setFile(SvQmlJsonFile *fl)
  {
  if( mFile )
    mFile->remove( this );

  mFile = fl;
  if( mFile ) {
    mFile->append( this );
    onLoadJson();
    }
  emit fileChanged();
  }




//Доступ к имени в файле JSON
void SvQmlJsonValue::setJsonName(const QString nm)
  {
  //Изменить имя
  mJsonName = nm;
  emit jsonNameChanged();
  //Обновить значение с новым именем
  onLoadJson();
  }




//Выдает истину, если json-файл присутствует, а также задано имя объекта
bool SvQmlJsonValue::isJsonPresent() const
  {
  return mFile != nullptr && !mJsonName.isEmpty();
  }


