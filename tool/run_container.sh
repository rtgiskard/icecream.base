#!/bin/bash

mfunc="$HOME/src/header/rc.sh"
[ -f "$mfunc" ] && . "$mfunc" && unset mfunc

D_SRC="$(dirname "$0")/../"
D_SRC="$(realpath $D_SRC)"

m_compose run --rm -v "$D_SRC:/cc/icecream" tool.dev bash
