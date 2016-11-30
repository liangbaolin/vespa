// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include <vespa/fastos/fastos.h>
#include <vespa/searchlib/attribute/attributefactory.h>
#include <vespa/searchlib/attribute/integerbase.h>
#include <vespa/searchlib/attribute/floatbase.h>
#include "defines.h"

#include <vespa/log/log.h>
LOG_SETUP(".createsetfastsearch");


#include <vespa/searchlib/attribute/attributevector.hpp>
#include <vespa/searchlib/attribute/enumstore.hpp>
#include <vespa/searchlib/attribute/enumattribute.hpp>
#include <vespa/searchlib/attribute/multivalueattribute.hpp>
#include <vespa/searchlib/attribute/multienumattribute.hpp>
#include <vespa/searchlib/attribute/multinumericenumattribute.hpp>
#include <vespa/searchlib/attribute/multinumericpostattribute.hpp>
#include <vespa/searchlib/attribute/multistringpostattribute.hpp>

namespace search {

using attribute::BasicType;

#define INTSET(T)   MultiValueNumericPostingAttribute< ENUM_ATTRIBUTE(IntegerAttributeTemplate<T>), WEIGHTED_MULTIVALUE_ENUM_ARG >
#define FLOATSET(T) MultiValueNumericPostingAttribute< ENUM_ATTRIBUTE(FloatingPointAttributeTemplate<T>), WEIGHTED_MULTIVALUE_ENUM_ARG >

#define CREATEINTSET(T, fname, info) static_cast<AttributeVector *>(new INTSET(T)(fname, info))
#define CREATEFLOATSET(T, fname, info) static_cast<AttributeVector *>(new FLOATSET(T)(fname, info))


AttributeVector::SP
AttributeFactory::createSetFastSearch(const vespalib::string & baseFileName, const Config & info)
{
    assert(info.collectionType().type() == attribute::CollectionType::WSET);
    assert(info.fastSearch());
    AttributeVector::SP ret;
    switch(info.basicType().type()) {
    case BasicType::UINT1:
    case BasicType::UINT2:
    case BasicType::UINT4:
        break;
    case BasicType::INT8:
        ret.reset(CREATEINTSET(int8_t, baseFileName, info));
        break;
    case BasicType::INT16:
        ret.reset(CREATEINTSET(int16_t, baseFileName, info));
        break;
    case BasicType::INT32:
        ret.reset(CREATEINTSET(int32_t, baseFileName, info));
        break;
    case BasicType::INT64:
        ret.reset(CREATEINTSET(int64_t, baseFileName, info));
        break;
    case BasicType::FLOAT:
        ret.reset(CREATEFLOATSET(float, baseFileName, info));
        break;
    case BasicType::DOUBLE:
        ret.reset(CREATEFLOATSET(double, baseFileName, info));
        break;
    case BasicType::STRING:
        ret.reset(static_cast<AttributeVector *>(new WeightedSetStringPostingAttribute(baseFileName, info)));
        break;
    default:
        break;
    }
    return ret;
}

}
