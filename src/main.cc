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

/*!
* \file main.cc
* \mainpage Benchmark-Viewer Documentation
*
*/

#include <QApplication>

#include <QTranslator>
#include <QDate>
#include <QLocale>
#include <QDir>
#include <QTextStream>
#include <QMetaType>
#include <QDebug>

#include "config.hh"
#include "main-window.hh"

void printUsage()
{
  QTextStream out(stdout);
  out << endl;

  out << "********************************************************" << endl;
  out << "Description: Display benchmark results as an interactive graph" << endl;
  out << endl;

  out << "Usage: " << QCoreApplication::applicationName() << " [OPTIONS] FILE" << endl;
  out << endl;

  out << "FILE: benchmark file" << endl;
  out << endl;

  out << "OPTIONS:" << endl;
  out << "\t-v, --version\t" << "print version and exit." << endl;
  out << "\t-h, --help\t\t"  << "print this help and exit" << endl;
  out << endl;
  out << "********************************************************" << endl;
  out << endl;
}

void printVersion()
{
  QTextStream out(stdout);
  out << QCoreApplication::applicationName()
  << " " << QCoreApplication::applicationVersion()
  << endl;
}


/// Main routine of the application
int main(int argc, char *argv[])
{
  QApplication application(argc, argv);

  QApplication::setOrganizationName("ViTechnology");
  QApplication::setOrganizationDomain("vitechnology.com");
  QApplication::setApplicationName(PROJECT_APPLICATION_NAME);
  QApplication::setApplicationVersion(PROJECT_VERSION);

  // Load the application ressources (icons, ...)
  Q_INIT_RESOURCE(benchmark);

  // Check for a standard theme icon. If it does not exist, for
  // instance on MacOSX or Windows, fallback to one of the theme
  // provided in the ressource file.
  if (!QIcon::hasThemeIcon("document-open"))
  {
    QIcon::setThemeName("tango");
  }

  // Parse command line arguments
  QStringList arguments = QApplication::arguments();

  if (arguments.contains("-h") || arguments.contains("--help"))
  {
    printUsage();
    return 0;
  }
  else if (arguments.contains("--version") || arguments.contains("-v"))
  {
    printVersion();
    return 0;
  }

  // Localization
  QDir translationDirectory;
  QString translationFilename = QString("benchmark-viewer_%1.qm").arg(QLocale::system().name().split('_').first());
  QString directory;

  translationDirectory = QDir(PROJECT_DATA_PATH);
  if (translationDirectory.exists())
  {
    directory = translationDirectory.absoluteFilePath("lang");
  }
  else
  {
    directory = QDir::current().absoluteFilePath("lang");
  }

  QTranslator translator;
  translator.load(translationFilename, directory);
  application.installTranslator(&translator);

  CMainWindow mainWindow;
  mainWindow.show();

  foreach (const QString & arg, arguments)
  {
    if (QFile(arg).exists() && CMainWindow::isFilenameSupported(arg))
    {
      mainWindow.open(arg);
    }
  }

  return application.exec();
}
