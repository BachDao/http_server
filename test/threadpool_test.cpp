//
// Created by Bach Dao.
//
#include "http_server/threadpool/threadpool.h"
#include <gtest/gtest.h>
using namespace std::chrono_literals;
using namespace http_simple::detail;

TEST(ThreadPool, SubmitWork) {
  int flag = false;
  {
    thread_pool tp(8);
    tp.submit([&] { flag = true; });
    std::this_thread::sleep_for(500ms);
  }
  EXPECT_TRUE(flag);
}

TEST(ThreadPool, BulkSubmit) {
  std::mutex mutex;
  std::condition_variable cv;
  std::atomic_uint64_t counter = 0;
  size_t testRound = 1'000'000;

  thread_pool tp(8);
  repeat(testRound, [&] { tp.submit([&] { counter.fetch_add(1); }); });

  tp.submit([&] {
    std::lock_guard lck{mutex};
    cv.notify_all();
  });
  std::unique_lock lck{mutex};
  cv.wait(lck);
  EXPECT_EQ(counter, testRound);
}

TEST(ThreadPool, PrivateSubmit) {
  std::mutex mutex;
  std::condition_variable cv;
  std::atomic_uint64_t counter = 0;
  size_t testRound = 1000;

  // use one worker thread to avoid other worker finish their
  // job sooner and notify main thread exit
  thread_pool tp(1);
  repeat(testRound, [&] {
    tp.submit([&] {
      counter.fetch_add(1);
      tp.submit([&] { counter.fetch_add(1); });
    });
  });

  tp.submit([&] {
    std::lock_guard lck{mutex};
    cv.notify_all();
  });
  std::unique_lock lck{mutex};
  cv.wait(lck);
  EXPECT_EQ(counter, testRound * 2);
}