/*
Project "SaliScore Score music edit, view and tutorial program"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Defines interval of score for training
*/
#ifndef CSTRAININTERVAL_H
#define CSTRAININTERVAL_H

#include "CsCursorPosition.h"

#include <QList>

struct CsTrainInterval
  {
    CsCursorPosition mStart; //!< Start position of interval
    CsCursorPosition mStop;  //!< Stop position of interval

    CsTrainInterval();

    //!
    //! \brief jsonWrite Writes content into json writer object
    //! \param js        JSON writer object
    //!
    void     jsonWrite( CsJsonWriter &js ) const;

    //!
    //! \brief jsonRead Reads content from json reader object
    //! \param js       JSON reader object
    //!
    void     jsonRead( CsJsonReader &js );

  };


using CsTrainList = QList<CsTrainInterval>;


#endif // CSTRAININTERVAL_H
