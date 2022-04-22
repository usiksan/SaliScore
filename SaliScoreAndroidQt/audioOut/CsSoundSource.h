/*
  Project "SaliScore Score music edit, view and tutorial programm"

  Author
    Sibilev Alexander S.
  Web
    www.saliLab.com
    www.saliLab.ru

  Description
    Abstract class for sound source
*/
#ifndef CSSOUNDSOURCE_H
#define CSSOUNDSOURCE_H

#include "CsConfig.h"
#include "CsSoundSample.h"

class CsSoundSource
  {
  public:
    CsSoundSource() {}

    //!
    //! \brief sample Return next sample of sound
    //! \return       Next sample of sound
    //!
    virtual CsSoundSample sample() = 0;

    //!
    //! \brief isStopped Return true when sound is stopped and can be remove from play list
    //! \return          True when sound is stopped and can be remove from play list
    //!
    virtual bool          isStopped() const = 0;
  };


using CsSoundSourcePtr = CsSoundSource*;

#endif // CSSOUNDSOURCE_H
