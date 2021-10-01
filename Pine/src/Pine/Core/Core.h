#pragma once

// Defines and macros
#define PINE_VERSION "v0.1-pre-alpha"

#if PINE_DEBUG
#define ENABLE_ASSERT 1
#endif

#define PINE_OPENGL_MAJOR_VERSION 4
#define PINE_OPENGL_MINOR_VERSION 5


#ifdef ENABLE_ASSERT
#define PINE_ASSERT(x, ...) { if (!(x)) { PINE_LOG_CORE_CRITICAL(__VA_ARGS__); __debugbreak(); } }
#else
#define PINE_ASSERT(x, ...)
#endif
