// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include "lz4compressor.h"
#include <vespa/vespalib/util/alloc.h>
#include <lz4.h>
#include <lz4hc.h>
#include <cassert>

using vespalib::alloc::Alloc;


namespace document {

size_t LZ4Compressor::adjustProcessLen(uint16_t, size_t len)   const { return LZ4_compressBound(len); }
size_t LZ4Compressor::adjustUnProcessLen(uint16_t, size_t len) const { return len; }

bool
LZ4Compressor::process(const CompressionConfig& config, const void * inputV, size_t inputLen, void * outputV, size_t & outputLenV)
{
    const char * input(static_cast<const char *>(inputV));
    char * output(static_cast<char *>(outputV));
    int sz(-1);
    int maxOutputLen = LZ4_compressBound(inputLen);
    if (config.compressionLevel > 6) {
        Alloc state = Alloc::alloc(LZ4_sizeofStateHC());
        sz = LZ4_compress_HC_extStateHC(state.get(), input, output, inputLen, maxOutputLen, config.compressionLevel);
    } else {
        Alloc state = Alloc::alloc(LZ4_sizeofState());
        sz = LZ4_compress_fast_extState(state.get(), input, output, inputLen, maxOutputLen, 1);
    }
    if (sz != 0) {
        outputLenV = sz;
    }
    assert(sz != 0);
    return (sz != 0);
    
}

bool
LZ4Compressor::unprocess(const void * inputV, size_t inputLen, void * outputV, size_t & outputLenV)
{
    const char * input(static_cast<const char *>(inputV));
    char * output(static_cast<char *>(outputV));
    int sz = LZ4_decompress_safe(input, output, inputLen, outputLenV);
    if (sz > 0) {
        outputLenV = sz;
    }
    assert(sz > 0);
    return (sz > 0);
}

}
