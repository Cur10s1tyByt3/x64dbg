#include <cstdio>
#include <dlfcn.h>
#include <cstdlib>
#include "idasymbols.h"

#ifndef __clangd__

#define DECLARE_STUB(name) \
void* fp_##name = nullptr;

#define DEFINE_STUB(name) \
__attribute__((naked)) void name() { \
        __asm__("adrp x16, _fp_" #name "@PAGE\n" \
                "ldr x16, [x16, _fp_" #name "@PAGEOFF]\n" \
                "br x16"); \
}

#define SYMBOL_ENTRY(name) {#name, &fp_##name}

extern "C"
{
struct SymbolEntry {
    const char* name;
    void** ptr;
};


// Declare all function pointers
#define X(name) DECLARE_STUB(name)
IDA_SYMBOLS
#undef X

static SymbolEntry g_symbols[] = {
#define X(name) SYMBOL_ENTRY(name),
    IDA_SYMBOLS
#undef X
    };

extern "C" bool idalib_resolve()
{
    auto g_libidalib_handle = dlopen("/Applications/IDA Professional 9.2.app/Contents/MacOS/libidalib.dylib", RTLD_NOW | RTLD_LOCAL);
    fprintf(stderr, "ida_resolve(), libidalib.dylib: %p\n", g_libidalib_handle);

    if (!g_libidalib_handle) {
        fprintf(stderr, "Failed to load: %s\n", dlerror());
        return false;
    }

    // Resolve all symbols
    bool resolved = true;
    for (auto& entry : g_symbols) {
        *entry.ptr = dlsym(g_libidalib_handle, entry.name);
        if (!*entry.ptr) {
            fprintf(stderr, "Failed to resolve %s\n", entry.name);
            resolved = false;
        }
    }
    return resolved;
}

// Trampoline functions
// Define all trampolines
#define X(name) DEFINE_STUB(name)
IDA_SYMBOLS
#undef X

}

#endif // __clangd__
