progscdoc = depscdoc.get_variable(pkgconfig: 'scdoc')

foreach file : [
        'buffyboard.1',
        'buffyboard.conf.5'
    ]

    section = file.split('.')[-1]

    custom_target(file,
        command: progscdoc,
        feed: true,
        capture: true,
        input: file + '.scd',
        output: file,
        install: true,
        install_dir: get_option('mandir') / 'man' + section,
        install_tag: 'buffyboard'
    )
endforeach

foreach file : [
        'unl0kr.1',
        'unl0kr.conf.5'
    ]

    section = file.split('.')[-1]

    custom_target(file,
        command: progscdoc,
        feed: true,
        capture: true,
        input: file + '.scd',
        output: file,
        install: true,
        install_dir: get_option('mandir') / 'man' + section,
        install_tag: 'unl0kr'
    )
endforeach
