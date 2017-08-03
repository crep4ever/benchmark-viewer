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
#include "node.hh"

#include <QDebug>
#include <utility>
#include "utils.hh"

CStep::CStep(CNode *p_parent, QString  p_label, const QDateTime & p_dateTime) :
m_parent(p_parent),
m_label(std::move(p_label)),
m_duration(0)
{
  m_duration = p_dateTime.toMSecsSinceEpoch() - p_parent->startMs();
}

CStep::~CStep()= default;

CNode *CStep::parent() const
{
  return m_parent;
}

qint64 CStep::duration() const
{
  return m_duration;
}

const QString & CStep::label() const
{
    return m_label;
}

void CStep::setLabel(const QString & p_label)
{
  m_label = p_label;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

CNode::CNode() :
m_label(),
m_color(),
m_category(),
m_start(),
m_startMs(0),
m_stop(),
m_stopMs(0),
m_duration(0),
m_steps(),
m_level(0),
m_parent(nullptr),
m_children()
{
}

CNode::~CNode()
{
  foreach (CStep *step, m_steps)
  {
    delete step;
  }
  m_steps.clear();
  m_children.clear();
}

const QDateTime & CNode::start() const
{
  return m_start;
}

void CNode::setStart(const QDateTime & p_dateTime)
{
  m_start = p_dateTime;
  m_startMs = p_dateTime.toMSecsSinceEpoch();
  if (isValid())
  {
    m_duration = m_stopMs - m_startMs;
  }
}

qint64 CNode::startMs() const
{
  return m_startMs;
}

const QDateTime & CNode::stop() const
{
  return m_stop;
}

void CNode::setStop(const QDateTime & p_dateTime)
{
  m_stop = p_dateTime;
  m_stopMs = p_dateTime.toMSecsSinceEpoch();
  if (isValid())
  {
    m_duration = m_stopMs - m_startMs;
  }
}

qint64 CNode::stopMs() const
{
  return m_stopMs;
}


const QList<CStep*> & CNode::steps() const
{
  return m_steps;
}

void CNode::setSteps(const QList<CStep*> & p_steps)
{
  m_steps = p_steps;
}

void CNode::addStep(const QString & p_label, const QDateTime & p_dateTime)
{
    m_steps << new CStep(this, p_label, p_dateTime);
}

void CNode::addStep(CStep *p_step)
{
    m_steps << p_step;
}

const QString & CNode::label() const
{
  return m_label;
}

void CNode::setLabel(const QString & p_label)
{
  m_label = p_label;
}

const QString & CNode::category() const
{
  return m_category;
}

void CNode::setCategory(const QString & p_category)
{
  m_category = p_category;
}

int CNode::level() const
{
  return m_level;
}

void CNode::setLevel(const int p_level)
{
  m_level = p_level;
}

CNode *CNode::parent() const
{
  return m_parent;
}

void CNode::setParent(CNode *p_parent)
{
  m_parent = p_parent;
}

const QList<CNode *> & CNode::children() const
{
  return m_children;
}

void CNode::addChild(CNode *p_node)
{
  p_node->setParent(this);
  m_children << p_node;
}

bool CNode::isValid() const
{
  return !m_start.isNull() && !m_stop.isNull();
}

qint64 CNode::duration() const
{
  return m_duration;
}

const QColor & CNode::color() const
{
  return m_color;
}

void CNode::setColor(const QColor & p_color)
{
  m_color = p_color;
}

QString CNode::toString() const
{
  return QString("%1 (%2)")
  .arg(label())
  .arg(::mSecsToString(duration()));
}
