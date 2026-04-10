LIBS = -lqui -lbobuiest -lqui
CONFIG -= link_prl
JOINEDLIBS = $$join(LIBS, "_")

!contains(JOINEDLIBS, -lqui_-lbobuiest_-lqui) {
   message("FAILED: duplibs")
}
