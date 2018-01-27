#include <ctype.h>
#include "condition.hpp"
#include "condition_factory.hpp"

#define EXPR_OP(c) ('(' == c || ')' == c || '&' == c || '|' == c)

NAMESPACE_BEG(cond)

UnionCondition::~UnionCondition()
{}

int UnionCondition::childCount() const
{
    return (int)mConds.size();
}

bool UnionCondition::add(Conditional *c)
{
    mConds.push_back(c);
    return true;
}

bool ConditionAnd::test(const void *ctx)
{
    CondList::iterator it = mConds.begin();

    for (; it != mConds.end(); it++)
    {
        Conditional *c = *it;
        if (c && !c->test(ctx))
            return false;
    }

    return true;
}

bool ConditionAnd::condMatch(const char *expr)
{
    if ('&' == *expr && '\0' == *(expr + 1))
        return true;
    return false;
}

bool ConditionOr::test(const void *ctx)
{
    CondList::iterator it = mConds.begin();

    for (; it != mConds.end(); it++)
    {
        Conditional *c = *it;
        if (c && c->test(ctx))
            return true;
    }

    return false;
}

bool ConditionOr::condMatch(const char *expr)
{
    if ('|' == *expr && '\0' == *(expr + 1))
        return true;
    return false;
}

ConditionParser::~ConditionParser()
{
    _clearAll();
}

bool ConditionParser::parse(const char *expr)
{
    _clearAll();
    if (!_parse(expr))
    {
        _clearAll();
        return false;
    }

    return true;
}

bool ConditionParser::_parse(const char *expr)
{
    const char *ptr = expr, *end_ptr = expr + strlen(expr);

    while (ptr < end_ptr && isspace(*ptr))
        ptr++;

    // if read ends
    if (ptr == end_ptr)
    {
        return true;
    }

    // parse operation
    switch (*ptr)
    {
    case '(':
        mPreCond[++mStack] = mCurCond;
        mCurCond = NULL;
        return _parse(++ptr);
    case ')':
        {
            if (mStack == 0)
                return false;

            Conditional *c = mPreCond[mStack--];
            if (NULL == c)
            {
                return _parse(++ptr);
            }
            else if (c->childCount() == 1)
            {
                if (NULL == mCurCond)
                    return false;

                c->add(mCurCond);
                mCurCond = c;
                return _parse(++ptr);
            }

            return false;
        }
    case '&':
    case '|':
        {
            if (!mCurCond || mCurCond->childCount() == 1)
                return false;

            char op[2] = {*ptr, '\0'};
            Conditional *logic = newCond(op);
            if (!logic)
                return false;

            if (!logic->add(mCurCond))
            {
                return false;
            }

            mCurCond = logic;
            return _parse(++ptr);
        }
    default:
        break;
    }

    // parse expression
    int phraselen = 0;
    char phrase[COND_EXPR_SIZE] = {0};
    const char *phrase_end = ptr;
    while ((phrase_end < end_ptr && !EXPR_OP(*phrase_end)) &&
           phraselen < (int)sizeof(phrase) - 1)
    {
        phrase[phraselen++] = *phrase_end;
        phrase_end++;
    }

    if (phrase_end < end_ptr && !EXPR_OP(*phrase_end))
        return false;

    Conditional *c = newCond(phrase);
    if (!c)
        return false;

    if (NULL == mCurCond)
    {
        mCurCond = c;
        return _parse(phrase_end);
    }
    if (mCurCond->childCount() == 1)
    {
        mCurCond->add(c);
        return _parse(phrase_end);
    }

    return false;
}

Conditional *ConditionParser::getCondition() const
{
    return mCurCond;
}

void ConditionParser::_clearAll()
{
    CondList::iterator it = mCondList.begin();

    for (; it != mCondList.end(); it++)
    {
        ConditionFactory *fac = (*it)->getFactory();
        fac->destroyCondition(*it);
    }
    mCondList.clear();

    memset(mPreCond, NULL, sizeof(mPreCond));
    mCurCond = NULL;
    mStack = 0;
}

Conditional *ConditionParser::newCond(const char *expr)
{
    ConditionFactoryRegistry *reg = ConditionFactoryRegistry::getRegistry();
    ConditionFactory *fac = reg->getCondFactory(expr);

    if (!fac)
        return NULL;

    Conditional *c = fac->createCondition(expr);
    assert(c && "new condition failed");
    mCondList.push_back(c);
    return c;
}

CONDITION_FACTORY_REGISTRATION(ConditionAnd);
CONDITION_FACTORY_REGISTRATION(ConditionOr);

NAMESPACE_END
