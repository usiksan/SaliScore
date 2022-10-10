#ifndef CSSVGBOOK_H
#define CSSVGBOOK_H

#include "CsSvg.h"
#include "SvLib/SvSingleton.h"

class CsSvgBook
  {
    CsSvgPtrList mSvgList;
    int          mWidth;

    void clear();

    CsSvgBook();
  public:
    ~CsSvgBook();
    friend CsSvgBook *svInstance<CsSvgBook>();

    //Singleton function
    static CsSvgBook *instance() { return svInstance<CsSvgBook>(); }

    void   setScale( int w );

    CsSvg *svgNote( int duration, bool up ) const;

  };

#endif // CSSVGBOOK_H
