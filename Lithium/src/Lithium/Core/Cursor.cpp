#include "lipch.h"
#include "Cursor.h"

namespace Li
{
	Cursor::Cursor()
		: m_Current(SDL_SYSTEM_CURSOR_ARROW)
	{
		SDL_Cursor* sdl_cursor = SDL_CreateSystemCursor(m_Current);
		m_Cursors[m_Current] = sdl_cursor;
		SDL_SetCursor(nullptr);
		SDL_SetCursor(sdl_cursor);
	}

	Cursor::~Cursor()
	{
		for (SDL_Cursor* cursor : m_Cursors)
		{
			if (cursor)
				SDL_FreeCursor(cursor);
		}
	}

	void Cursor::Set(SDL_SystemCursor cursor)
	{
		if (cursor == m_Current)
			return;
		
		Log::CoreWarn("Setting cursor does not work.");
		SDL_Cursor* stored = m_Cursors[cursor];
		if (stored)
		{
			SDL_SetCursor(stored);
		}
		else
		{
			SDL_Cursor* sdl_cursor = SDL_CreateSystemCursor(cursor);
			SDL_SetCursor(sdl_cursor);
			m_Cursors[cursor] = sdl_cursor;
		}
		m_Current = cursor;
	}
}
