/** @file util.hpp.h
 *   @brief Created on 2021-04-08 by Ben
 */
#pragma once

/**********************************************************************
 * Includes
 **********************************************************************/
#include "FreeRTOS.h"
#include "queue.h"
#include <concepts>
#include <optional>

// clang-format off
template <typename T, typename DataType>
concept Queue = requires (T rb, DataType val){
  rb.clear();
  {rb.is_empty()} -> std::same_as<bool>;
  {rb.is_full()} -> std::same_as<bool>;
  {rb.size()} -> std::same_as<std::size_t>;
  {rb.pop()} -> std::same_as<std::optional<DataType> >;
  {rb.peek()} -> std::same_as<std::optional<DataType> >;
  rb.push(val);
};
// clang-format on

template <typename T, std::size_t N> class ring_buffer
{
public:
  [[nodiscard]] bool
  is_empty () const
  {
    return !full && (head == tail);
  }

  void
  clear ()
  {
    full = false;
    head = 0;
    tail = 0;
  }

  [[nodiscard]] bool
  is_full () const
  {
    return full;
  }

  [[nodiscard]] std::size_t
  size () const
  {
    if (full)
      {
        return N;
      }
    if (head >= tail)
      {
        return head - tail;
      }
    return N + head - tail;
  }

  void
  push (T val)
  {
    internal_storage[head] = val;
    if (full)
      {
        tail = (tail + 1) % N;
      }
    head = (head + 1) % N;
    full = (tail == head);
  }

  std::optional<T>
  pop ()
  {
    if (is_empty ())
      {
        return {};
      }

    auto val = internal_storage[tail];
    full = false;
    tail = (tail + 1) % N;
    return val;
  }

  [[nodiscard]] std::optional<T>
  peek (std::size_t idx) const
  {
    if (size () >= idx)
      {
        return {};
      }
    return internal_storage[(head + idx) % N];
  }

  [[nodiscard]] std::optional<T>
  peek () const
  {
    return peek (0);
  }

protected:
  T internal_storage[N]{};
  std::size_t head = 0;
  std::size_t tail = 0;
  bool full = false;
};

template <typename T, std::size_t N> class blocking_ring_buffer
{
public:
  blocking_ring_buffer ()
  {
    queue = xQueueCreateStatic (N, item_size, buffer, &static_queue);
  }

  bool
  is_empty ()
  {
    return this->size () == 0;
  }

  bool
  is_full ()
  {
    return this->size () == N;
  }

  void
  clear ()
  {
    xQueueReset (queue);
  }

  std::size_t
  size ()
  {
    return uxQueueMessagesWaiting (queue);
  }

  void
  push (const T &val)
  {
    xQueueSend (queue, &val, max_wait_time);
  }

  std::optional<T>
  pop ()
  {
    T value{};
    if (xQueueReceive (queue, &value, max_wait_time) != pdTRUE)
      {
        return {};
      }
    return value;
  }

  std::optional<T>
  peek ()
  {
    T value{};
    if (xQueuePeek (queue, &value, max_wait_time) != pdTRUE)
      {
        return {};
      }
    return value;
  }

private:
  static constexpr std::size_t item_size = sizeof (T);
  static constexpr TickType_t max_wait_time = portMAX_DELAY;
  StaticQueue_t static_queue{};
  QueueHandle_t queue{};
  uint8_t buffer[item_size * N]{};
};
