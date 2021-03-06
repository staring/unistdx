#ifndef TEST_THREAD_MUTEX_TEST_HH
#define TEST_THREAD_MUTEX_TEST_HH

#include <chrono>
#include <condition_variable>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>
#include <iostream>

template <class Mutex>
struct ThreadMutexTest: public ::testing::Test {

	typedef Mutex mutex_type;
	typedef std::lock_guard<Mutex> lock_type;

	void
	test_thread() {
		std::condition_variable cnd;
		std::mutex orig_mtx;
		mutex_type mtx;
		std::thread child([&] () {
			lock_type lock(mtx);
			std::unique_lock<std::mutex> orig_lock(orig_mtx);
			cnd.wait(orig_lock);
		});
		using namespace std::chrono;
		using namespace std::this_thread;
		sleep_for(milliseconds(100));
		EXPECT_FALSE(mtx.try_lock());
		cnd.notify_one();
		child.join();
		EXPECT_TRUE(mtx.try_lock());
	}

};

#endif // vim:filetype=cpp
