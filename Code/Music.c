
#include "Music.h"

tU8		CurNote;
bool	CurWave = 0;				// Start playing as square wave

tU8* WaveData[3];
tU32 WaveLen [3];


WAVEHDR			WaveHeader[2];
HWAVEOUT		hWaveOut;          // Handle to sound card output
WAVEFORMATEX	WaveFormat;        // The sound format



struct {

	const tU16	Freq;	// 440 = A4
	const tU8	Len;	// 1/4th note etc

} TetrisNotes[] = {

		{E5,4},		{B4,8},		{C5,8},		{D5,4},		{C5,8},		{B4,8},
		{A4,4},		{A4,8},		{C5,8},		{E5,4},		{D5,8},		{C5,8},
		{B4,4},		{C5,8},		{D5,4},		{E5,4},
		{C5,4},		{A4,4},		{A4,8},		{A4,8},		{B4,8},		{C5,8},

		{D5,4},		{F5,8},		{A5,4},		{G5,8},		{F5,8},
		{E5,4},		{C5,8},		{E5,4},		{D5,8},		{C5,8},
		{B4,4},		{B4,8},		{C5,8},		{D5,4},		{E5,4},
		{C5,4},		{A4,4},		{A4,4},		{1,4},

		{E5,2},		{C5,2},		{D5,2},		{B4,2},		{C5,2},		{A4,2},
		{Ab4,2},	{B4,4},		{1,4},		{E5,2},		{C5,2},		{D5,2},
		{B4,2},		{C5,4},		{E5,4},		{A5,2},		{Ab5,2},	{1, 2}

};


void musicAddNotes() {

	WaveLen [2] = (tU32)(BPS*(4/(double)TetrisNotes[CurNote].Len));
	WaveData[2] = (char*)malloc(WaveLen[2]);

	tU8 Data;
	tS8 Amp = 16;
	tU16 Pos, Note;
	tU32 DataPos = 0;

	for (Pos = 0; Pos < (tU16)(BPS*(4/(double)TetrisNotes[CurNote].Len)); ++Pos) {

		Data = 128+Amp;

		if (!(Pos % (tU16)(SPS/(double)TetrisNotes[CurNote].Freq)))
			Amp = -Amp;

		WaveData[2][DataPos++] = Data;

	}

	++CurNote;

	// End of song
	if (CurNote == (sizeof(TetrisNotes)/sizeof(TetrisNotes[0]))) {
		CurNote = 0;
		CurWave ^= 1;
		(sizeof(TetrisNotes)/sizeof(TetrisNotes[0]));
	}

}



// Adds a beep to the sound buffer! (Freq, Starting Position, Length of Beep)
inline void musicStep() {

	if (!(WaveHeader[CurWave].dwFlags & WHDR_DONE))	// Wait until it is free
		return;

	if (WaveHeader[CurWave].dwFlags & WHDR_PREPARED) {
		waveOutUnprepareHeader(hWaveOut, &WaveHeader[CurWave], sizeof(WAVEHDR));
		free(WaveData[CurWave]);	// Free the old Data!
	}


	WaveData[CurWave] = WaveData[2];
	WaveLen [CurWave] = WaveLen [2];


	WaveHeader[CurWave].lpData			= WaveData[CurWave];
	WaveHeader[CurWave].dwBufferLength	= WaveLen [CurWave];

	waveOutPrepareHeader(hWaveOut, &WaveHeader[CurWave], sizeof(WAVEHDR));
	waveOutWrite		(hWaveOut, &WaveHeader[CurWave], sizeof(WAVEHDR));

	musicAddNotes();

}


inline void musicInit() {

	WaveFormat.wFormatTag		= WAVE_FORMAT_PCM;		// Uncompressed sound format
	WaveFormat.nChannels		= 1;					// Mono, Stereo
	WaveFormat.wBitsPerSample	= 8;					// Bits per sample per channel
	WaveFormat.nSamplesPerSec	= SPS;					// Sample Per Second

	WaveFormat.nBlockAlign		= 1;
	WaveFormat.nAvgBytesPerSec	= 8;
	WaveFormat.cbSize			= 0;


	WaveHeader[0].dwFlags = WHDR_DONE;
	WaveHeader[0].dwLoops = 0;
	WaveHeader[1].dwFlags = WHDR_DONE;
	WaveHeader[1].dwLoops = 0;

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFormat, 0, 0, CALLBACK_NULL);
	waveOutSetVolume(hWaveOut, 0x8888);
	musicAddNotes();

}


