#pragma once

#include "SDL.h"

namespace Li
{
	class Cursor
	{
	public:
		Cursor();
		~Cursor();

		void Set(SDL_SystemCursor cursor);

	private:
		SDL_SystemCursor m_Current;
		std::array<SDL_Cursor*, SDL_NUM_SYSTEM_CURSORS> m_Cursors = {};
	};
}
