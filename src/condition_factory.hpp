#ifndef __CONDITION_FACTORY_HPP__
#define __CONDITION_FACTORY_HPP__

#include <vector>

#include "iterator_wrapper.hpp"
#include "condition.hpp"

NAMESPACE_BEG(cond)

class ConditionFactory
{
  public:
    ConditionFactory() {}
    virtual ~ConditionFactory() {}

    virtual bool condMatch(const char *expr) const = 0;

    virtual Conditional *createCondition(const char *expr) = 0;
    virtual void destroyCondition(Conditional *cond) = 0;
};

template <class Cond>
class ConditionFactoryImp : public ConditionFactory
{
  public:
    ConditionFactoryImp()
            :ConditionFactory()
    {}

    virtual ~ConditionFactoryImp() {}

    virtual bool condMatch(const char *expr) const
    {
        return Cond::condMatch(expr);
    }

    virtual Conditional *createCondition(const char *expr)
    {
        Cond *c = new Cond();

        if (!c)
            return NULL;
        c->_setEnv(this, expr);
        return c;
    }

    virtual void destroyCondition(Conditional *cond)
    {
        delete cond;
    }
};

typedef std::vector<ConditionFactory *> CondFactoryList;
class ConditionFactoryRegistry
{
  public:
    ConditionFactoryRegistry();
    ~ConditionFactoryRegistry();

    static ConditionFactoryRegistry *getRegistry();
    static bool isValid();

    void registerFactory(ConditionFactory *factory);
    void unregisterFactory(ConditionFactory *factory);

    ConditionFactory *getCondFactory(const char *expr);

    VectorIterator<CondFactoryList> getIter();

  protected:
    CondFactoryList mFactories;
    static bool mbValid;
};

template<class Cond>
class AutoRegisterCondition
{
  public:
    AutoRegisterCondition()
            :mRegistry(ConditionFactoryRegistry::getRegistry())
    {
        mRegistry->registerFactory(&mFactory);
    }

    ~AutoRegisterCondition()
    {
        if (ConditionFactoryRegistry::isValid())
        {
            mRegistry->unregisterFactory(&mFactory);
        }
    }

  private:
    ConditionFactoryRegistry *mRegistry;
    ConditionFactoryImp<Cond> mFactory;
};

#define CONDITION_FACTORY_REGISTRATION(Cond)                        \
    static AutoRegisterCondition<Cond> _autoRegisterRegistry_##Cond

NAMESPACE_END

#endif // __CONDITION_FACTORY_HPP__
