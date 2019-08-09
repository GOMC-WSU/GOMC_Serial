/*******************************************************************************
GPU OPTIMIZED MONTE CARLO (GOMC) 2.40
Copyright (C) 2018  GOMC Group
A copy of the GNU General Public License can be found in the COPYRIGHT.txt
along with this program, also can be found at <http://www.gnu.org/licenses/>.
********************************************************************************/
#ifndef CLOCK_H
#define CLOCK_H

//clock() function; CLOCKS_PER_SEC constant
#include <iomanip> //
#include <sstream>      // std::ostringstream
#include <fstream>
#include <time.h>
#include "BasicTypes.h"             //uint, ulong
#include <iostream> //for cout
#include <cstdio>
#if defined(__linux__) || defined(__APPLE__)
#include <sys/time.h> //for timing
#elif (_WIN32) || (__CYGWIN__)
#include <time.h>
#endif

struct Clock {
  Clock(): stepsPerOut(0), prevStep(0), lastStep(0), lastTime(0.0) {}
  void Init(const ulong steps, const ulong totSt, const ulong startStep)
  {
    stepsPerOut = steps;
    prevStep = startStep;
#if defined(__linux__) || defined(__APPLE__)
    gettimeofday(&tv, &tz);
    strt = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
#elif (_WIN32) || (__CYGWIN__)
    strt = clock();
#endif
    lastTime = strt;
    lastStep = totSt - 1;
  }
  void CheckTime(const ulong step);
  void CheckTime(const ulong step, std::ofstream * consoleOut);
  void SetStart();
  void SetStop();
  double GetTimDiff();
  void CompletionTime(uint &day, uint &hr, uint &min);

private:

#if defined(__linux__) || defined(__APPLE__)
  struct timeval tv;
  struct timezone tz;
  double strt, stop, lastTime;
#elif (_WIN32) || (__CYGWIN__)
  clock_t strt, stop, lastTime;
#endif
  ulong stepsPerOut, prevStep, lastStep;
};

inline void Clock::CheckTime(const ulong step)
{
  ulong stepDelta = step - prevStep;
  double speed = 0.0;
  if (stepDelta == stepsPerOut && step != lastStep) {
#if defined(__linux__) || defined(__APPLE__)
    gettimeofday(&tv, &tz);
    double currTime = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
    speed = stepDelta / (currTime - lastTime);
#elif (_WIN32) || (__CYGWIN__)
    clock_t currTime = clock();
    speed = stepDelta / (((double)currTime - lastTime) / CLOCKS_PER_SEC);
#endif
    uint day, hr, min;
    prevStep = step;
    lastTime = currTime;
    CompletionTime(day, hr, min);
    printf("Steps/sec: %7.3f, Simulation ends in: %3d d: %3d h: %3d m \n\n",
           speed, day, hr, min);

  } else if (step == lastStep) {
#if defined(__linux__) || defined(__APPLE__)
    gettimeofday(&tv, &tz);
    stop = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
    std::cout << "Simulation Time (total): " << (stop - strt)
              << " sec." << std::endl;
#elif (_WIN32) || (__CYGWIN__)
    stop = clock();
    std::cout << "Simulation Time (total): "
              << (((double)stop - strt) / CLOCKS_PER_SEC)
              << " sec." << std::endl;
#endif

  }
}

inline void Clock::CheckTime(const ulong step, std::ofstream * consoleOut)
{
  std::ostringstream default_format;

  ulong stepDelta = step - prevStep;
  double speed = 0.0;
  if (stepDelta == stepsPerOut && step != lastStep) {
#if defined(__linux__) || defined(__APPLE__)
    gettimeofday(&tv, &tz);
    double currTime = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
    speed = stepDelta / (currTime - lastTime);
#elif (_WIN32) || (__CYGWIN__)
    clock_t currTime = clock();
    speed = stepDelta / (((double)currTime - lastTime) / CLOCKS_PER_SEC);
#endif
    uint day, hr, min;
    prevStep = step;
    lastTime = currTime;
    CompletionTime(day, hr, min);
    std::cout << "\r";
   // printf("Steps/sec: %7.3f, Simulation ends in: %3d d: %3d h: %3d m ",
       //  speed, day, hr, min);

    std::cout << "Steps/sec: ";
    std::cout << std::fixed << std::setw(7) << std::setprecision(3) << speed;
    std::cout.copyfmt(default_format);
    std::cout << ", Simulation ends in: ";
    std::cout << std::setw(3) << day;
    std::cout.copyfmt(default_format);
    std::cout << " d: ";
    std::cout << std::setw(3) << hr;
    std::cout.copyfmt(default_format);
    std::cout << " h: ";  
    std::cout << std::setw(3) << min;
    std::cout.copyfmt(default_format);
    std::cout << " m \n" << std::endl; 


    *consoleOut << "Steps/sec: ";
    *consoleOut << std::fixed << std::setw(7) << std::setprecision(3) << speed;
    consoleOut->copyfmt(default_format);
    *consoleOut << ", Simulation ends in: ";
    *consoleOut << std::setw(3) << day;
    consoleOut->copyfmt(default_format);
    *consoleOut << " d: ";
    *consoleOut << std::setw(3) << hr;
    consoleOut->copyfmt(default_format);
    *consoleOut << " h: ";  
    *consoleOut << std::setw(3) << min;
    consoleOut->copyfmt(default_format);
    *consoleOut << " m \n" << std::endl;      

  } else if (step == lastStep) {
#if defined(__linux__) || defined(__APPLE__)
    gettimeofday(&tv, &tz);
    stop = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
    *consoleOut << "Simulation Time (total): " << (stop - strt)
              << " sec." << std::endl;
#elif (_WIN32) || (__CYGWIN__)
    stop = clock();
    *consoleOut << "Simulation Time (total): "
              << (((double)stop - strt) / CLOCKS_PER_SEC)
              << " sec." << std::endl;
#endif

  }
}

inline void Clock::SetStart()
{
#if defined(__linux__) || defined(__APPLE__)
  gettimeofday(&tv, &tz);
  strt = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
#elif (_WIN32) || (__CYGWIN__)
  strt = clock();
#endif
}

inline void Clock::SetStop()
{
#if defined(__linux__) || defined(__APPLE__)
  gettimeofday(&tv, &tz);
  stop = (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
#elif (_WIN32) || (__CYGWIN__)
  stop = clock();
#endif
}

inline double Clock::GetTimDiff()
{
#if defined(__linux__) || defined(__APPLE__)
  return (stop - strt);
#elif (_WIN32) || (__CYGWIN__)
  return (double)(stop - strt) / CLOCKS_PER_SEC;
#endif
}

inline void Clock::CompletionTime(uint &day, uint &hr, uint &min)
{
  double speed = 0.0;
#if defined(__linux__) || defined(__APPLE__)
  speed = (double)(prevStep) / (lastTime - strt);
#elif (_WIN32) || (__CYGWIN__)
  speed = (double)(prevStep) / ((double)(lastTime - strt) / CLOCKS_PER_SEC);
#endif
  ulong rem = lastStep - prevStep;
  ulong sec = rem / speed;
  day = sec / 86400;
  sec = sec % 86400;
  hr = sec / 3600;
  sec = sec % 3600;
  min = ceil(sec / 60.0);
}

#endif /*CLOCK_H*/
