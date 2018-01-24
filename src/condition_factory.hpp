#ifndef __CONDITION_FACTORY_HPP__
#define __CONDITION_FACTORY_HPP__

#include <vector>

#include "condition.hpp"
#include "iterator_wrapper.hpp"

NAMESPACE_BEG(cond)

class ConditionFactory
{
  public:
    ConditionFactory() {}
    virtual ~ConditionFactory() {}

    virtual std::string getName() = 0;
    virtual Conditional* createCondition() = 0;
};

template <class Cond>
class ConditionFactoryImp : public ConditionFactory
{
  public:
    ConditionFactoryImp()
            :ConditionFactory()
    {}

    virtual ~ConditionFactoryImp() {}

    virtual std::string getName()
    {
        return Cond::getName();
    }

    virtual Conditional *createCondition()
    {
        return new Cond();
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

    ConditionFactory *getCondFactory(const std::string &cond);

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

#define CONDITION_DECLARATION(Cond)             \
    static std::string getName();               \

#define CONDITION_IMPLEMENTION(Cond, name)      \
    std::string Cond::getName()                 \
    {                                           \
        return name;                            \
    }

#define CONDITION_FACTORY_REGISTRATION(Cond)                        \
    static AutoRegisterCondition<Cond> _autoRegisterRegistry_##Cond

NAMESPACE_END

#endif // __CONDITION_FACTORY_HPP__
