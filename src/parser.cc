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

  QHash<QString, CNode*> incompleteNodes;
  int level = 0;
  while (!stream.atEnd())
  {
    QString line = stream.readLine();
    const QStringList & tokens = line.replace("\"", "").split(separator);

    if (tokens.count() < nbTokens)
    {
      continue;
    }

    const QString & label = tokens[(int) TOKEN_LABEL];

    CNode *node = incompleteNodes[label];
    if (!node) // new node
    {
      node = new CNode();
      node->setLabel(label);
      node->setColor(pickColor(qHash(label)));
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
    else if (type == stop && level > 0)
    {
      --level;
      node->setStop(QDateTime::fromString(date, dateFormat));
    }
    else if (type == step)
    {
      node->addStep(QDateTime::fromString(date, dateFormat));
    }

    if (node->isValid() && node->duration() > 1) // skip nodes under 1ms
    {
      m_nodes << node; // save valid node
      incompleteNodes.remove(label);
      node = 0;
    }
  }
  file.close();

  qDebug() << "File" << p_fileName << "processed in" << timer.elapsed() << "ms";
  qDebug() << "Nodes: valid" << m_nodes.size() << "; invalid" << incompleteNodes.size();

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
    const QList<CNode *> & nodes = m_levels[level];
    foreach (CNode *node, nodes)
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
