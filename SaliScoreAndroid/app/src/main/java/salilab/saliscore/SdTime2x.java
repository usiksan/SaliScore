/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Common short time

  All time in seconds 32-bit from begin of year 2018.
*/
package salilab.saliscore;

import java.util.Calendar;
import java.util.Date;

public class SdTime2x {
  //Start time is begin of year 2018 in seconds from epoch
  final static long SD_TIME_2X_START = 1514764800l;

  //Current moment [Текущий глобальный момент]
  static int       current() {  return fromEpoch( Calendar.getInstance().getTimeInMillis() / 1000l ); }

  //Conversion to-from epich [Взаимодействие со временем относительно эпохи]
  //Convert to epoch [Преобразовать в эпоху]
  static long      toEpoch( int tm ) { return SD_TIME_2X_START + tm; }

  //Convert from epoch [Преобразовать из эпохи]
  static int       fromEpoch( long tm ) {
    //При отсутствии времени возвращаем ноль обозначающий тоже отсутствие времени
    if( tm == 0 )
      return 0;
    return (int) (tm - SD_TIME_2X_START);
  }


  //Get real utc date [Получить реальную дату]
  static Date toUtc( int tm ) {
    if( tm <= 0 )
      return new Date();
    return new Date( toEpoch(tm) * 1000 );
  }

  //Get real local date [Получить реальную локальную дату]
  static Date toLocal( int tm ) {
    return toUtc( tm );
  }

  //Convert from QDataTime [Преобразовать из момента]
  static int    fromDateTime( Date tm ) {
    return fromEpoch( tm.getTime() / 1000 );
  }


  //Convert to string representation of time
  static String    toLocalString( int tm ) {
    return toUtc(tm).toString();
  }


}
