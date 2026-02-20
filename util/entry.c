#include <util/types.h>
#include <util/sys.h>

#include <muc.h>

void entry(u32 argc, u8 ** argv, u8 ** env) {

    i32 rc;

    rc = main(argc, argv, env);

    exit(rc);
}
