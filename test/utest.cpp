#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/ui/text/TestRunner.h"

#include "utest.h"
#include "relational_condition.h"

using namespace cond;

CPPUNIT_TEST_SUITE_REGISTRATION(UTest);

//--------------------------------------------------------------------------
UTest::UTest()
{}

UTest::~UTest()
{}

void UTest::testLogicExpr_1()
{
    _testValidExpr("10>1");
    _testValidExpr("10 > 1");
    _testValidExpr("(10 > 1)& ((1 > 0 | 1 < 0) & (100 > 10)");
    _testValidExpr("(10 >1)& ((1 > 0 | 1 < 0) & (100 > 1099)|(1000 > 10)");
}

void UTest::_testValidExpr(const char *expr)
{
    ConditionParser parser;

    CPPUNIT_ASSERT_MESSAGE(expr, parser.parse(expr));

    Conditional *c = parser.getCondition();
    CPPUNIT_ASSERT_MESSAGE(expr, c);
    CPPUNIT_ASSERT_MESSAGE(expr, c->test(NULL));
}
//--------------------------------------------------------------------------


int main(int argc, char *argv[])
{
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    runner.run();
    exit(0);
}
