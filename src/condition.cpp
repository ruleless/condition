#include <ctype.h>
#include "condition.hpp"

#define EXPR_OP(c) ('(' == c || ')' == c || '&' == c || '|' == c)

NAMESPACE_BEG(cond)

ConditionParser::~ConditionParser()
{}

bool ConditionParser::_parse(const char *expr)
{
    char *ptr = expr, *end_ptr = expr + strlen(expr);

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
    int exprlen = 0;
    char expr[EXPR_SIZE] = {0};
    char *expr_end = ptr;
    while ((expr_end < end_ptr && !EXPR_OP(*expr_end)) &&
           exprlen < sizeof(expr) - 1)
    {
        expr[exprlen++] = *expr_end;
        expr_end++;
    }

    if (expr_end < end_ptr && !EXPR_OP(*expr_end))
        return false;

    Conditional *c = newCond(expr);
    if (!c)
        return false;

    if (NULL == mCurCond)
    {
        mCurCond = c;
        return _parse(expr_end);
    }
    if (mCurCond->childCount() == 1)
    {
        mCurCond->add(c);
        return _parse(expr_end);
    }

    return false;
}

Conditional *ConditionParser::getCondition() const
{}

void ConditionParser::_clearAll()
{}

Conditional *ConditionParser::newCond(const std::string *expr)
{
    ConditionFactoryRegistry *reg = ConditionFactoryRegistry::getRegistry();
    ConditionFactory *fac = reg->getCondFactory(expr);

    if (!fac)
        return NULL;

    Conditional *c = fac->createCondition();
    assert(c && "new condition failed");
    mCondList->push_back(c);
    return c;
}

NAMESPACE_END
