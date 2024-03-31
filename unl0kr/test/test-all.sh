#!/bin/bash

root=$(dirname "${BASH_SOURCE[0]}")

"$root/test-with-drm.sh"
"$root/test-without-drm.sh"
