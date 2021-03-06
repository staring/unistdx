#include <thread>
#include <chrono>

#include <unistdx/base/make_object>
#include <unistdx/fs/file_mutex>
#include <unistdx/ipc/process>
#include <gtest/gtest.h>

TEST(FileMutex, Check) {
	typedef sys::file_mutex mutex_type;
	typedef std::lock_guard<mutex_type> lock_type;
	const char* filename = "file_mutex_test.lock";
	mutex_type mtx(filename, 0600);
	EXPECT_TRUE(static_cast<bool>(mtx));
	sys::process child([filename] () {
		mutex_type mtx2(filename, 0600);
		lock_type lock(mtx2);
		::pause();
		return 0;
	});
	using namespace std::chrono;
	using namespace std::this_thread;
	sleep_for(milliseconds(500));
	EXPECT_FALSE(mtx.try_lock());
	child.terminate();
	child.join();
	EXPECT_TRUE(mtx.try_lock());
}
