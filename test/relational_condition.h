#ifndef __RELATIONAL_CONDITION_H__
#define __RELATIONAL_CONDITION_H__

#include "condition_factory.hpp"

NAMESPACE_BEG(cond)

class RelationalCondition : public Conditional
{
  public:
    RelationalCondition()
            :Conditional()
    {}

    virtual ~RelationalCondition() {}

    virtual bool test(const void *ctx);

    static bool condMatch(const char *expr);
};

NAMESPACE_END

#endif // __RELATIONAL_CONDITION_H__
