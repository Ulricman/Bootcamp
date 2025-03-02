/**
 * This script allocate a 16MB stack for a thread to run, which
 * typically is 8MB, and run a function `probe` recursively to
 * get the size of the allocated (approximately).
 */

#include <pthread.h>
#include <signal.h>

#include <chrono>
#include <iostream>
#include <thread>

/**
 * * This function would run recursively until overflows the
 * * allocated stack.
 */
void* probe(void* args) {
  // Print the address of the current frame.
  std::cout << __builtin_frame_address(0) << std::endl;
  probe(args);
  return nullptr;
}

int main() {
  pthread_t t;
  pthread_attr_t attr;
  size_t stack_size = 1 << 24;
  void* stack;

  // Allocate a block of aligned memory.
  if (posix_memalign(&stack, sysconf(_SC_PAGESIZE), stack_size) != 0) {
    perror("posix_memalign failed");
    return 1;
  }

  // Initialize thread attribute.
  if (pthread_attr_init(&attr) != 0) {
    perror("pthread_attr_init failed");
    free(stack);
    return 1;
  }

  // Set stack attribute of the thread.
  if (pthread_attr_setstack(&attr, stack, stack_size) != 0) {
    perror("pthread_attr_setstack failed");
    pthread_attr_destroy(&attr);
    free(stack);
    return 1;
  }

  // Create a thread with custom stack attribute.
  if (pthread_create(&t, &attr, probe, nullptr) != 0) {
    perror("pthread_create");
    pthread_attr_destroy(&attr);
    free(stack);
    return 1;
  }

  // Clean up attributes.
  // The `pthread_attr_t` object becomes invalid can cannot be reused unless
  // reinitialized.
  pthread_attr_destroy(&attr);

  // Wait for the thread to finish.
  pthread_join(t, nullptr);
  free(stack);
}