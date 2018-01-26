#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <list>

#ifndef NAMESPACE_BEG
# define NAMESPACE_BEG(spaceName) namespace spaceName {
# define NAMESPACE_END }
#endif // NAMESPACE_BEG

NAMESPACE_BEG(cond)

class Conditional;
typedef std::list<Conditional *> CondList;

// interface for condition
class Conditional
{
  public:
    Conditional() {}
    virtual ~Conditional() {}

    virtual bool test() = 0;

    // check if the expression matchs this conditional object
    virtual bool match(const std::string &expr) const = 0;

    // number of child condition
    virtual int childCount() const;

    // add condition
    virtual bool add(Conditional *c);
};

// logic and
class ConditionAnd : public Conditional
{
  public:
    ConditionAnd()
            :Conditional()
    {}

    virtual ~ConditionAnd();

  private:
};

// logic or
class ConditionOr : public Conditional
{
  public:
    ConditionOr()
            :Conditional()
    {}

    virtual ~ConditionOr();
};

// condition parser
class ConditionParser
{
  public:
    ConditionParser()
            :mCondList()
            ,mPreCond(NULL)
            ,mCurCond(NULL)
            ,mStack(0)
    {}

    virtual ~ConditionParser();

    // parse logical expression
    bool parse(const char *expr);
    bool _parse(const char *expr);

    // get the result by parse
    Conditional *getCondition() const;

    void _clearAll();

  private:
    Conditional *newCond(const std::string *expr);

  private:
    static const int STACK_SIZE = 32;
    static const int EXPR_SIZE = 64;

    CondList mCondList;

    Conditional *mPreCond[STACK_SIZE];
    Conditional *mCurCond;
    int mStack;
};

NAMESPACE_END

#endif // __CONDITION_HPP__
