#ifndef SYS_FILDESBUF_HH
#define SYS_FILDESBUF_HH

#include <vector>
#include <cassert>

#include <stdx/packetbuf.hh>
#include <stdx/streambuf.hh>

#include "fildes.hh"
#include "system.hh"

namespace sys {

	template<class Ch, class Tr=std::char_traits<Ch>, class Fd=sys::fildes>
	class basic_fildesbuf: public stdx::basic_packetbuf<Ch,Tr> {

		typedef stdx::streambuf_traits<Fd> streambuf_traits_type;

	public:

		typedef stdx::basic_packetbuf<Ch,Tr> base_type;
		using base_type::gptr;
		using base_type::eback;
		using base_type::egptr;
		using base_type::pptr;
		using base_type::pbase;
		using base_type::epptr;
		using base_type::setg;
		using base_type::setp;
		using base_type::gbump;
		using base_type::pbump;
		using typename base_type::int_type;
		using typename base_type::traits_type;
		using typename base_type::char_type;
		using typename base_type::pos_type;
		using typename base_type::off_type;
		typedef std::ios_base::openmode openmode;
		typedef std::ios_base::seekdir seekdir;
		typedef Fd fd_type;
		typedef typename std::vector<char_type>::size_type size_type;
		typedef typename std::make_signed<size_type>::type signed_size_type;

		inline
		basic_fildesbuf():
		basic_fildesbuf(std::move(fd_type()), page_size())
		{}

		inline explicit
		basic_fildesbuf(fd_type&& fd):
		basic_fildesbuf(std::move(fd), page_size())
		{}

		inline
		basic_fildesbuf(fd_type&& fd, size_type bufsize):
		basic_fildesbuf(std::move(fd), bufsize, bufsize)
		{}

		inline
		basic_fildesbuf(fd_type&& fd, size_type gbufsize, size_type pbufsize):
		_fd(std::move(fd)), _gbuf(gbufsize), _pbuf(pbufsize)
		{
			char_type* end = _gbuf.data();
			setg(end, end, end);
			char_type* beg = _pbuf.data();
			setp(beg, beg + _pbuf.size());
		}

		basic_fildesbuf(basic_fildesbuf&& rhs) = default;

		int_type
		underflow() override {
			assert(gptr() == egptr());
			setg(eback(), eback(), eback());
			return do_fill() == 0
				? traits_type::eof()
				: traits_type::to_int_type(*gptr());
		}

		int_type
		overflow(int_type c) override {
			assert(pptr() == epptr());
			if (c != traits_type::eof()) {
				pgrow();
				if (pptr() != epptr()) {
					*pptr() = c;
					pbump(1);
				}
			} else {
				do_flush();
			}
			return c;
		}

		int
		sync() override {
			int ret = 0;
			if (has_pbuf()) {
				ret |= do_flush()==-1 ? -1 : 0;
			}
			if (has_gbuf()) {
				ret |= do_fill()==-1 ? -1 : 0;
			}
			return ret;
		}

		pos_type
		seekoff(off_type off, seekdir way, openmode which) override {
			pos_type ret(off_type(-1));
			if (way == std::ios_base::beg) {
				ret = seekpos(off, which);
			}
			if (way == std::ios_base::cur) {
				const pos_type pos = which & std::ios_base::in
					? static_cast<pos_type>(gptr() - eback())
					: static_cast<pos_type>(pptr() - pbase());
				ret = off == 0 ? pos : seekpos(pos + off, which);
			}
			if (way == std::ios_base::end) {
				const pos_type pos = which & std::ios_base::in
					? static_cast<pos_type>(egptr() - eback())
					: static_cast<pos_type>(epptr() - pbase());
				ret = seekpos(pos + off, which);
			}
			return ret;
		}

		pos_type
		seekpos(pos_type pos, openmode mode) override {
			pos_type ret(off_type(-1));
			if (mode & std::ios_base::in) {
				const std::streamsize size = egptr() - eback();
				if (pos >= 0 && pos <= size) {
					setg(eback(), eback()+pos, egptr());
					ret = pos;
				}
			}
			if (mode & std::ios_base::out) {
				if (pos >= 0 && pos <= psize()) {
					setp(pbase(), epptr());
					pbump(pos);
					ret = pos;
				}
			}
			return ret;
		}

		std::streamsize
		showmanyc() override {
			return streambuf_traits_type::in_avail(_fd);
		}

		inline void
		setfd(fd_type&& rhs) {
			this->_fd = std::move(rhs);
		}

		inline const fd_type&
		fd() const {
			return this->_fd;
		}

		inline fd_type&
		fd() {
			return this->_fd;
		}

		inline void
		close() {
			this->_fd.close();
		}

	private:

		inline std::streamsize
		do_fill() {
			// TODO 2016-03-09 this is not optimal solution,
			// but i don't know a better alternative
			const std::streamsize old_egptr_offset = egptr() - eback();
			char_type* first = egptr();
			char_type* last = glast();
			std::streamsize n = 0;
			while ((n = streambuf_traits_type::read(_fd, first, last-first)) > 0) {
				first += n;
				setg(eback(), gptr(), first);
				if (first == last) {
					ggrow();
					first = egptr();
					last = glast();
				}
			}
			return first - (eback() + old_egptr_offset);
		}

		inline std::streamsize
		do_flush() {
			const std::streamsize m = pptr() - pbase();
			if (m == 0) return 0;
			const std::streamsize n = streambuf_traits_type::write(this->_fd, pbase(), m);
			const bool success = n==m;
			if (success) {
				setp(pfirst(), plast());
			} else if (n > 0) {
				setp(pbase()+n, epptr());
				pbump(m-n);
			}
			return n==-1 ? 0 : n;
		}

	protected:

		inline char_type*
		gfirst() noexcept {
			return this->_gbuf.data();
		}

		inline char_type*
		glast() noexcept {
			return this->_gbuf.data() + this->_gbuf.size();
		}

		inline void
		rebase() noexcept {
			char_type* base = this->_gbuf.data();
			setg(
				base,
				base + (gptr()-eback()),
				base + (egptr()-eback())
			);
		}

		inline bool
		has_gbuf() const noexcept {
			return !this->_gbuf.empty();
		}

		inline void
		ggrow() {
			this->_gbuf.resize(_gbuf.size() * 2);
			rebase();
		}

		inline char_type*
		pfirst() noexcept {
			return this->_pbuf.data();
		}

		inline char_type*
		plast() noexcept {
			return this->_pbuf.data() + this->_pbuf.size();
		}

		inline bool
		has_pbuf() const noexcept {
			return !this->_pbuf.empty();
		}

		inline void
		pgrow() {
			const pos_type pptr_offset = pptr() - pbase();
			const pos_type pbase_offset = pbase() - pfirst();
			this->_pbuf.resize(_pbuf.size() * 2);
			setp(pfirst() + pbase_offset, plast());
			pbump(pbase_offset + pptr_offset);
		}

		inline signed_size_type
		psize() const {
			return static_cast<signed_size_type>(this->_pbuf.size());
		}

		inline signed_size_type
		gsize() const {
			return static_cast<signed_size_type>(this->_gbuf.size());
		}

		fd_type _fd;
		std::vector<char_type> _gbuf;
		std::vector<char_type> _pbuf;
	};

	typedef basic_fildesbuf<char> fildesbuf;

}

#endif // SYS_FILDESBUF_HH
