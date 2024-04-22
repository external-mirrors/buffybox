/**
 * Copyright 2021 Johannes Marbach
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "themes.h"

#include "log.h"

#include <string.h>


/**
 * Static variables
 */

/* Breezy light (based on KDE Breeze color palette, see https://develop.kde.org/hig/style/color/default/) */
static const bbx_theme breezy_light = {
    .name = "breezy-light",
    .window = {
        .bg_color = 0xeff0f1
    },
    .header = {
        .bg_color = 0xfcfcfc,
        .border_width = 1,
        .border_color = 0xbdc3c7,
        .pad = 10,
        .gap = 10
    },
    .keyboard = {
        .bg_color = 0xfcfcfc,
        .border_width = 1,
        .border_color = 0xbdc3c7,
        .pad = 10,
        .gap = 10,
        .keys = {
            .border_width = 1,
            .corner_radius = 5,
            .key_char = {
                .normal = {
                    .fg_color = 0x232629,
                    .bg_color = 0xeff0f1,
                    .border_color = 0xbdc3c7
                },
                .pressed = {
                    .fg_color = 0x232629,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            },
            .key_non_char = {
                .normal = {
                    .fg_color = 0x232629,
                    .bg_color = 0xbdc3c7,
                    .border_color = 0x7f8c8d
                },
                .pressed = {
                    .fg_color = 0x232629,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            },
            .key_mod_act = {
                .normal = {
                    .fg_color = 0x232629,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                },
                .pressed = {
                    .fg_color = 0x232629,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            },
            .key_mod_inact = {
                .normal = {
                    .fg_color = 0x232629,
                    .bg_color = 0xbdc3c7,
                    .border_color = 0x7f8c8d
                },
                .pressed = {
                    .fg_color = 0x232629,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            }
        }
    },
    .button = {
        .border_width = 1,
        .corner_radius = 5,
        .pad = 5,
        .normal = {
            .fg_color = 0x232629,
            .bg_color = 0xeff0f1,
            .border_color = 0xbdc3c7
        },
        .pressed = {
            .fg_color = 0x232629,
            .bg_color = 0x3daee9,
            .border_color = 0x2980b9
        }
    },
    .textarea = {
        .fg_color = 0x232629,
        .bg_color = 0xfcfcfc,
        .border_width = 1,
        .border_color = 0xbdc3c7,
        .corner_radius = 5,
        .pad = 5,
        .placeholder_color = 0x7f8c8d,
        .cursor = {
            .width = 1,
            .color = 0x232629,
            .period = 700
        }
    },
    .dropdown = {
        .button = {
            .border_width = 1,
            .corner_radius = 5,
            .pad = 5,
            .normal = {
                .fg_color = 0x232629,
                .bg_color = 0xeff0f1,
                .border_color = 0xbdc3c7
            },
            .pressed = {
                .fg_color = 0x232629,
                .bg_color = 0x3daee9,
                .border_color = 0x2980b9
            }
        },
        .list = {
            .fg_color = 0x232629,
            .bg_color = 0xfcfcfc,
            .selection_fg_color = 0x232629,
            .selection_bg_color = 0x3daee9,
            .border_width = 1,
            .border_color = 0xbdc3c7,
            .corner_radius = 0,
            .pad = 0
        }
    },
    .label = {
        .fg_color = 0x232629
    },
    .msgbox = {
        .fg_color = 0x232629,
        .bg_color = 0xeff0f1,
        .border_width = 1,
        .border_color = 0xbdc3c7,
        .corner_radius = 0,
        .pad = 20,
        .gap = 20,
        .dimming = {
            .color = 0x232629,
            .opacity = 178
        }
    },
    .bar = {
        .border_width = 1,
        .border_color = 0x3daee9,
        .corner_radius = 5,
        .indicator = {
            .bg_color = 0x3daee9
        }
    }
};


/* Breezy dark (based on KDE Breeze Dark color palette, see https://develop.kde.org/hig/style/color/dark/) */
static const bbx_theme breezy_dark = {
    .name = "breezy-dark",
    .window = {
        .bg_color = 0x31363b
    },
    .header = {
        .bg_color = 0x232629,
        .border_width = 1,
        .border_color = 0x7f8c8d,
        .pad = 10,
        .gap = 10
    },
    .keyboard = {
        .bg_color = 0x232629,
        .border_width = 1,
        .border_color = 0x7f8c8d,
        .pad = 10,
        .gap = 10,
        .keys = {
            .border_width = 1,
            .corner_radius = 5,
            .key_char = {
                .normal = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x31363b,
                    .border_color = 0xbdc3c7
                },
                .pressed = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            },
            .key_non_char = {
                .normal = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x232629,
                    .border_color = 0x7f8c8d
                },
                .pressed = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            },
            .key_mod_act = {
                .normal = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                },
                .pressed = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            },
            .key_mod_inact = {
                .normal = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x232629,
                    .border_color = 0x7f8c8d
                },
                .pressed = {
                    .fg_color = 0xeff0f1,
                    .bg_color = 0x3daee9,
                    .border_color = 0x2980b9
                }
            }
        }
    },
    .button = {
        .border_width = 1,
        .corner_radius = 5,
        .pad = 5,
        .normal = {
            .fg_color = 0xeff0f1,
            .bg_color = 0x31363b,
            .border_color = 0xbdc3c7
        },
        .pressed = {
            .fg_color = 0xeff0f1,
            .bg_color = 0x3daee9,
            .border_color = 0x2980b9
        }
    },
    .textarea = {
        .fg_color = 0xeff0f1,
        .bg_color = 0x232629,
        .border_width = 1,
        .border_color = 0x7f8c8d,
        .corner_radius = 5,
        .pad = 5,
        .placeholder_color = 0x7f8c8d,
        .cursor = {
            .width = 1,
            .color = 0xeff0f1,
            .period = 700
        }
    },
    .dropdown = {
        .button = {
            .border_width = 1,
            .corner_radius = 5,
            .pad = 5,
            .normal = {
                .fg_color = 0xeff0f1,
                .bg_color = 0x31363b,
                .border_color = 0xbdc3c7
            },
            .pressed = {
                .fg_color = 0xeff0f1,
                .bg_color = 0x3daee9,
                .border_color = 0x2980b9
            }
        },
        .list = {
            .fg_color = 0xeff0f1,
            .bg_color = 0x232629,
            .selection_fg_color = 0x232629,
            .selection_bg_color = 0x3daee9,
            .border_width = 1,
            .border_color = 0x7f8c8d,
            .corner_radius = 0,
            .pad = 0
        }
    },
    .label = {
        .fg_color = 0xeff0f1
    },
    .msgbox = {
        .fg_color = 0xeff0f1,
        .bg_color = 0x31363b,
        .border_width = 1,
        .border_color = 0x3b4045,
        .corner_radius = 0,
        .pad = 20,
        .gap = 20,
        .dimming = {
            .color = 0x232629,
            .opacity = 178
        }
    },
    .bar = {
        .border_width = 1,
        .border_color = 0x3daee9,
        .corner_radius = 5,
        .indicator = {
            .bg_color = 0x3daee9
        }
    }
};

