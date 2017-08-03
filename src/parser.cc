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
#include <QtGlobal>
#include <QSettings>
#include <QDebug>

#include "node.hh"
#include "tango-colors.hh"

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

int CParser::computeMaxTokenPosition() const
{
  QSettings settings;
  settings.beginGroup("parser");
  const int tokenDateTimePosition = settings.value("tokenDateTimePosition", 0).toInt();
  const int tokenLabelPosition = settings.value("tokenLabelPosition", 1).toInt();
  const int tokenActionPosition = settings.value("tokenActionPosition", 2).toInt();
  const int tokenCommentPosition = settings.value("tokenCommentPosition", 3).toInt();
  settings.endGroup();

  int max = 0;
  max = qMax(max, tokenDateTimePosition);
  max = qMax(max, tokenLabelPosition);
  max = qMax(max, tokenActionPosition);
  max = qMax(max, tokenCommentPosition);
  return max;
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

  QSettings settings;
  settings.beginGroup("parser");
  const QString start = settings.value("actionStartLabel", "START").toString();
  const QString stop = settings.value("actionStopLabel", "STOP").toString();
  const QString step = settings.value("actionStepLabel", "STEP").toString();
  const QString separator = settings.value("tokensSeparator", ",").toString();
  const QString dateTimeFormat = settings.value("dateTimeFormat", "yyyy-M-d hh:mm:ss.zzz").toString();

  const int tokenDateTimePosition = settings.value("tokenDateTimePosition", 0).toInt();
  const int tokenLabelPosition = settings.value("tokenLabelPosition", 1).toInt();
  const int tokenActionPosition = settings.value("tokenActionPosition", 2).toInt();
  const int tokenCommentPosition = settings.value("tokenCommentPosition", 3).toInt();
  settings.endGroup();

  bool chopMicrosec = false;
  QString dateFormat = dateTimeFormat;
  if (dateTimeFormat.endsWith("zzzzzz"))
  {
    dateFormat = dateFormat.replace("zzzzzz", "zzz");
    chopMicrosec = true;
  }

  const int minNumberOfTokens = computeMaxTokenPosition() + 1;

  QTextStream stream(&file);

  QHash<QString, CNode*> incompleteNodes;
  int level = 0;
  int skips = 0;
  while (!stream.atEnd())
  {
    QString line = stream.readLine();
    const QStringList & tokens = line.replace(R"(")", "").split(separator);

    if (tokens.count() < minNumberOfTokens)
    {
      continue;
    }

    const QString & label = tokens[tokenLabelPosition];

    CNode *node = incompleteNodes[label];
    if (!node) // new node
    {
      node = new CNode();
      node->setLabel(label);
      node->setColor(pickColor(qHash(label)));
      incompleteNodes[label] = node;
    }

    const QString type = tokens[tokenActionPosition];
    QString date = tokens[tokenDateTimePosition];
    if (chopMicrosec)
    {
      date.chop(3);
    }

    if (type == start)
    {
      node->setStart(QDateTime::fromString(date, dateFormat));
      node->setLevel(level++);
    }
    else if (type == stop && level > 0)
    {
      --level;
      node->setStop(QDateTime::fromString(date, dateFormat));
    }
    else if (type == step)
    {
      // TODO: warning, crash-prone if invalid src file
      node->addStep(tokens[tokenCommentPosition], QDateTime::fromString(date, dateFormat));
    }

    if (node->isValid())
    {
      if (node->duration() < 1) // skip nodes under 1ms
      {
        ++skips;
        delete node;
      }
      else
      {
        m_nodes << node;
      }

      incompleteNodes.remove(label);
      node = 0;
    }
  }
  file.close();

  qDebug() << "File" << p_fileName << "processed in" << timer.elapsed() << "ms";
  qDebug() << "Nodes: valid" << m_nodes.size() << "; skipped" << skips << "; invalid" << incompleteNodes.size();

  // delete invalid nodes
  foreach (CNode * node, incompleteNodes)
  {
    delete node;
  }

  computeTreeModel();

  return true;
}

int CParser::computeDepth() const
{
  int depth = 0;
  foreach(CNode *node, m_nodes)
  {
    depth = qMax(node->level(), depth);
  }
  return depth;
}

void CParser::computeTreeModel()
{
  QElapsedTimer timer;
  timer.start();

  const int depth = computeDepth();
  QVector< QList<CNode *> > m_levels(depth + 1);
  foreach(CNode *node, m_nodes)
  {
    m_levels[node->level()] << node;
  }

  for (int level = 0; level < depth; ++level)
  {
    const QList<CNode *> & levelNodes = m_levels[level];
    foreach (CNode *node, levelNodes)
    {
      const qint64 start = node->startMs();
      const qint64 stop  = node->stopMs();

      const QList<CNode *> & candidates = m_levels[level + 1];
      foreach (CNode *candidate, candidates)
      {
        const qint64 candidateStart = candidate->startMs();
        const qint64 candidateStop  = candidate->stopMs();

        if (candidateStart >= start && candidateStop <= stop)
        {
          node->addChild(candidate);
        }
      }
    }
  }

  qDebug() << "Compute tree model in" << timer.elapsed() << "ms";
}
