#!/bin/bash

version=$(git describe --exact-match --tags HEAD || git rev-parse HEAD)
archive=buffybox-${version}.tar.gz

echo "Archiving repository with submodules..."
git-archive-all ${archive}

echo "Uploading archive to GitLab..."
response=$(curl --request POST \
    --header "PRIVATE-TOKEN: $1" \
    --form "file=@${archive}" \
    "https://gitlab.postmarketos.org/api/v4/projects/172/uploads")

path=$(echo "${response}" | jq -r .full_path)

echo "${response}"
echo
echo "https://gitlab.postmarketos.org${path}"

rm -f "${archive}"
