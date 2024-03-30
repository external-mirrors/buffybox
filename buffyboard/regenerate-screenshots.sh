#!/bin/bash

# Change this depending on what device you're generating the screenshots on
fb_res=1920x1080
fb_depth=8
fb_format=rgba

executable=$1
outdir=screenshots
config=buffyboard-screenshots.conf

themes=(
    breezy-light
    breezy-dark
    pmos-light
    pmos-dark
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
[theme]
default=$1

[input]
pointer=false
touchscreen=false
EOF
}

# Hide cursor
echo -e '\033[?25l'

function clean_up() {
    rm -f $config

    # Show cursor
    echo -e '\033[?25h'
}

trap clean_up EXIT

rm -rf "$outdir"
mkdir "$outdir"

readme="# Buffyboard themes"$'\n'

for theme in ${themes[@]}; do
    write_config $theme

    readme="$readme"$'\n'"## $theme"$'\n\n'
    
    for res in ${resolutions[@]}; do
        $executable -g $res -C $config > /dev/null 2>&1 &
        pid=$!

        sleep 3 # Wait for UI to render

        cat /dev/fb0 > "$outdir/$theme-$res"
        convert -size $fb_res \
            -depth $fb_depth \
            screenshot-background.png \
            $fb_format:"$outdir/$theme-$res" \
            -crop $res+0+0 \
            -gravity NorthWest \
            -composite \
            "$outdir/$theme-$res.png"

        rm "$outdir/$theme-$res"
        kill -15 $pid

        readme="$readme<img src=\"$theme-$res.png\" alt=\"$res\" height=\"300\"/>"$'\n'

        sleep 1 # Delay to prevent terminal mode set / reset interference
    done
done

echo -n "$readme" > "$outdir/README.md"
