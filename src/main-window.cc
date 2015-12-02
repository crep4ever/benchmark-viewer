// Copyright (C) 2015, Romain Goffe <romain.goffe@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//******************************************************************************
#include "main-window.hh"

#include <QApplication>
#include <QAction>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>
#include <QDebug>
#include <QStringList>
#include <QSplitter>
#include <QDockWidget>

#include "preferences.hh"
#include "parser.hh"
#include "scene.hh"
#include "timeline-view.hh"
#include "overlay-widget.hh"
#include "info-widget.hh"
#include "utils.hh"
#include "config.hh"

CMainWindow::CMainWindow(QWidget *p_parent) : QMainWindow(p_parent)
, m_mainToolBar(0)
, m_mainWidget(0)
, m_scene(0)
, m_timelineView(0)
, m_treeView(0)
, m_preferencesAct(0)
, m_documentationAct(0)
, m_bugsAct(0)
, m_aboutAct(0)
, m_exitAct(0)
, m_openAct(0)
, m_timeLineViewAct(0)
, m_treeViewAct(0)
, m_openPath(QDir::homePath())
, m_isInitialized(false)
{
  setWindowTitle("Benchmark viewer");
  setWindowIcon(QIcon(":/icons/benchmark-viewer/src/benchmark-viewer.svg"));

  createActions();
  createMenus();
  createToolBar();

  // place elements into the main window
  m_mainWidget = new QSplitter;
  setCentralWidget(m_mainWidget);

  readSettings(true);
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::readSettings(bool p_firstLaunch)
{
  QSettings settings;
  settings.beginGroup("general");
  if (p_firstLaunch)
  {
    resize(settings.value("size", QSize(800,600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    if (settings.value("maximized", isMaximized()).toBool())
    {
      showMaximized();
    }
  }
  m_openPath = settings.value("openPath", QDir::homePath()).toString();
  settings.endGroup();
}

void CMainWindow::writeSettings()
{
  QSettings settings;
  settings.beginGroup( "general" );
  settings.setValue( "maximized", isMaximized() );
  if (!isMaximized())
  {
    settings.setValue( "pos", pos() );
    settings.setValue( "size", size() );
  }
  settings.setValue( "openPath", m_openPath );
  settings.endGroup();
}

void CMainWindow::createActions()
{
  m_openAct = new QAction(tr("&Open..."), this);
  m_openAct->setIcon(QIcon::fromTheme("document-open", QIcon(":/icons/tango/32x32/actions/document-open.png")));
  m_openAct->setShortcut(QKeySequence::Open);
  m_openAct->setStatusTip(tr("Open a data file"));
  connect(m_openAct, SIGNAL(triggered()), this, SLOT(open()));

  m_documentationAct = new QAction(tr("Online &Documentation"), this);
  m_documentationAct->setShortcut(QKeySequence::HelpContents);
  m_documentationAct->setIcon(QIcon::fromTheme("help-contents", QIcon(":/icons/tango/32x32/actions/help-contents.png")));
  m_documentationAct->setStatusTip(tr("Download documentation pdf file "));
  connect(m_documentationAct, SIGNAL(triggered()), this, SLOT(documentation()));

  m_bugsAct = new QAction(tr("&Report a bug"), this);
  m_bugsAct->setStatusTip(tr("Report a bug about this application"));
  connect(m_bugsAct, SIGNAL(triggered()), this, SLOT(reportBug()));

  m_aboutAct = new QAction(tr("&About"), this);
  m_aboutAct->setIcon(QIcon::fromTheme("help-about", QIcon(":/icons/tango/32x32/actions/help-about.png")));
  m_aboutAct->setStatusTip(tr("About this application"));
  m_aboutAct->setMenuRole(QAction::AboutRole);
  connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  m_exitAct = new QAction(tr("&Quit"), this);
  m_exitAct->setIcon(QIcon::fromTheme("application-exit", QIcon(":/icons/tango/32x32/application-exit.png")));
  m_exitAct->setShortcut(QKeySequence::Quit);
  m_exitAct->setStatusTip(tr("Quit the program"));
  m_exitAct->setMenuRole(QAction::QuitRole);
  connect(m_exitAct, SIGNAL(triggered()), this, SLOT(close()));

  m_preferencesAct = new QAction(tr("&Preferences"), this);
  m_preferencesAct->setIcon(QIcon::fromTheme("document-properties", QIcon(":/icons/tango/32x32/document-properties.png")));
  m_preferencesAct->setStatusTip(tr("Configure the application"));
  m_preferencesAct->setMenuRole(QAction::PreferencesRole);
  connect(m_preferencesAct, SIGNAL(triggered()), SLOT(preferences()));

  m_timeLineViewAct = new QAction(tr("&Timeline"), this);
  m_timeLineViewAct->setIcon(QIcon(":/icons/benchmark-viewer/48x48/timeline.png"));
  m_timeLineViewAct->setStatusTip(tr("Display the session timeline"));
  m_timeLineViewAct->setCheckable(true);
  m_timeLineViewAct->setChecked(true);
  connect(m_timeLineViewAct, SIGNAL(toggled(bool)), SLOT(toggleTimeLineView(bool)));

  m_treeViewAct = new QAction(tr("&Info panel"), this);
  m_treeViewAct->setIcon(QIcon(":/icons/benchmark-viewer/48x48/callgraph.png"));
  m_treeViewAct->setStatusTip(tr("Display detailed information about current action"));
  m_treeViewAct->setCheckable(true);
  m_treeViewAct->setChecked(true);
  connect(m_treeViewAct, SIGNAL(toggled(bool)), SLOT(toggleTreeView(bool)));
}


void CMainWindow::closeEvent(QCloseEvent *p_event)
{
  writeSettings();
  p_event->accept();
}

void CMainWindow::createMenus()
{
  menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);

  QMenu *fileMenu = menuBar()->addMenu(tr("&Benchmark"));
  fileMenu->addAction(m_openAct);
  fileMenu->addSeparator();
  fileMenu->addAction(m_preferencesAct);
  fileMenu->addSeparator();
  fileMenu->addAction(m_exitAct);

  QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(m_treeViewAct);

  QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(m_documentationAct);
  helpMenu->addAction(m_bugsAct);
  helpMenu->addAction(m_aboutAct);
}

void CMainWindow::createToolBar()
{
  m_mainToolBar = new QToolBar(tr("Benchmark"), this);
  m_mainToolBar->setMovable(false);
  m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_mainToolBar->addAction(m_openAct);
  m_mainToolBar->addSeparator();

  addToolBar(m_mainToolBar);
  setUnifiedTitleAndToolBarOnMac(true);
}

void CMainWindow::preferences()
{
  ConfigDialog dialog(this);
  dialog.exec();
  readSettings();
}

void CMainWindow::documentation()
{
  QDesktopServices::openUrl(QUrl("http://www.github.com/crep4ever/benchmark-viewer"));
}

void CMainWindow::reportBug()
{
  QDesktopServices::openUrl(QUrl("http://www.github.com/crep4ever/benchmark-viewer/issues"));
}

void CMainWindow::about()
{
  QString title(tr("About Benchmark Viewer"));
  QString version = QCoreApplication::applicationVersion();

  QString description(tr("This program displays benchmark results as interactive graphs."));

  QStringList authorsList = QStringList() << "R. Goffe";
  QString authors = authorsList.join(", ");

  QMessageBox::about(this, title, tr("<p>%1</p>"
  "<p><b>Version:</b> %2</p>"
  "<p><b>Authors:</b> %3</p>"
  "<p><b>Qt:</b> %4</p>")
  .arg(description)
  .arg(version)
  .arg(authors)
  .arg(QT_VERSION_STR));
}

void CMainWindow::toggleTimeLineView(bool p_visible)
{
  if (m_mainWidget && m_mainWidget->widget(0))
  {
    m_mainWidget->widget(0)->setVisible(p_visible);
  }
}

void CMainWindow::toggleTreeView(bool p_visible)
{
  if (m_mainWidget && m_mainWidget->widget(1))
  {
    m_mainWidget->widget(1)->setVisible(p_visible);
  }
}

void CMainWindow::open(const QString & filename)
{
  if (m_isInitialized)
  {
      clear();
  }

  QFileInfo fi(filename);
  m_openPath = fi.absolutePath();
  writeSettings(); // updates openPath

  showMessage(tr("Opening %1").arg(filename));
  CParser parser(filename);

  showMessage(tr("Building timeline"));
  m_scene = new CScene(parser.nodes());

  QString sessionInfo = QString("%1\n%2\n%3 (%4 actions)")
    .arg(QFileInfo(filename).absoluteFilePath())
    .arg(m_scene->first().date().toString(Qt::SystemLocaleLongDate))
    .arg(mSecsToString(m_scene->duration()))
    .arg(parser.nodes().size());

  showMessage(tr("Initializing views"));
  m_timelineView = new CTimeLineView(m_scene);
  m_timelineView->sessionInfo()->setText(sessionInfo);
  m_timelineView->setVisible(m_timeLineViewAct->isChecked());
  m_mainWidget->addWidget(m_timelineView);

  m_treeView = new CInfoWidget;
  connect(m_timelineView, SIGNAL(currentNodeChanged(CNode*)),
          m_treeView, SLOT(setNode(CNode*)));
  m_treeView->setVisible(m_treeViewAct->isChecked());
  m_mainWidget->addWidget(m_treeView);

  showMessage(tr("Ready"));
  m_isInitialized = true;
}

void CMainWindow::open()
{
  QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                        tr("Open data file"),
                                                        m_openPath,
                                                        tr("Text files (*.txt *.csv)"));
  foreach (const QString & filename, filenames)
  {
    if (!filename.isEmpty())
    {
      open(filename);
    }
  }
}

void CMainWindow::showMessage(const QString & p_message) const
{
  statusBar()->showMessage(p_message);
}

void CMainWindow::clear()
{
  delete m_timelineView;
  delete m_treeView;
  delete m_scene;

  m_isInitialized = false;
}
