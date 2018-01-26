#include "condition_factory.hpp"

NAMESPACE_BEG(cond)

bool ConditionFactoryRegistry::mbValid = false;

ConditionFactoryRegistry::ConditionFactoryRegistry()
{
    mbValid = true;
}

ConditionFactoryRegistry::~ConditionFactoryRegistry()
{
    mbValid = false;
}

ConditionFactoryRegistry *ConditionFactoryRegistry::getRegistry()
{
    static ConditionFactoryRegistry s_registry;
    return &s_registry;
}

bool ConditionFactoryRegistry::isValid()
{
    return mbValid;
}

void ConditionFactoryRegistry::registerFactory(ConditionFactory *factory)
{
    mFactories.push_back(factory);
}

void ConditionFactoryRegistry::unregisterFactory(ConditionFactory *factory)
{
    CondFactoryList::iterator it = mFactories.begin();
    while (it != mFactories.end())
    {
        if ((*it) == factory)
        {
            it = mFactories.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

ConditionFactory *ConditionFactoryRegistry::getCondFactory(const std::string &expr)
{
    CondFactoryList::iterator it = mFactories.begin();
    for (; it != mFactories.end(); ++it)
    {
        if (condMatch(expr))
        {
            return (*it);
        }
    }
    return NULL;
}

VectorIterator<CondFactoryList> ConditionFactoryRegistry::getIter()
{
    return VectorIterator<CondFactoryList>(mFactories.begin(), mFactories.end());
}

NAMESPACE_END
