#include "global.h"
catalog_manager cm;
buffer_manager bm;
record_manager rm(&bm);
index_manager im(&bm);
API api(&cm, &rm, &im, &bm);
interpreter interp(&cm, &api);
