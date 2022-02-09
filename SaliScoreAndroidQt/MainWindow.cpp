#include "MainWindow.h"

#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  {
//  QVBoxLayout *lay = new QVBoxLayout();
//  lay->addWidget( new QLabel(QStringLiteral("Primer. Hello.") ) );
//  setLayout( lay );
  setCentralWidget( new QLabel(QStringLiteral("Primer. Hello") )  );
  }

MainWindow::~MainWindow()
  {
  }

