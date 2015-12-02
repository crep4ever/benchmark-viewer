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

ConfigDialog::ConfigDialog(QWidget *p_parent) : QDialog(p_parent)
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
  m_pagesWidget->addWidget(new ParserPage(this));

  QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Close);
  connect(buttons, SIGNAL(rejected()), this, SLOT(close()));

  createIcons();
  m_contentsWidget->setCurrentRow(0);

  QBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(m_contentsWidget);
  horizontalLayout->addWidget(m_pagesWidget, 10);

  QBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addSpacing(12);
  mainLayout->addWidget(buttons);

  setLayout(mainLayout);
  setWindowTitle(tr("Preferences"));
  resize(600, 600);
}

CMainWindow* ConfigDialog::parent() const
{
  CMainWindow *p = qobject_cast<CMainWindow*>(QDialog::parent());
  if (!p) qWarning() << tr("ConfigDialog::parent() invalid parent");
  return p;
}

void ConfigDialog::createIcons()
{
  QListWidgetItem *parserButton = new QListWidgetItem(m_contentsWidget);
  parserButton->setIcon(QIcon::fromTheme("preferences-desktop", QIcon(":/icons/tango/48x48/categories/preferences-desktop.png")));
  parserButton->setText(tr("Parser"));
  parserButton->setTextAlignment(Qt::AlignHCenter);
  parserButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

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
  for (int i = 0; i < m_pagesWidget->count(); ++i)
  {
    m_pagesWidget->widget(i)->close();
  }
}


// Page

Page::Page(QWidget *p_parent) : QScrollArea(p_parent)
, m_content(new QWidget)
{
}

ConfigDialog * Page::parent() const
{
  ConfigDialog *p = qobject_cast<ConfigDialog*>(QScrollArea::parent());
  if (!p) qWarning() << tr("Page::parent() invalid parent");
  return p;
}

void Page::closeEvent(QCloseEvent *p_event)
{
  writeSettings();
  p_event->accept();
}

void Page::readSettings(){}

void Page::writeSettings(){}

void Page::setLayout(QLayout *layout)
{
  m_content->setLayout(layout);
  setWidget(m_content);
}


// Parser Page

ParserPage::ParserPage(QWidget *p_parent) : Page(p_parent)
, m_dateTimeFormat(new QLineEdit)
, m_tokensSeparator(new QLineEdit)
, m_actionStartLabel(new QLineEdit)
, m_actionStopLabel(new QLineEdit)
, m_actionStepLabel(new QLineEdit)
, m_tokenDateTimePosition(new QSpinBox)
, m_tokenLabelPosition(new QSpinBox)
, m_tokenActionPosition(new QSpinBox)
, m_tokenCommentPosition(new QSpinBox)
{
  m_dateTimeFormat->setMinimumWidth(180);

  QGroupBox *fileContentGroupBox = new QGroupBox(tr("File content format"));
  QFormLayout *fileContentLayout = new QFormLayout;
  fileContentLayout->addRow(tr("DateTime format"), m_dateTimeFormat);
  fileContentLayout->addRow(tr("Tokens separator"), m_tokensSeparator);
  fileContentLayout->addRow(tr("Start label"), m_actionStartLabel);
  fileContentLayout->addRow(tr("Stop label"), m_actionStopLabel);
  fileContentLayout->addRow(tr("Step label"), m_actionStepLabel);
  fileContentGroupBox->setLayout(fileContentLayout);

  QGroupBox *tokensPositionGroupBox = new QGroupBox(tr("Tokens position"));
  QFormLayout *tokensPositionLayout = new QFormLayout;
  tokensPositionLayout->addRow(tr("DateTime"), m_tokenDateTimePosition);
  tokensPositionLayout->addRow(tr("Label"), m_tokenLabelPosition);
  tokensPositionLayout->addRow(tr("Action"), m_tokenActionPosition);
  tokensPositionLayout->addRow(tr("Comment"), m_tokenCommentPosition);
  tokensPositionGroupBox->setLayout(tokensPositionLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(fileContentGroupBox);
  mainLayout->addWidget(tokensPositionGroupBox);
  mainLayout->addStretch(1);
  setLayout(mainLayout);

  readSettings();
}

void ParserPage::readSettings()
{
  QSettings settings;
  settings.beginGroup("parser");

  m_dateTimeFormat->setText(settings.value("dateTimeFormat", "yyyy-M-d hh:mm:ss.zzz").toString());
  m_tokensSeparator->setText(settings.value("tokensSeparator", ",").toString());
  m_actionStartLabel->setText(settings.value("actionStartLabel", "START").toString());
  m_actionStopLabel->setText(settings.value("actionStopLabel", "STOP").toString());
  m_actionStepLabel->setText(settings.value("actionStepLabel", "STEP").toString());

  m_tokenDateTimePosition->setValue(settings.value("tokenDateTimePosition", 0).toInt());
  m_tokenLabelPosition->setValue(settings.value("tokenLabelPosition", 1).toInt());
  m_tokenActionPosition->setValue(settings.value("tokenActionPosition", 2).toInt());
  m_tokenCommentPosition->setValue(settings.value("tokenCommentPosition", 4).toInt());

  settings.endGroup();
}

void ParserPage::writeSettings()
{
  QSettings settings;
  settings.beginGroup("parser");

  settings.setValue("dateTimeFormat", m_dateTimeFormat->text());
  settings.setValue("tokensSeparator", m_tokensSeparator->text());
  settings.setValue("actionStartLabel", m_actionStartLabel->text());
  settings.setValue("actionStopLabel", m_actionStopLabel->text());
  settings.setValue("actionStepLabel", m_actionStepLabel->text());

  settings.setValue("tokenDateTimePosition", m_tokenDateTimePosition->value());
  settings.setValue("tokenLabelPosition", m_tokenLabelPosition->value());
  settings.setValue("m_tokenActionPosition", m_tokenActionPosition->value());
  settings.setValue("m_tokenCommentPosition", m_tokenCommentPosition->value());

  settings.endGroup();
}
