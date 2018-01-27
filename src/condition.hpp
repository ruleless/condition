#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <list>

#include "condition_factory.hpp"

#ifndef NAMESPACE_BEG
# define NAMESPACE_BEG(spaceName) namespace spaceName {
# define NAMESPACE_END }
#endif // NAMESPACE_BEG

NAMESPACE_BEG(cond)

class ConditionFactory;
class Conditional;
typedef std::list<Conditional *> CondList;

// interface for condition
class Conditional
{
  public:
    Conditional(ConditionFactory *fac)
            :mFactory(fac)
    {}

    virtual ~Conditional() {}

    virtual bool test(const void *ctx) = 0;

    // check if the expression matchs this conditional object
    virtual bool match(const char *expr) const = 0;

    // number of child condition
    virtual int childCount() const = 0;

    // add condition
    virtual bool add(Conditional *c) = 0;

    inline ConditionFactory *getFactory()
    {
        return mFactory;
    }

  protected:
    ConditionFactory *mFactory;
};

// union condition
class UnionCondition : public Conditional
{
  public:
    UnionCondition(ConditionFactory *fac)
            :Conditional(fac)
            ,mConds()
    {}

    virtual ~UnionCondition();

    // number of child condition
    virtual int childCount() const;

    // add condition
    virtual bool add(Conditional *c);

  protected:
    CondList mConds;
};

// logic and
class ConditionAnd : public UnionCondition
{
  public:
    CONDITION_DECLARATION(ConditionAnd);

    ConditionAnd(ConditionFactory *fac)
            :UnionCondition(fac)
    {}

    virtual ~ConditionAnd() {}

    virtual bool test(const void *ctx);

    // check if the expression matchs this conditional object
    virtual bool match(const char *expr) const;
};

// logic or
class ConditionOr : public UnionCondition
{
  public:
    CONDITION_DECLARATION(ConditionOr);

    ConditionOr(ConditionFactory *fac)
            :UnionCondition(fac)
    {}

    virtual ~ConditionOr() {}

    virtual bool test(const void *ctx);

    // check if the expression matchs this conditional object
    virtual bool match(const char *expr) const;
};

// condition parser
class ConditionParser
{
  public:
    ConditionParser()
            :mCondList()
            ,mCurCond(NULL)
            ,mStack(0)
    {
        memset(mPreCond, NULL, sizeof(mPreCond));
    }

    virtual ~ConditionParser();

    // parse logical expression
    bool parse(const char *expr);
    bool _parse(const char *expr);

    // get the result by parse
    Conditional *getCondition() const;

    void _clearAll();

  private:
    Conditional *newCond(const char *expr);

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
