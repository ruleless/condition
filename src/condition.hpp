#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <list>

#define COND_EXPR_SIZE 64
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
    Conditional()
            :mFactory(NULL)
    {
        *mExpr = '\0';
    }

    virtual ~Conditional() {}

    virtual bool test(const void *ctx) = 0;

    // number of child condition
    virtual int childCount() const { return 0; }

    // add condition
    virtual bool add(Conditional *c) { return false; };

    inline void _setEnv(ConditionFactory *fac, const char *expr)
    {
        mFactory = fac;
        snprintf(mExpr, sizeof(mExpr), "%s", expr);
    }
    inline ConditionFactory *getFactory()
    {
        return mFactory;
    }

  protected:
    ConditionFactory *mFactory;
    char mExpr[COND_EXPR_SIZE];
};

// union condition
class UnionCondition : public Conditional
{
  public:
    UnionCondition()
            :Conditional()
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
    ConditionAnd()
            :UnionCondition()
    {}

    virtual ~ConditionAnd() {}

    virtual bool test(const void *ctx);

    static bool condMatch(const char *expr);
};

// logic or
class ConditionOr : public UnionCondition
{
  public:
    ConditionOr()
            :UnionCondition()
    {}

    virtual ~ConditionOr() {}

    virtual bool test(const void *ctx);

    static bool condMatch(const char *expr);
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

    CondList mCondList;

    Conditional *mPreCond[STACK_SIZE];
    Conditional *mCurCond;
    int mStack;
};

NAMESPACE_END

#endif // __CONDITION_HPP__
