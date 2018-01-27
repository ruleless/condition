#ifndef __UTEST_H__
#define __UTEST_H__

#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

class UTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(UTest);
    CPPUNIT_TEST(testLogicExpr_1);
    CPPUNIT_TEST_SUITE_END();

  public:
    UTest();
    virtual ~UTest();

    virtual void setUp() {}

    virtual void tearDown() {}

    void testLogicExpr_1();
    void _testValidExpr(const char *expr);
};

#endif // __UTEST_H__
