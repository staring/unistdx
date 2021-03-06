#include "websocket"

#include <algorithm>
#include <limits>
#include <ostream>

#include <unistdx/net/bytes>

sys::websocket_frame::length64_type
sys::websocket_frame::payload_size() const noexcept {
	switch (this->_hdr.len) {
	case length16_tag: return to_host_format<length16_type>(this->_hdr.extlen);
	case length64_tag: {
		bytes<length64_type> tmp(this->base(), sizeof(length64_type));
		tmp.to_host_format();
		return tmp.value();
	}
	default:
		return this->_hdr.len;
	}
}

void
sys::websocket_frame::payload_size(length64_type rhs) noexcept {
	if (rhs <= 125) {
		this->_hdr.len = rhs;
	} else if (rhs > 125 && rhs <= std::numeric_limits<length16_type>::max()) {
		this->_hdr.len = length16_tag;
		bytes<length16_type> raw = rhs;
		raw.to_network_format();
		this->_hdr.extlen = raw;
	} else {
		this->_hdr.len = length64_tag;
		bytes<length64_type> raw = rhs;
		raw.to_network_format();
		std::copy(raw.begin(), raw.end(), this->base());
	}
}

sys::websocket_frame::mask_type
sys::websocket_frame::mask() const noexcept {
	switch (this->_hdr.len) {
	case length16_tag:
		return this->_hdr.extlen2;
	case length64_tag:
		return bytes<mask_type>(this->end() - mask_size, this->end());
	default:
		return bytes<mask_type>(this->base(), mask_size);
	}
}

void
sys::websocket_frame::mask(mask_type rhs) noexcept {
	if (rhs == 0) {
		this->_hdr.maskbit = 0;
	} else {
		this->_hdr.maskbit = 1;
		switch (this->_hdr.len) {
		case length16_tag: this->_hdr.extlen2 = rhs; break;
		case length64_tag: {
			bytes<mask_type> tmp = rhs;
			std::copy(
				tmp.begin(),
				tmp.end(),
				this->end() - mask_size
			);
			break;
		}
		default: {
			bytes<mask_type> tmp = rhs;
			std::copy(tmp.begin(), tmp.end(), this->base());
			break;
		}
		}
	}
}

void
sys::websocket_frame::mask_payload(char* first, char* last) const noexcept {
	if (this->is_masked()) {
		bytes<mask_type> m = this->mask();
		size_t i = 0;
		while (first != last) {
			*first ^= m[i%4];
			++first;
			++i;
		}
	}
}

void
sys::websocket_frame::clear() noexcept {
	std::fill_n(this->_bytes, sizeof(this->_bytes), '\0');
}

std::ostream&
sys::operator<<(std::ostream& out, const websocket_frame& rhs) {
	typedef websocket_frame::mask_type mask_type;
	std::ostream::sentry s(out);
	if (s) {
		out << "fin=" << rhs._hdr.fin << ','
		    << "rsv1=" << rhs._hdr.rsv1 << ','
		    << "rsv2=" << rhs._hdr.rsv2 << ','
		    << "rsv3=" << rhs._hdr.rsv3 << ','
		    << "opcode=" << rhs._hdr.opcode << ','
		    << "maskbit=" << rhs._hdr.maskbit << ','
		    << "len=" << rhs._hdr.len << ','
		    << "mask=" << bytes<mask_type>(rhs.mask()) << ','
		    << "payload_size=" << rhs.payload_size() << ','
		    << "header_size=" << rhs.size();
	}
	return out;
}
