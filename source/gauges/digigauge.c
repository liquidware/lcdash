#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <avr/eeprom.h>
#include <math.h>

#include "../t6963.h"
#include "../gui.h"
#include "../touchscreen.h"
#include "../gauge.h"
#include "../string_manip.h"
#include "../LCDash.h"
#include "../bitops.h"
#include "../menu.h"

#include "digiGauge.h"


//////////////////////////////////////////////////////////////////
void gauge_digiGauge_refresh(DIGI_GAUGE * g)
{

/*! If there is a change... */
if (g->value.curr_val != g->prev_val)
	{
	char out[7];
	
	gauge_convert_units(&g->value.curr_val,g->value.units,g->value.user_units); //change units
	gauge_get_digits(&g->value.curr_val,g->value.param_id, out); //get the precision

	gui_largeNumS(out,g->bounds.left,g->bounds.top,0); 			//!< Display the value

	g->prev_val = g->value.curr_val; 				//!< Our current value is now our previous
	}

}

////////////////////////////////////////////////////////////////////
void gauge_digiGauge_update(DIGI_GAUGE * g)
{

	g->prev_val = -1;
	g->value.curr_val = 0;
	gui_puts(g->value.label,g->bounds.left,g->bounds.top-9,0); //!< draw the label

	gauge_digiGauge_refresh(g); //!< refresh the gauge with it's new values
	
}
