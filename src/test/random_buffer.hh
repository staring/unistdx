#ifndef TEST_RANDOM_BUFFER_HH
#define TEST_RANDOM_BUFFER_HH

#include <random>
#include <sstream>

#include <unistdx/base/log_message>
#include <unistdx/base/streambuf_traits>

namespace test {

	/// Stream buffer that writes/reads random number of bytes
	/// in one sputn/sgetn call.
	class random_buffer: public std::stringbuf {

	private:
		typedef std::random_device engine_type;
		typedef std::uniform_int_distribution<std::streamsize>
			dist_type;

	private:
		engine_type _engine;

	protected:
		using std::stringbuf::char_type;

	public:
		random_buffer() = default;

		random_buffer(const random_buffer&) = delete;

		~random_buffer() = default;

		random_buffer(random_buffer&& rhs):
		std::stringbuf(std::move(rhs)),
		_engine()
		{}

		random_buffer&
		operator=(random_buffer&& rhs) {
			std::stringbuf::operator=(std::move(rhs));
			return *this;
		}

		inline bool
		dirty() const noexcept {
			return this->pptr() != this->pbase();
		}

	protected:

		std::streamsize
		xsgetn(char_type* s, std::streamsize n) override {
			if (n == 0) {
				return 0;
			}
			dist_type dist(0, std::min(std::streamsize(65536), n));
			std::streamsize nbytes = dist(this->_engine);
			if (nbytes == 0) {
				return -1;
			}
			sys::log_message("tst", "get [_/_]", nbytes, n);
			return std::stringbuf::xsgetn(s, nbytes);
		}

		std::streamsize
		xsputn(const char_type* s, std::streamsize n) override {
			if (n == 0) {
				return 0;
			}
			dist_type dist(0, std::min(std::streamsize(65536), n));
			std::streamsize nbytes = dist(this->_engine);
			if (nbytes == 0) {
				return -1;
			}
			sys::log_message("tst", "put [_/_]", nbytes, n);
			return std::stringbuf::xsputn(s, nbytes);
		}

	};

}

namespace sys {

	template<>
	class streambuf_traits<test::random_buffer> {

	public:
		typedef char char_type;
		typedef test::random_buffer T;

		inline static std::streamsize
		write(T& buf, const char_type* s, std::streamsize n) {
			return buf.sputn(s, n);
		}

		inline static std::streamsize
		read(T& buf, char_type* s, std::streamsize n) {
			return buf.sgetn(s, n);
		}

		inline static bool
		is_blocking(const T&) {
			return false;
		}

		inline static std::streamsize
		in_avail(T& buf) {
			return buf.in_avail();
		}

	};


}

#endif // vim:filetype=cpp