/* pmOS light (based on palette https://coolors.co/009900-395e66-db504a-e3b505-ebf5ee) */
static const bbx_theme pmos_light = {
    .name = "pmos-light",
    .window = {
        .bg_color = 0xf2f7f8,
    },
    .header = {
        .bg_color = 0xf2f7f8,
        .border_width = 0,
        .border_color = 0xf2f7f8,
        .pad = 20,
        .gap = 10
    },
    .keyboard = {
        .bg_color = 0xd8e6e9,
        .border_width = 2,
        .border_color = 0x97bcc4,
        .pad = 20,
        .gap = 10,
        .keys = {
            .border_width = 1,
            .corner_radius = 3,
            .key_char = {
                .normal = {
                    .fg_color = 0x070c0d,
                    .bg_color = 0xd8e6e9,
                    .border_color = 0x97bcc4
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            },
            .key_non_char = {
                .normal = {
                    .fg_color = 0x070c0d,
                    .bg_color = 0xbed5da,
                    .border_color = 0xb1cdd3
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            },
            .key_mod_act = {
                .normal = {
                    .fg_color = 0x009900,
                    .bg_color = 0xbed5da,
                    .border_color = 0x009900
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            },
            .key_mod_inact = {
                .normal = {
                    .fg_color = 0x070c0d,
                    .bg_color = 0xbed5da,
                    .border_color = 0xb1cdd3
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            }
        }
    },
    .button = {
        .border_width = 1,
        .corner_radius = 3,
        .pad = 8,
        .normal = {
            .fg_color = 0x070c0d,
            .bg_color = 0xbed5da,
            .border_color = 0xb1cdd3
        },
        .pressed = {
            .fg_color = 0xf2f7f8,
            .bg_color = 0x009900,
            .border_color = 0x009900
        }
    },
    .textarea = {
        .fg_color = 0x070c0d,
        .bg_color = 0xebffeb,
        .border_width = 1,
        .border_color = 0x009900,
        .corner_radius = 3,
        .pad = 8,
        .placeholder_color = 0x009900,
        .cursor = {
            .width = 2,
            .color = 0x009900,
            .period = 700
        }
    },
    .dropdown = {
        .button = {
            .border_width = 1,
            .corner_radius = 3,
            .pad = 8,
            .normal = {
                .fg_color = 0x070c0d,
                .bg_color = 0xbed5da,
                .border_color = 0xb1cdd3
            },
            .pressed = {
                .fg_color = 0xf2f7f8,
                .bg_color = 0x009900,
                .border_color = 0x009900
            }
        },
        .list = {
            .fg_color = 0x070c0d,
            .bg_color = 0xd8e6e9,
            .selection_fg_color = 0xf2f7f8,
            .selection_bg_color = 0x009900,
            .border_width = 1,
            .border_color = 0x97bcc4,
            .corner_radius = 0,
            .pad = 8
        }
    },
    .label = {
        .fg_color = 0x070c0d
    },
    .msgbox = {
        .fg_color = 0x070c0d,
        .bg_color = 0xd8e6e9,
        .border_width = 1,
        .border_color = 0x97bcc4,
        .corner_radius = 3,
        .pad = 20,
        .gap = 20,
        .dimming = {
            .color = 0x070c0d,
            .opacity = 225
        }
    },
    .bar = {
        .border_width = 1,
        .border_color = 0x009900,
        .corner_radius = 3,
        .indicator = {
            .bg_color = 0x009900
        }
    }
};

/* pmOS dark (based on palette https://coolors.co/009900-395e66-db504a-e3b505-ebf5ee) */
static const bbx_theme pmos_dark = {
    .name = "pmos-dark",
    .window = {
        .bg_color = 0x070c0d
    },
    .header = {
        .bg_color = 0x070c0d,
        .border_width = 0,
        .border_color = 0x070c0d,
        .pad = 20,
        .gap = 10
    },
    .keyboard = {
        .bg_color = 0x162427,
        .border_width = 2,
        .border_color = 0x395e66,
        .pad = 20,
        .gap = 10,
        .keys = {
            .border_width = 1,
            .corner_radius = 3,
            .key_char = {
                .normal = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x162427,
                    .border_color = 0x395e66
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            },
            .key_non_char = {
                .normal = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x253c41,
                    .border_color = 0x2c484e
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            },
            .key_mod_act = {
                .normal = {
                    .fg_color = 0x009900,
                    .bg_color = 0x253c41,
                    .border_color = 0x009900
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            },
            .key_mod_inact = {
                .normal = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x253c41,
                    .border_color = 0x2c484e
                },
                .pressed = {
                    .fg_color = 0xf2f7f8,
                    .bg_color = 0x009900,
                    .border_color = 0x009900
                }
            }
        }
    },
    .button = {
        .border_width = 1,
        .corner_radius = 3,
        .pad = 8,
        .normal = {
            .fg_color = 0xf2f7f8,
            .bg_color = 0x253c41,
            .border_color = 0x2c484e
        },
        .pressed = {
            .fg_color = 0xf2f7f8,
            .bg_color = 0x009900,
            .border_color = 0x009900
        }
    },
    .textarea = {
        .fg_color = 0xf2f7f8,
        .bg_color = 0x002900,
        .border_width = 1,
        .border_color = 0x009900,
        .corner_radius = 3,
        .pad = 8,
        .placeholder_color = 0x009900,
        .cursor = {
            .width = 2,
            .color = 0x009900,
            .period = 700
        }
    },
    .dropdown = {
        .button = {
            .border_width = 1,
            .corner_radius = 3,
            .pad = 8,
            .normal = {
                .fg_color = 0xf2f7f8,
                .bg_color = 0x253c41,
                .border_color = 0x2c484e
            },
            .pressed = {
                .fg_color = 0xf2f7f8,
                .bg_color = 0x009900,
                .border_color = 0x009900
            }
        },
        .list = {
            .fg_color = 0xf2f7f8,
            .bg_color = 0x162427,
            .selection_fg_color = 0xf2f7f8,
            .selection_bg_color = 0x009900,
            .border_width = 1,
            .border_color = 0x395e66,
            .corner_radius = 0,
            .pad = 8
        }
    },
    .label = {
        .fg_color = 0xf2f7f8,
    },
    .msgbox = {
        .fg_color = 0xf2f7f8,
        .bg_color = 0x162427,
        .border_width = 1,
        .border_color = 0x395e66,
        .corner_radius = 3,
        .pad = 20,
        .gap = 20,
        .dimming = {
            .color = 0x070c0d,
            .opacity = 225
        }
    },
    .bar = {
        .border_width = 1,
        .border_color = 0x009900,
        .corner_radius = 3,
        .indicator = {
            .bg_color = 0x009900
        }
    }
};

static const bbx_theme adwaita_dark = {
    .name = "adwaita-dark",
    .window = {
        .bg_color = 0x151515
    },
    .header = {
        .bg_color = 0x242424,
        .border_width = 0,
        .border_color = 0x242424,
        .pad = 10,
        .gap = 10
    },
    .keyboard = {
        .bg_color = 0x242424,
        .border_width = 2,
        .border_color = 0x242424,
        .pad = 20,
        .gap = 10,
        .keys = {
            .border_width = 1,
            .corner_radius = 5,
            .key_char = {
                .normal = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x464448,
                    .border_color = 0x464448
                },
                .pressed = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x747077,
                    .border_color = 0x747077
                }
            },
            .key_non_char = {
                .normal = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x3A3A3A,
                    .border_color = 0x3A3A3A
                },
                .pressed = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x666666,
                    .border_color = 0x666666
                }
            },
            .key_mod_act = {
                .normal = {
                    .fg_color = 0x1E1E1E,
                    .bg_color = 0x747077,
                    .border_color = 0x747077
                },
                .pressed = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x464448,
                    .border_color = 0x464448
                }
            },
            .key_mod_inact = {
                .normal = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x3A3A3A,
                    .border_color = 0x3A3A3A
                },
                .pressed = {
                    .fg_color = 0xDEDDDA,
                    .bg_color = 0x3A3A3A,
                    .border_color = 0x3A3A3A
                }
            }
        }
    },
    .button = {
        .border_width = 1,
        .corner_radius = 5,
        .pad = 8,
        .normal = {
            .fg_color = 0xDEDDDA,
            .bg_color = 0x3A3A3A,
            .border_color = 0x3A3A3A
        },
        .pressed = {
            .fg_color = 0xDEDDDA,
            .bg_color = 0x666666,
            .border_color = 0x666666
        }
    },
    .textarea = {
        .fg_color = 0xDEDDDA,
        .bg_color = 0x282828,
        .border_width = 1,
        .border_color = 0x1C71D8,
        .corner_radius = 10,
        .pad = 8,
        .placeholder_color = 0x1C71D8,
        .cursor = {
            .width = 2,
            .color = 0xDEDDDA,
            .period = 700
        }
    },
    .dropdown = {
        .button = {
            .border_width = 1,
            .corner_radius = 5,
            .pad = 8,
            .normal = {
                .fg_color = 0xDEDDDA,
                .bg_color = 0x3A3A3A,
                .border_color = 0x3A3A3A
            },
            .pressed = {
                .fg_color = 0xDEDDDA,
                .bg_color = 0x666666,
                .border_color = 0x666666
            }
        },
        .list = {
            .fg_color = 0xDEDDDA,
            .bg_color = 0x383838,
            .selection_fg_color = 0xDEDDDA,
            .selection_bg_color = 0x5E5E5E,
            .border_width = 1,
            .border_color = 0x383838,
            .corner_radius = 5,
            .pad = 8
        }
    },
    .label = {
        .fg_color = 0xDEDDDA,
    },
    .msgbox = {
        .fg_color = 0xDEDDDA,
        .bg_color = 0x383838,
        .border_width = 1,
        .border_color = 0x383838,
        .corner_radius = 7,
        .pad = 20,
        .gap = 20,
        .dimming = {
            .color = 0x151515,
            .opacity = 225
        }
    },
    .bar = {
        .border_width = 1,
        .border_color = 0x1C71D8,
        .corner_radius = 5,
        .indicator = {
            .bg_color = 0x1C71D8
        }
    }
};

/**
 * Public interface
 */

const int bbx_themes_num_themes = 5;
const bbx_theme *bbx_themes_themes[] = {
    &breezy_light,
    &breezy_dark,
    &pmos_light,
    &pmos_dark,
    &adwaita_dark,
};

bbx_themes_theme_id_t bbx_themes_find_theme_with_name(const char *name) {
    for (int i = 0; i < bbx_themes_num_themes; ++i) {
        if (strcmp(bbx_themes_themes[i]->name, name) == 0) {
            bbx_log(BBX_LOG_LEVEL_VERBOSE, "Found theme: %s\n", name);
            return i;
        }
    }
    bbx_log(BBX_LOG_LEVEL_WARNING, "Theme %s not found\n", name);
    return BBX_THEMES_THEME_NONE;
}
