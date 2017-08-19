test(
	'unistdx/semaphore',
	executable(
		'locks-test',
		sources: 'locks_test.cc',
		include_directories: srcdir,
		dependencies: [threads,unistdx,gtest],
	)
)

test(
	'unistdx/sharedmem',
	executable(
		'sharedmem-test',
		sources: 'sharedmem_test.cc',
		include_directories: srcdir,
		dependencies: [threads,unistdx,gtest],
	)
)

test(
	'unistdx/endpoint',
	executable(
		'endpoint-test',
		sources: 'endpoint_test.cc',
		include_directories: srcdir,
		dependencies: [gtest,unistdx],
	)
)

test(
	'unistdx/poller',
	executable(
		'poller-test',
		sources: 'poller_test.cc',
		include_directories: srcdir,
		dependencies: [gtest,unistdx],
	)
)

test(
	'unistdx/dir',
	executable(
		'sys-test',
		sources: 'sys_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)

test(
	'unistdx/process',
	executable(
		'process-test',
		sources: 'process_test.cc',
		include_directories: srcdir,
		dependencies: [gtest,unistdx],
	)
)

test(
	'sys::fildesbuf',
	executable(
		'buffer-test',
		sources: 'buffer_test.cc',
		include_directories: srcdir,
		dependencies: [gtest,unistdx],
	)
)

test(
	'sys::users',
	executable(
		'users-test',
		sources: 'users_test.cc',
		include_directories: srcdir,
		dependencies: [unistdx,gtest],
	)
)

executable(
	'posix-compliance',
	sources: 'posix_test.cc',
	include_directories: srcdir,
)

test(
	'system-test',
	executable(
		'system-test',
		sources: 'system_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)

test(
	'file-mutex-test',
	executable(
		'file-mutex-test',
		sources: 'file_mutex_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)

test(
	'signal-test',
	executable(
		'signal-test',
		sources: 'signal_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)

test(
	'unistdx/path',
	executable(
		'path-test',
		sources: 'path_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)

test(
	'unistdx/net/ifaddr',
	executable(
		'ifaddr-test',
		sources: 'ifaddr_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)

test(
	'unistdx/net/endian',
	executable(
		'endian-test',
		sources: 'endian_test.cc',
		include_directories: srcdir,
		dependencies: [gtest, unistdx],
	)
)
test(
	'std::uint128_t (builtin)',
	executable(
		'uint128-test',
		sources: 'uint128_test.cc',
		include_directories: srcdir,
		dependencies: gtest
	)
)

test(
	'std::uint128_t (emulate)',
	executable(
		'uint128-emulate-test',
		sources: 'uint128_test.cc',
		cpp_args: '-DUNISTDX_TEST_FORCE_CUSTOM_UINT128',
		include_directories: srcdir,
		dependencies: gtest
	)
)

test(
	'paired_iterator',
	executable(
		'iterator-test',
		sources: 'iterator_test.cc',
		include_directories: srcdir,
		dependencies: gtest
	)
)

test(
	'random-test',
	executable(
		'random-test',
		sources: 'random_test.cc',
		include_directories: srcdir,
		dependencies: gtest
	)
)