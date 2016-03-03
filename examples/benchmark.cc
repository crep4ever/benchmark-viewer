// Copyright (C) 2016, Romain Goffe <romain.goffe@gmail.com>
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
#include "benchmark.hh"
#include <cstdio>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <cassert>

#define BENCHMARK_STR_BUFFER_SIZE 1024

FILE * Benchmark::_file = 0;
const std::string Benchmark::_separator  = ",";
const std::string Benchmark::_startLabel = "START";
const std::string Benchmark::_stopLabel  = "STOP";
const std::string Benchmark::_stepLabel  = "STEP";

void Benchmark::initSession(const std::string & p_filePath)
{
    Benchmark::_file = fopen(p_filePath.c_str(), "w+");
    assert(Benchmark::_file);
}

Benchmark::Benchmark(const std::string & p_label) : m_label(p_label)
{
    writeLine(_startLabel);
}

Benchmark::~Benchmark()
{
    writeLine(_stopLabel);
}

void Benchmark::addStep(const std::string & p_comment)
{
    writeLine(_stepLabel, p_comment);
}

void Benchmark::writeLine(const std::string & p_action,
                          const std::string & p_comment)
{
    if (_file)
    {
        char buffer[BENCHMARK_STR_BUFFER_SIZE];
        sprintf(buffer, "%s%s%s%s%s%s%s", currentDateTime().c_str(),
                _separator.c_str(),
                m_label.c_str(),
                _separator.c_str(),
                p_action.c_str(),
                _separator.c_str(),
                p_comment.c_str());

        fprintf(_file, "%s\n", buffer);
    }
}

const std::string Benchmark::currentDateTime() const
{
  timeval curTime;
  gettimeofday(&curTime, NULL);
  const int ms = curTime.tv_usec / 1000;

  char buffer [24];
  strftime(buffer, 24, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

  char currentTime[28];
  sprintf(currentTime, "%s.%03d", buffer, ms);

  return currentTime;
}


