/*
 
 This file is a part of liblsdj, a C library for managing everything
 that has to do with LSDJ, software for writing music (chiptune) with
 your gameboy. For more information, see:
 
 * https://github.com/stijnfrishert/liblsdj
 * http://www.littlesounddj.com
 
 --------------------------------------------------------------------------------
 
 MIT License
 
 Copyright (c) 2018 - 2020 Stijn Frishert
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 */

#include "wave.h"

#include <assert.h>
#include <string.h>

#define WAVES_OFFSET (0x6000)

void lsdj_wave_set_bytes(lsdj_song_t* song, uint8_t wave, const uint8_t* data)
{
	const size_t index = wave * LSDJ_WAVE_BYTE_COUNT;
	assert(index < 4096);

	memcpy(&song->bytes[WAVES_OFFSET + index], data, LSDJ_WAVE_BYTE_COUNT);
}

const uint8_t* lsdj_wave_get_bytes(const lsdj_song_t* song, uint8_t wave)
{
	const size_t index = wave * LSDJ_WAVE_BYTE_COUNT;
	assert(index < 4096);

	return &song->bytes[WAVES_OFFSET + index];
}

