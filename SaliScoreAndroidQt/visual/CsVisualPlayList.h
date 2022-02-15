#ifndef CSVISUALPLAYLIST_H
#define CSVISUALPLAYLIST_H

#include "config.h"
#include "score/CsPlayList.h"
#include "CsVisualAbstractList.h"

class CsVisualPlayList : public CsVisualAbstractList
  {
    Q_OBJECT

    CsPlayList &mPlayList; //!< Play list which displayed with this widget
    int         mDelIconX; //!< X position of delete icon
  public:
    CsVisualPlayList( CsPlayList &playList, QWidget *parent = nullptr );

    virtual int  itemHeight( int index ) const override;

    virtual int  paintItem( int index, int y, QPainter &painter ) override;

    virtual int  count() const override { return mPlayList.partCount() + 8; }

    virtual void clicked( int x, int itemIndex ) override;

  signals:
    //!
    //! \brief selectPart Signal emited when part with partIndex need to be activated
    //! \param partIndex  Index of activated part
    //!
    void selectPart( int partIndex );

    void selectSettings();
    void selectHelp();
    void web();

    void selectNoteTrainer();
    void selectGammaTrainer();

  public slots:
    //!
    //! \brief playListUpgrade Upgrade play list when it changed outside
    //!
    void playListUpgrade();

  private:
    int     partIndex( int itemIndex );
    QString title( int itemIndex );
  };

#endif // CSVISUALPLAYLIST_H