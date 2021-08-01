#ifndef CSWINSCORE_H
#define CSWINSCORE_H

#include "CsConfig.h"
#include "score/CsComposition.h"
#include "CsWinPage.h"
#include "CsWinTrain.h"
#include "CsWinEditor.h"
#include "CsWinKaraoke.h"

class CsWinScore : public CsWinPage
  {
    Q_OBJECT

    CsComposition mComposition;

    CsWinEditor  *mWinEditor;
    CsWinTrain   *mWinTrain;
    CsWinKaraoke *mWinKaraoke;
  public:
    explicit CsWinScore(const QString filePath, CsComposition &src, QWidget *parent = nullptr);


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

    void cmViewEditor();
    void cmViewTrain();
    void cmViewKaraoke();

  signals:


    // CsWinPage interface
  public:

    //!
    //! \brief activate Called when window activated to ajust view
    //!
    virtual void    activate() override;

    //!
    //! \brief extension Returns extension of files of editor
    //! \return          Extension of files of editor
    //!
    virtual QString extension() const override;

    //!
    //! \brief isDirty Returns if composition edited
    //! \return        true if composition edited
    //!
    virtual bool    isDirty() const override { return mComposition.isDirty(); }

    //!
    //! \brief cmFileSave Saves file into path
    //! \param path       Path to file
    //!
    virtual void    cmFileSave( const QString path ) override;

    virtual void    cmFilePublic() override;
    virtual void    cmFileExport() override;
    virtual void    cmFilePrint() override;

  };

#endif // CSWINSCORE_H
