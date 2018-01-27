#include "relational_condition.h"

NAMESPACE_BEG(cond)

static const int VALUE_SIZE = 8;

enum ERelationalOp
{
    Op_Equal,
    Op_Greater,
    Op_GreaterOrEqual,
    Op_Less,
    Op_LessOrEqual,
};

struct RelationalOp
{
    char symbol[4];
    ERelationalOp op;
} g_ops[] = {
    {"==", Op_Equal},
    {">",  Op_Greater},
    {">=", Op_GreaterOrEqual},
    {"<",  Op_Less},
    {"<=", Op_LessOrEqual},
};

struct RelExpParseResult
{
    char lval[VALUE_SIZE], rval[VALUE_SIZE];
    int opindex;
};

static char *parseValue(const char *ptr, const char *end_ptr, char *val)
{
    int i = 0;

    while (ptr < end_ptr && isspace(*ptr))
        ptr++;
    while (i < VALUE_SIZE - 1 && ptr < end_ptr)
    {
        if (isdigit(*ptr))
        {
            val[i++] = *ptr++;
        }
        else if (isspace(*ptr))
        {
            break;
        }
        else
        {
            return NULL;
        }
    }
    while (ptr < end_ptr && isspace(*ptr))
        ptr++;
    val[i] = '\0';
    if (ptr != end_ptr || i == 0)
    {
        return NULL;
    }

    return val;
}

static RelExpParseResult parseExpr(const char *expr)
{
    RelExpParseResult r = {{0}, {0}, -1};
    const char *midptr = NULL, *end_ptr = expr + strlen(expr);
    int i;

    for (i = 0; sizeof(g_ops) / sizeof(g_ops[0]); i++)
    {
        if ((midptr = strstr(expr, g_ops[i].symbol)))
        {
            r.opindex = i;
            break;
        }
    }

    if (-1 == r.opindex)
    {
        return r;
    }

    if (!parseValue(expr, midptr, r.lval))
    {
        r.opindex = -1;
        return r;
    }

    midptr += strlen(g_ops[r.opindex].symbol);
    if (!parseValue(midptr, end_ptr, r.rval))
    {
        r.opindex = -1;
        return r;
    }

    return r;
}

bool RelationalCondition::test(const void *ctx)
{
    RelExpParseResult r = parseExpr(mExpr);
    int lval = atoi(r.lval);
    int rval = atoi(r.rval);

    switch (g_ops[r.opindex].op)
    {
    case Op_Equal:
        return lval == rval;
    case Op_Greater:
        return lval > rval;
    case Op_GreaterOrEqual:
        return lval >= rval;
    case Op_Less:
        return lval < rval;
    case Op_LessOrEqual:
        return lval <= rval;
    }

    assert(false && "unknown relational operation");
}

bool RelationalCondition::condMatch(const char *expr)
{
    RelExpParseResult r = parseExpr(expr);
    return r.opindex != -1;
}

CONDITION_FACTORY_REGISTRATION(RelationalCondition);

NAMESPACE_END
