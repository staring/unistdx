#include "file_mode"

#include <ostream>

std::ostream&
sys::operator<<(std::ostream& out, const file_mode& rhs) {
	const mode_type s = rhs.special();
	const mode_type usr = rhs.user();
	const mode_type grp = rhs.group();
	const mode_type oth = rhs.other();
	const char bits[] = {
		((s & sys::file_mode::setuid) ? 'u' : '-'),
		((s & sys::file_mode::setgid) ? 'g' : '-'),
		((s & sys::file_mode::sticky) ? 't' : '-'),
		((usr & sys::file_mode::user_r) ? 'r' : '-'),
		((usr & sys::file_mode::user_w) ? 'w' : '-'),
		((usr & sys::file_mode::user_x) ? 'x' : '-'),
		((grp & sys::file_mode::group_r) ? 'r' : '-'),
		((grp & sys::file_mode::group_w) ? 'w' : '-'),
		((grp & sys::file_mode::group_x) ? 'x' : '-'),
		((oth & sys::file_mode::other_r) ? 'r' : '-'),
		((oth & sys::file_mode::other_w) ? 'w' : '-'),
		((oth & sys::file_mode::other_x) ? 'x' : '-'),
		0
	};
	return out << bits;
}

