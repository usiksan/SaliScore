#ifndef CSWINEDITOR_H
#define CSWINEDITOR_H

#include <QWidget>
#include <QAbstractScrollArea>

class CsWinEditor : public QAbstractScrollArea
  {
    Q_OBJECT
  public:
    explicit CsWinEditor(QWidget *parent = nullptr);

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

#endif // CSWINEDITOR_H
