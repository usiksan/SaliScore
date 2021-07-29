#ifndef CSWINSCORE_H
#define CSWINSCORE_H

#include "CsConfig.h"
#include "score/CsComposition.h"

#include <QWidget>

class CsWinScore : public QWidget
  {
    Q_OBJECT

    QString       mPath;
    CsComposition mComposition;
    bool          mDirty;
  public:
    explicit CsWinScore(const QString path, CsComposition &src, QWidget *parent = nullptr);

    //!
    //! \brief path Returns current file path
    //! \return     Current file path
    //!
    QString path() const { return mPath; }

    //!
    //! \brief name Returns current file name (without path)
    //! \return     Current file name
    //!
    QString name() const;

    //!
    //! \brief isDirty Returns if composition edited
    //! \return        true if composition edited
    //!
    bool    isDirty() const { return mDirty; }

    //!
    //! \brief cmFileSave Saves file into path
    //! \param path       Path to file
    //!
    void    cmFileSave( const QString path );

    //Menu File
    void cmFilePublic();
    void cmFileExport();
    void cmFilePrint();

    //Menu Edit
    virtual void cmEditUndo();
    virtual void cmEditRedo();
    virtual void cmEditCut();
    virtual void cmEditCopy();
    virtual void cmEditPaste();
    virtual void cmEditDelete();
//    virtual

//    static QActionPtr  actionEditCopy;
//    static QActionPtr  actionEditPaste;
//    static QActionPtr  actionEditDelete;
//    static QActionPtr  actionEditSelectAll;
//    static QActionPtr  actionEditUnSelect;

  signals:

  };

#endif // CSWINSCORE_H