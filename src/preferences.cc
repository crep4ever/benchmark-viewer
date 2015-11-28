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
#include "preferences.hh"

#include <QBoxLayout>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFontDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSettings>
#include <QSpinBox>
#include <QStackedWidget>

#include "main-window.hh"

#include <QDebug>

// Config Dialog

ConfigDialog::ConfigDialog(QWidget* parent)
: QDialog(parent)
, m_contentsWidget(0)
, m_pagesWidget(0)
{
  m_contentsWidget = new QListWidget(this);
  m_contentsWidget->setViewMode(QListView::IconMode);
  m_contentsWidget->setIconSize(QSize(62, 62));
  m_contentsWidget->setMovement(QListView::Static);
  m_contentsWidget->setSpacing(12);
  m_contentsWidget->setFixedWidth(110);

  m_pagesWidget = new QStackedWidget(this);
  m_pagesWidget->addWidget(new DisplayPage(this));

  QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close);
  connect(buttons, SIGNAL(rejected()), this, SLOT(close()));

  createIcons();
  m_contentsWidget->setCurrentRow(0);

  QBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(m_contentsWidget);
  horizontalLayout->addWidget(m_pagesWidget, 1);

  QBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addSpacing(12);
  mainLayout->addWidget(buttons);

  setLayout(mainLayout);
  setWindowTitle(tr("Preferences"));
  resize(600,600);
}

CMainWindow* ConfigDialog::parent() const
{
  CMainWindow *p = qobject_cast<CMainWindow*>(QDialog::parent());
  if (!p) qWarning() << tr("ConfigDialog::parent() invalid parent");
  return p;
}

void ConfigDialog::createIcons()
{
  QListWidgetItem *displayButton = new QListWidgetItem(m_contentsWidget);
  displayButton->setIcon(QIcon::fromTheme("preferences-desktop", QIcon(":/icons/tango/48x48/categories/preferences-desktop.png")));
  displayButton->setText(tr("Display"));
  displayButton->setTextAlignment(Qt::AlignHCenter);
  displayButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  connect(m_contentsWidget,
    SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
    this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!current)
  current = previous;

  m_pagesWidget->setCurrentIndex(m_contentsWidget->row(current));
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
  Q_UNUSED(event);
  for ( int i = 0 ; i < m_pagesWidget->count() ; ++i )
  {
    m_pagesWidget->widget(i)->close();
  }
}


// Page

Page::Page(QWidget *parent)
: QScrollArea(parent)
, m_content(new QWidget)
{
}

ConfigDialog * Page::parent() const
{
  ConfigDialog *p = qobject_cast<ConfigDialog*>(QScrollArea::parent());
  if (!p) qWarning() << tr("Page::parent() invalid parent");
  return p;
}

void Page::closeEvent(QCloseEvent *event)
{
  writeSettings();
  event->accept();
}

void Page::readSettings(){}

void Page::writeSettings(){}

void Page::setLayout(QLayout *layout)
{
  m_content->setLayout(layout);
  setWidget(m_content);
}


// Display Page

DisplayPage::DisplayPage(QWidget *parent) : Page(parent)
, m_statusBarCheckBox(0)
, m_toolBarCheckBox(0)
{
  QGroupBox *displayApplicationGroupBox = new QGroupBox(tr("Application"));
  m_statusBarCheckBox = new QCheckBox(tr("Status bar"));
  m_toolBarCheckBox = new QCheckBox(tr("Tool bar"));

  QVBoxLayout *displayApplicationLayout = new QVBoxLayout;
  displayApplicationLayout->addWidget(m_statusBarCheckBox);
  displayApplicationLayout->addWidget(m_toolBarCheckBox);
  displayApplicationGroupBox->setLayout(displayApplicationLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(displayApplicationGroupBox);
  mainLayout->addStretch(1);
  setLayout(mainLayout);

  readSettings();
}

void DisplayPage::readSettings()
{
  QSettings settings;
  settings.beginGroup("display");
  m_statusBarCheckBox->setChecked(settings.value("statusBar", true).toBool());
  m_toolBarCheckBox->setChecked(settings.value("toolBar", true).toBool());
  settings.endGroup();
}

void DisplayPage::writeSettings()
{
  QSettings settings;
  settings.beginGroup("display");
  settings.setValue("statusBar", m_statusBarCheckBox->isChecked());
  settings.setValue("toolBar", m_toolBarCheckBox->isChecked());
  settings.endGroup();
}
