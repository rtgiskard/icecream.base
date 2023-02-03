#!/bin/bash

install_system_libs() {
	os_id="$(grep "^ID=" /etc/os-release|cut -d= -f 2)"

	case "$os_id" in
	"fedora")
		pkg-config --exists fmt || \
			dnf install -y --nodocs fmt-devel spdlog-devel
		;;
	esac
}

# wrap file is already included
install_subprojects() {
	local prj_root="$(dirname $0)/.."
	[ -f "${prj_root}/meson.build" ] || return
	pushd "$prj_root" >/dev/null

	subd="./subprojects"
	items="spdlog argparse tomlplusplus"

	[ -d "$subd" ] || mkdir -p "$subd"
	for item in $items; do
		[ ! -f "${subd}/${item}.wrap" ] || continue

		echo "meson wrap install $item .."
		meson wrap install $item
	done

	popd >/dev/null
}

install_system_libs
