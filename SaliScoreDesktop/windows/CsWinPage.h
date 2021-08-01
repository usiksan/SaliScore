#ifndef CSWINPAGE_H
#define CSWINPAGE_H

#include <QStackedWidget>

class CsWinPage : public QStackedWidget
  {
    Q_OBJECT

    QString       mPath;
  public:
    explicit CsWinPage(const QString path, QWidget *parent = nullptr);

    //!
    //! \brief path Returns current file path
    //! \return     Current file path
    //!
    QString path() const { return mPath; }

    //!
    //! \brief setPath Setup new path for composition
    //! \param thePath New path for composition
    //!
    void    setPath( const QString thePath );

    //!
    //! \brief name Returns current file name (without path)
    //! \return     Current file name
    //!
    QString name() const;

    //!
    //! \brief extension Returns extension of files of editor
    //! \return          Extension of files of editor
    //!
    virtual QString extension() const = 0;

    //!
    //! \brief isDirty Returns if composition edited
    //! \return        true if composition edited
    //!
    virtual bool    isDirty() const = 0;

    //Menu File
    //!
    //! \brief cmFileSave Saves file into path
    //! \param path       Path to file
    //!
    virtual void    cmFileSave( const QString path ) = 0;

    virtual void    cmFilePublic() = 0;
    virtual void    cmFileExport() = 0;
    virtual void    cmFilePrint() = 0;

  signals:

  };

#endif // CSWINPAGE_H
