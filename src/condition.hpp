#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#ifndef NAMESPACE_BEG
# define NAMESPACE_BEG(spaceName) namespace spaceName {
# define NAMESPACE_END }
#endif // NAMESPACE_BEG

NAMESPACE_BEG(cond)

class Conditional
{
  public:
    Conditional() {}
    virtual ~Conditional() {}

    virtual bool test() = 0;
};

NAMESPACE_END

#endif // __CONDITION_HPP__
