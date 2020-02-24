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

#include "phrase.h"

#include <assert.h>

#define PHRASE_NOTES_OFFSET (0x0000)
#define PHRASE_ALLOCATIONS_OFFSET (0x3E82)
#define PHRASE_COMMANDS_OFFSET (0x4000)
#define PHRASE_COMMAND_VALUES_OFFSET (0x4FF0)
#define PHRASE_INSTRUMENTS_OFFSET (0x7000)

#define PHRASE_SETTER(OFFSET, LENGTH, VALUE) \
const size_t index = phrase * LSDJ_PHRASE_LENGTH + row; \
assert(index <= LENGTH); \
song->bytes[OFFSET + index] = VALUE;

#define PHRASE_GETTER(OFFSET, LENGTH) \
const size_t index = phrase * LSDJ_PHRASE_LENGTH + row; \
assert(index <= LENGTH); \
return song->bytes[OFFSET + index];

bool lsdj_phrase_is_allocated(const lsdj_song_t* song, uint8_t phrase)
{
	const size_t index = phrase / 8;
	assert(index < 32);

	const size_t mask = 1 << (phrase - (index * 8));
    
	return (song->bytes[PHRASE_ALLOCATIONS_OFFSET + index] & mask) != 0;
}

void lsdj_phrase_set_note(lsdj_song_t* song, uint8_t phrase, uint8_t row, uint8_t note)
{
	PHRASE_SETTER(PHRASE_NOTES_OFFSET, 4080, note)
}

uint8_t lsdj_phrase_get_note(const lsdj_song_t* song, uint8_t phrase, uint8_t row)
{
	PHRASE_GETTER(PHRASE_NOTES_OFFSET, 4080)
}

void lsdj_phrase_set_instrument(lsdj_song_t* song, uint8_t phrase, uint8_t row, uint8_t instrument)
{
	PHRASE_SETTER(PHRASE_INSTRUMENTS_OFFSET, 4080, instrument)
}

uint8_t lsdj_phrase_get_instrument(const lsdj_song_t* song, uint8_t phrase, uint8_t row)
{
	PHRASE_GETTER(PHRASE_INSTRUMENTS_OFFSET, 4080)
}

bool lsdj_phrase_set_command(lsdj_song_t* song, uint8_t phrase, uint8_t row, lsdj_command command)
{
    if (command == LSDJ_COMMAND_B && lsdj_song_get_format_version(song) < 8)
        return false;
    
	PHRASE_SETTER(PHRASE_COMMANDS_OFFSET, 4080, (uint8_t)command)
    return true;
}

lsdj_command lsdj_phrase_get_command(const lsdj_song_t* song, uint8_t phrase, uint8_t row)
{
	PHRASE_GETTER(PHRASE_COMMANDS_OFFSET, 4080)
}

void lsdj_phrase_set_command_value(lsdj_song_t* song, uint8_t phrase, uint8_t row, uint8_t value)
{
	PHRASE_SETTER(PHRASE_COMMAND_VALUES_OFFSET, 4080, value)
}

uint8_t lsdj_phrase_get_command_value(const lsdj_song_t* song, uint8_t phrase, uint8_t row)
{
	PHRASE_GETTER(PHRASE_COMMAND_VALUES_OFFSET, 4080)
}
