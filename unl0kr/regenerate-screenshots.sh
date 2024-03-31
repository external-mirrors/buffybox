#!/bin/bash

# Change this depending on what device you're generating the screenshots on
fb_res=1920x1080

executable=$1
outdir=screenshots
config=unl0kr-screenshots.conf

themes=(
    breezy-light
    breezy-dark
    pmos-light
    pmos-dark
    nord-light
    nord-dark
)

resolutions=(
    # Nokia N900
    480x800
    800x480
    # Samsung Galaxy A3 2015
    540x960
    960x540
    # Samsung Galaxy Tab A 8.0 2015
    768x1024
    1024x768
    # Pine64 PineTab (landscape)
    1280x800
    # Pine64 PinePhone (landscape)
    1440x720
    # BQ Aquaris X Pro (landscape)
    1920x1080
)

if [[ ! -f $executable || ! -x $executable ]]; then
    echo "Error: Could not find executable at $executable" 1>&2
    exit 1
fi

function write_config() {
    cat << EOF > $config
[general]
animations=true

[keyboard]
autohide=false
layout=de
popovers=true

[textarea]
obscured=true

[theme]
default=$1

[input]
keyboard=true
pointer=false
touchscreen=false
EOF
}

function nuke_config() {
    rm -f $config
}

trap "nuke_config" EXIT

rm -rf "$outdir"
mkdir "$outdir"

readme="# Unl0kr themes"$'\n'

for theme in ${themes[@]}; do
    write_config $theme

    readme="$readme"$'\n'"## $theme"$'\n\n'
    
    for res in ${resolutions[@]}; do
        $executable -g $res -C $config &
        pid=$!

        sleep 3 # Wait for UI to render

        ../../fbcat/fbcat /dev/fb0 > "$outdir/$theme-$res.ppm"
        convert -size $fb_res "$outdir/$theme-$res.ppm" -crop $res+0+0 "$outdir/$theme-$res.png"
        rm "$outdir/$theme-$res.ppm"
        kill -15 $pid

        readme="$readme<img src=\"$theme-$res.png\" alt=\"$res\" height=\"300\"/>"$'\n'

        sleep 1 # Delay to prevent terminal mode set / reset interference
    done
done

echo -n "$readme" > "$outdir/README.md"
