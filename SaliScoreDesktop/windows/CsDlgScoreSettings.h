/*
Project "SaliScore Score music edit, view and tutorial programm"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Dialog for setup and edit hole composition settings
*/
#ifndef CSDLGSCORESETTINGS_H
#define CSDLGSCORESETTINGS_H

#include "score/CsComposition.h"

#include <QDialog>
#include <QLineEdit>

class CsDlgScoreSettings : public QDialog
  {
    Q_OBJECT

    CsComposition &mComposition; //!< Edited composition

    QLineEdit     *mLineOffset;
    QLineEdit     *mNumerator;
    QLineEdit     *mDenominator;
  public:
    CsDlgScoreSettings( CsComposition &comp, QWidget *parent );

    // QDialog interface
  public slots:
    virtual void accept() override;
  };

#endif // CSDLGSCORESETTINGS_H
