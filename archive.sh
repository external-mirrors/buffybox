#!/bin/bash

version=$(git describe --exact-match --tags HEAD || git rev-parse HEAD)
archive=unl0kr-${version}.tar.gz

echo "Archiving repository with submodules..."
git-archive-all ${archive}

echo "Uploading archive to GitLab..."
response=$(curl --request POST \
    --header "PRIVATE-TOKEN: $1" \
    --form "file=@${archive}" \
    "https://gitlab.com/api/v4/projects/29322771/uploads")

path=$(echo "${response}" | jq -r .full_path)

echo "${response}"
echo
echo "https://gitlab.com/${path}"

rm -f "${archive}"
