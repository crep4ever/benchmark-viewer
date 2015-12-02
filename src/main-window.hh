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

#ifndef __MAIN_WINDOW_HH__
#define __MAIN_WINDOW_HH__

#include <QMainWindow>
#include <QDir>

class QTabWidget;
class QToolBar;
class QSplitter;

class CScene;
class CTimeLineView;
class CInfoWidget;

/*!
  \file main-window.hh
  \class CMainWindow
  \brief CMainWindow is the base class of the application.

  Class for the main window of the application.
*/
class CMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /// Constructor.
  CMainWindow(QWidget *parent = 0);

  /// Destructor.
  ~CMainWindow();

  void showMessage(const QString & p_message) const;

  void clear();

protected:
  /*!
    Saves settings before closing the application.
  */
  void closeEvent(QCloseEvent *event);

public slots:
  void open(const QString &filename);

private slots:
  void open();
  void toggleTimeLineView(bool);
  void toggleTreeView(bool);

  //application
  void preferences();
  void documentation();
  void reportBug();
  void about();

private:
  void readSettings(bool firstLaunch = false);
  void writeSettings();

  void createActions();
  void createMenus();
  void createToolBar();

private:
  // Widgets
  QToolBar *m_mainToolBar;
  QSplitter *m_mainWidget;
  CScene *m_scene;
  CTimeLineView *m_timelineView;
  CInfoWidget *m_treeView;

  // Application actions
  QAction *m_preferencesAct;
  QAction *m_documentationAct;
  QAction *m_bugsAct;
  QAction *m_aboutAct;
  QAction *m_exitAct;

  QAction *m_openAct;
  QAction *m_timeLineViewAct;
  QAction *m_treeViewAct;

  // Settings
  QString m_openPath;
  bool m_isInitialized;
};

#endif  // __MAIN_WINDOW_HH__
