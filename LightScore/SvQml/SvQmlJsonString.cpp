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
#include "SvQmlJsonString.h"

SvQmlJsonString::SvQmlJsonString(QObject *parent) :
  SvQmlJsonValue( parent ),
  mValue("")
  {

  }




//Доступ к значению в виде строки
void SvQmlJsonString::setValue(const QString val)
  {
  mValue = val;
  if( mFile )
    mFile->setDirty();
  emit valueChanged();
  emit asBoolChanged();
  emit asIntChanged();
  emit asRealChanged();
  }




//Доступ к значению в виде bool
void SvQmlJsonString::setAsBool(bool b)
  {
  if( b ) setValue( QString("true") );
  else    setValue( QString("false") );
  }



//Доступ к значению в виде int
void SvQmlJsonString::setAsInt(int i)
  {
  setValue( QString::number(i) );
  }




//Доступ к значению в виде double
void SvQmlJsonString::setAsReal(double v)
  {
  setValue( QString::number( v, 'f', 3 )  );
  }




//Вызывается из JSON после загрузки объекта, чтобы скорректировать значение в соответствии с загруженным
void SvQmlJsonString::onLoadJson()
  {
  if( mFile != nullptr && !mJsonName.isEmpty() ) {
    mValue = mFile->asString( mJsonName );
    emit valueChanged();
    emit asBoolChanged();
    emit asIntChanged();
    emit asRealChanged();
    }
  }




//Вызывается из JSON перед сохранением объекта, чтобы обновить в модели файла все значения
void SvQmlJsonString::onSaveJson()
  {
  if( mFile != nullptr && !mJsonName.isEmpty() )
    mFile->setString( mJsonName, mValue );
  }
