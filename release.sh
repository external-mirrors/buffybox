#!/bin/bash

if [[ -z "$1" ]]; then
    echo "Error: No version number provided"
    exit 1
fi

if [[ -z "$2" ]]; then
    echo "Error: No GitLab API token provided"
    exit 1
fi

if ! grep "## $1" CHANGELOG.md > /dev/null; then
    echo "Error: No entry in CHANGELOG.md for version $1"
    exit 1
fi

if ! grep -E "^\s*version:\s*'$1'" buffyboard/meson.build > /dev/null; then
    echo "Error: Version $1 differs from version in buffyboard/meson.build"
    exit 1
fi

if ! grep -E "^\s*version:\s*'$1'" unl0kr/meson.build > /dev/null; then
    echo "Error: Version $1 differs from version in unl0kr/meson.build"
    exit 1
fi

git tag "$1"
git push --tags

./archive.sh "$2"

echo "Now create a release for https://gitlab.com/postmarketOS/buffybox/-/tags/$1 and attach the uploaded archive"