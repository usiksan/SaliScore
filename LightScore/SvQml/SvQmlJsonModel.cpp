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
#include "SvQmlJsonModel.h"
#include <QMultiMap>

SvQmlJsonModel::SvQmlJsonModel(QObject *parent) :
  //SvQmlJsonValue( parent )
  QAbstractListModel( parent ),
  mFile( nullptr )
  {

  }

SvQmlJsonModel::~SvQmlJsonModel()
  {
  if( mFile != nullptr )
    mFile->removeModel( this );
  }




//Доступ к файлу, к которому относится это значение
void SvQmlJsonModel::setFile(SvQmlJsonFile *fl)
  {
  if( mFile ) {
    if( mFile->isDirty() )
      onSaveJson();
    mFile->removeModel( this );
    }

  mFile = fl;
  if( mFile ) {
    mFile->appendModel( this );
    onLoadJson();
    }
  emit fileChanged();
  }





//Доступ к имени в файле JSON
void SvQmlJsonModel::setJsonName(const QString nm)
  {
  //Изменить имя
  mJsonName = nm;
  emit jsonNameChanged();
  //Обновить значение с новым именем
  onLoadJson();
  }





//Доступ к списку полей
void SvQmlJsonModel::setFields(QStringList f)
  {
  beginResetModel();
  mFields = f;
  endResetModel();
  emit fieldsChanged();
  }





//Доступ к списку игнорируемых полей
void SvQmlJsonModel::setIgnoredFields(QStringList f)
  {
  mIgnored.clear();
  for( QString s : f )
    mIgnored.insert( s );
  emit ignoredFieldsChanged();
  }



//Количество записей в таблице
int SvQmlJsonModel::rowCount(const QModelIndex &parent) const
  {
  Q_UNUSED(parent)
  return mTable.count();
  }





//Данные из таблицы
QVariant SvQmlJsonModel::data(const QModelIndex &index, int role) const
  {
  //Проверить допуск индексов
  if( !index.isValid() || index.row() >= mTable.count() || index.row() < 0 || role <= Qt::UserRole )
    return QVariant();

  QString nm = mFields.at( role - Qt::UserRole - 1 );
  return mTable.at( index.row() ).toObject().value( nm ).toVariant();
  }




bool SvQmlJsonModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
  //Проверить допуск индексов
  if( !index.isValid() || index.row() >= mTable.count() || index.row() < 0 || role <= Qt::UserRole )
    return false;

  QString nm = mFields.at( role - Qt::UserRole - 1 );
  setString( index.row(), nm, value.toString() );
  return true;
  }



bool SvQmlJsonModel::insertRows(int row, int count, const QModelIndex &parent)
  {
  //Проверить допуск индексов
  if( row > mTable.count() || row < 0 )
    return false;
  beginInsertRows(parent, row, row + count - 1 );
  QJsonObject obj;
  for( QString fn : mFields )
    obj.insert( fn, QJsonValue( QString("") ) );
  while( count > 0 ) {
    if( row < mTable.count() )
      mTable.insert( row, obj );
    else
      mTable.append( obj );
    row++;
    count--;
    }
  //Данные изменились, сообщить, что файл грязный
  if( mFile != nullptr )
    mFile->setDirty();
  endInsertRows();

  //Сообщить об изменении модели
  emit afterModelChanged();

  return true;
  }




bool SvQmlJsonModel::removeRows(int row, int count, const QModelIndex &parent)
  {
  //Проверить допуск индексов
  if( row + count > mTable.count() || row < 0 )
    return false;
  beginRemoveRows( parent, row, row + count - 1 );
  while( count > 0 ) {
    mTable.removeAt(row);
    count--;
    }
  //Данные изменились, сообщить, что файл грязный
  if( mFile != nullptr )
    mFile->setDirty();
  endRemoveRows();

  //Сообщить об изменении модели
  emit afterModelChanged();

  return true;
  }




Qt::ItemFlags SvQmlJsonModel::flags(const QModelIndex &index) const
  {
  Q_UNUSED( index )
  return Qt::ItemIsEditable | Qt::ItemIsEnabled;
  }




QHash<int, QByteArray> SvQmlJsonModel::roleNames() const
  {
  QHash<int, QByteArray> roles;
  int i = 0;
  for( QString fn : mFields )
    roles.insert( Qt::UserRole + 1 + i++, fn.toUtf8() );
  return roles;
  }




