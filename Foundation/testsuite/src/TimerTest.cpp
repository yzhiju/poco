//
// TimerTest.cpp
//
// $Id: //poco/svn/Foundation/testsuite/src/TimerTest.cpp#2 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "TimerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"


using Poco::Timer;
using Poco::TimerCallback;
using Poco::Thread;
using Poco::Stopwatch;


TimerTest::TimerTest(const std::string& name): CppUnit::TestCase(name)
{
}


TimerTest::~TimerTest()
{
}


void TimerTest::testTimer()
{
	Timer t(100, 200);
	assert (t.getStartInterval() == 100);
	assert (t.getPeriodicInterval() == 200);

	Stopwatch sw;
	TimerCallback<TimerTest> tc(*this, &TimerTest::onTimer);
	sw.start();
        t.start(tc);
        _event.wait();
        sw.stop();
        assert (sw.elapsed() >= 80000 && sw.elapsed() < 250000);
        sw.restart();
        _event.wait();
        sw.stop();
        assert (sw.elapsed() >= 180000 && sw.elapsed() < 250000);
        sw.restart();
        _event.wait();
        sw.stop();
        assert (sw.elapsed() >= 180000 && sw.elapsed() < 250000);
        t.stop();       
}


void TimerTest::testDuplicateStop()
{
	Timer t(100, 200);
	t.stop();
	t.stop();

	TimerCallback<TimerTest> tc(*this, &TimerTest::onTimer);
	t.start(tc);
	_event.wait();
	t.stop();
	t.stop();
}

void TimerTest::setUp()
{
}


void TimerTest::tearDown()
{
}


void TimerTest::onTimer(Timer& timer)
{
	_event.set();
}


CppUnit::Test* TimerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("TimerTest");

	CppUnit_addTest(pSuite, TimerTest, testTimer);
	CppUnit_addTest(pSuite, TimerTest, testDuplicateStop);

	return pSuite;
}
