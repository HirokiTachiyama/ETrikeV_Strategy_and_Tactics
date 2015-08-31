#include "common_definition.h"
#include "Tactics.h"
#include "Line_tracer.h"


extern Line_tracer* line_tracer;

void Tactics_FINISHED(Tactics* tactics) {
  Line_tracer_set_forward(line_tracer, 0);
  Line_tracer_set_turn(line_tracer, 0);
}


