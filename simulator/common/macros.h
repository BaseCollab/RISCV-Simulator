#ifndef SIMULATOR_COMMON_MACROS_H
#define SIMULATOR_COMMON_MACROS_H

#include <cstdint>

// Inline (disabled for DEBUG)
#if !defined(NDEBUG)
#define ALWAYS_INLINE
#else
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

#define LIKELY(exp) (__builtin_expect((exp) != 0, true))
#define UNLIKELY(exp) (__builtin_expect((exp) != 0, false))

#define NO_COPY_CTOR(TypeName) TypeName(const TypeName &) = delete
#define NO_COPY_OPERATOR(TypeName) TypeName &operator=(const TypeName &) = delete

#define NO_MOVE_CTOR(TypeName) TypeName(TypeName &&) = delete
#define NO_MOVE_OPERATOR(TypeName) TypeName &operator=(TypeName &&) = delete

#define NO_COPY_SEMANTIC(TypeName) \
    NO_COPY_CTOR(TypeName);        \
    NO_COPY_OPERATOR(TypeName)

#define NO_MOVE_SEMANTIC(TypeName) \
    NO_MOVE_CTOR(TypeName);        \
    NO_MOVE_OPERATOR(TypeName)

#define DEFAULT_MOVE_CTOR(TypeName) TypeName(TypeName &&) = default;

#define DEFAULT_MOVE_OPERATOR(TypeName) TypeName &operator=(TypeName &&) = default

#define DEFAULT_COPY_CTOR(TypeName) TypeName(const TypeName &) = default;

#define DEFAULT_COPY_OPERATOR(TypeName) TypeName &operator=(const TypeName &) = default

#define DEFAULT_MOVE_SEMANTIC(TypeName) \
    DEFAULT_MOVE_CTOR(TypeName)         \
    DEFAULT_MOVE_OPERATOR(TypeName)

#define DEFAULT_COPY_SEMANTIC(TypeName) \
    DEFAULT_COPY_CTOR(TypeName)         \
    DEFAULT_COPY_OPERATOR(TypeName)

#endif // SIMULATOR_COMMON_MACROS_H
