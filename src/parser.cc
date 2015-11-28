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
#include "parser.hh"

#include <QObject>
#include <QFile>
#include <QElapsedTimer>
#include <QStringList>
#include <QDateTime>
#include <QDebug>

#include "node.hh"

CParser::CParser(const QString & p_fileName) :
m_nodes()
{
  if (!parse(p_fileName))
  {
    qWarning() << QObject::tr("Can't parse benchmark file ") << p_fileName;
  }
}

CParser::~CParser(){}

const QList<CNode*> & CParser::nodes() const
{
  return m_nodes;
}


bool CParser::parse(const QString & p_fileName)
{
  QElapsedTimer timer;
  timer.start();

  QFile file(p_fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qWarning() << "Can't open file " << p_fileName;
    return false;
  }

  const QString start = "START";
  const QString stop = "STOP";
  const QString step = "STEP";
  const QString separator = ",";
  const QString dateFormat = "yyyy-M-d hh:mm:ss.zzz";
  const int nbTokens = 4;

  QTextStream stream(&file);
  QString data = stream.readAll();
  file.close();

  if (data.isEmpty())
  {
    qWarning() << "Empty file " << p_fileName;
    return false;
  }

  QHash<QString, CNode*> incompleteNodes;
  QStringList lines = data.split("\n");

  m_nodes.reserve(lines.count() / 2);
  int level = 0;
  foreach(const QString & line, lines)
  {
    QString copy = line;
    const QStringList & tokens = copy.replace("\"", "").split(separator);
    if (tokens.count() < nbTokens)
    {
      continue;
    }

    const QString label = tokens[(int) TOKEN_LABEL];

    CNode *node = incompleteNodes[label];
    if (!node) // new node
    {
      node = new CNode();
      node->setLabel(label);
      incompleteNodes[label] = node;
    }

    const QString type = tokens[(int) TOKEN_ACTION];
    QString date = tokens[(int) TOKEN_DATE_TIME];
    date.chop(3);
    if (type == start)
    {
      node->setStart(QDateTime::fromString(date, dateFormat));
      node->setLevel(level++);
    }
    else if (type == stop)
    {
      --level;
      node->setStop(QDateTime::fromString(date, dateFormat));
    }
    else if (type == step)
    {
      node->addStep(QDateTime::fromString(date, dateFormat));
    }
    else
    {
      qDebug() << "invalid action type: " << type;
    }

    if (node->isValid() && node->duration() > 1) // skip nodes under 1ms
    {
      m_nodes << node; // save valid node
      incompleteNodes.remove(label);
      node = 0;
    }
  }

  qDebug() << "File" << p_fileName << "processed in" << timer.elapsed() << "ms";
  qDebug() << "Nodes: valid" << m_nodes.size() << "; invalid" << incompleteNodes.size();

  return true;
}
