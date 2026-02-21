#include <STYPES.H>
#include <SSYS.H>

#include <AMUC.H>

void entry(u32 argc, u8 ** argv, u8 ** env) {

    i32 rc;

    rc = main(argc, argv, env);

    exit(rc);
}
