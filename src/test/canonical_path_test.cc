#include <gtest/gtest.h>
#include <string>
#include <unistdx/fs/canonical_path>
#include <unordered_set>

TEST(CanonicalPath, Hash) {
	std::unordered_set<sys::canonical_path> s;
	for (int i=0; i<10; ++i) {
		s.emplace("/tmp");
	}
	EXPECT_EQ(1, s.size());
}

TEST(CanonicalPath, EqualsCurrentWorkingDirectory) {
	sys::canonical_path dir1(".");
	char buf[4096*4] = {0};
	EXPECT_NE(nullptr, ::getcwd(buf, sizeof(buf)));
	sys::canonical_path dir2(buf);
	EXPECT_EQ(dir1, dir2);
	sys::canonical_path dir3 = sys::this_process::workdir();
	EXPECT_EQ(dir2, dir3);
}

TEST(CanonicalPath, Assign) {
	sys::canonical_path dir1(".");
	sys::canonical_path dir2("..");
	dir1 = dir1.dirname();
	EXPECT_EQ(dir1, dir2);
}

TEST(CanonicalPath, Dirname) {
	sys::canonical_path dir1("/usr");
	sys::canonical_path dir2("/");
	EXPECT_EQ(dir1.dirname(), dir2);
	EXPECT_EQ(dir2.dirname(), dir2);
	EXPECT_EQ(dir2.basename(), dir2);
}

TEST(CanonicalPath, TwoArgConstructor) {
	sys::canonical_path cwd(".");
	EXPECT_EQ(sys::canonical_path(cwd.dirname(), cwd.basename()), cwd);
}

TEST(CanonicalPath, NonExistent) {
	sys::canonical_path root("/");
	sys::canonical_path tmp;
	EXPECT_THROW(
		{
		    sys::canonical_path nonexistent(root, "non-existent-directory");
		},
		sys::bad_call
	);
}

TEST(CanonicalPath, FromString) {
	std::string str = "/";
	sys::canonical_path root(str);
}
