#ifndef ABSTRACTDATAVISITOR_H
#define ABSTRACTDATAVISITOR_H
#include "../abstractdata.h"
class AbstractDataVisitor {
public:
    virtual ~AbstractDataVisitor() = default;
    virtual void visitSHP();
    virtual void visitGeotiff();
    
};

#endif