#!/bin/sh
cd shared || exit 1
yarn atdgen -t Counter.atd
# yarn atdgen -j Counter.atd
yarn atdgen -bs Counter.atd

# refmt Counter_t.mli > Counter_t.rei
# refmt Counter_t.ml > Counter_t.re
# refmt Counter_j.mli > Counter_j.rei
# refmt Counter_j.ml > Counter_j.re
# refmt Counter_bs.mli > Counter_bs.rei
# refmt Counter_bs.ml > Counter_bs.re

# rm Counter_t.mli Counter_t.ml Counter_j.mli Counter_j.ml Counter_bs.mli Counter_bs.ml
