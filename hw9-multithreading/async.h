#pragma once

#include <cstddef>

namespace async {

using handle_t = void *;

handle_t connect(std::size_t bulkSize);

void receive(handle_t handle, const char *data, std::size_t size);

void disconnect(handle_t handle);

void stop();
}
