
#include "Defines.h"
#include "Notes.h"

#define BPM			144					// Beats Per Minute
#define SPS			44100				// Samples Per Second
#define BPS			((60*SPS)/BPM)		// Beats Per Sample


inline void musicInit();
inline void musicStep();
