#ifndef MSD_CS6015_PROJECT_CMDLINE_HPP
#define MSD_CS6015_PROJECT_CMDLINE_HPP

#endif //MSD_CS6015_PROJECT_CMDLINE_HPP

typedef enum {
    do_nothing,
    do_interp,
    do_print,
    do_pretty_print,
} run_mode_t;

run_mode_t use_arguments( int argc, char **argv );
