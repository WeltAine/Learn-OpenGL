#pragma once

#define LEARNOPENGL_ASSERT(condition, ...) if(!(condition)) {LOG_ERROR("Assert Failed {0}", __VA_ARGS__); __debugbreak();}