void SvQmlJsonModel::onLoadJson()
  {
  if( mFile != nullptr ) {
    beginResetModel();
    mTable = mFile->asJsonArray( mJsonName );
    endResetModel();
    //Сообщить об изменении модели
    emit afterModelChanged();
    }
  }



void SvQmlJsonModel::onSaveJson()
  {
  if( mFile != nullptr )
    mFile->setJsonArray( mJsonName, mTable );
  }



//Заменить всю json таблицу
void SvQmlJsonModel::setJson(const QJsonArray &ar)
  {
  beginResetModel();
  mTable = ar;
  endResetModel();
  //Сообщить об изменении модели
  emit afterModelChanged();
  }




//Добавить запись к таблице
void SvQmlJsonModel::addRecord()
  {
  insertRecord( mTable.count() );
  }




//Вставить запись в модель
void SvQmlJsonModel::insertRecord(int row)
  {
  insertRow( row );
  }




//Удалить запись из модели
void SvQmlJsonModel::removeRecord(int row)
  {
  removeRow( row );
  }




//Доступ к данным модели
QString SvQmlJsonModel::asString(int row, const QString nm) const
  {
  //Проверить допуск индексов
  if( row >= mTable.count() || row < 0 )
    return QString();

  return mTable.at( row ).toObject().value( nm ).toString();
  }





QString SvQmlJsonModel::asStringByIndex(int row, int column) const
  {
  //Проверить допуск индексов
  if( column >= mFields.count() || column < 0 )
    return QString();

  return asString( row, mFields.at(column) );
  }





void SvQmlJsonModel::setString(int row, const QString nm, const QString str)
  {
  //Проверить допуск индексов
  if( row >= mTable.count() || row < 0 )
    return;

  int column = mFields.indexOf( nm );
  if( column >= 0 ) {
    //Получить объект для модификации
    QJsonObject obj = mTable.at( row ).toObject();
    //Модифицировать объект
    obj.insert( nm, str );
    //Обновить по индексу
    mTable.replace( row, obj );
    emit dataChanged( index(row), index(row), QVector<int>( 1, Qt::UserRole + 1 + column ) );
    //Данные изменились, сообщить, что файл грязный
    if( mFile != nullptr && !mIgnored.contains(nm) )
      mFile->setDirty();
    //Сообщить об изменении данных
    emit fieldChanged( row, nm, str );
    }
  }




void SvQmlJsonModel::setStringByIndex(int row, int column, const QString str)
  {
  //Проверить допуск индексов
  if( column >= mFields.count() || column < 0 )
    return;

  setString( row, mFields.at( column ), str );
  }




int SvQmlJsonModel::asInt(int row, const QString nm) const
  {
  return asString( row, nm ).toInt();
  }




void SvQmlJsonModel::setInt(int row, const QString nm, int val)
  {
  setString( row, nm, QString::number(val) );
  }




double SvQmlJsonModel::asReal(int row, const QString nm ) const
  {
  //Заменить запятые на точки
  QString str = asString( row, nm );
  str = str.replace( QChar(','), QString(".") );
  return str.toDouble();
  }



//Сохранить целое как дробное с учетом фактора
void SvQmlJsonModel::setIntAsReal(int row, const QString nm, int val, int factor)
  {
  double v = val;
  double f = factor;
  setString( row, nm, QString::number( v / f, 'f', 3) );
  }



//Очистить базу данных
void SvQmlJsonModel::clear()
  {
  beginResetModel();
  mTable = QJsonArray();
  //Данные изменились, сообщить, что файл грязный
  if( mFile != nullptr )
    mFile->setDirty();
  endResetModel();

  //Сообщить об изменении модели
  emit afterModelChanged();
  }





//Сортировка по заданному полю
void SvQmlJsonModel::sort(const QString nm)
  {
  //Мультикарта хранится отсортированной
  QMultiMap<QString,int> sortedIndirect;

  //Накопим ассоциации значение поля - номер записи
  for( int i = 0; i < mTable.count(); i++ )
    sortedIndirect.insert( asString(i,nm), i );

  //Новый отсортированный массив
  QJsonArray na;

  //Перенести из исходного массива в целевой с учетом сортировки
  for( QMultiMap<QString,int>::const_iterator i = sortedIndirect.cbegin(); i != sortedIndirect.cend(); i++ )
    na.append( mTable.at( i.value() )  );

  //Обновить в визуальном представлении
  beginResetModel();
  mTable = na;
  //Данные изменились, сообщить, что файл грязный
  if( mFile != nullptr )
    mFile->setDirty();
  endResetModel();

  //Сообщить об изменении модели
  emit afterModelChanged();
  }



