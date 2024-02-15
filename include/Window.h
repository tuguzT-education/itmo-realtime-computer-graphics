#pragma once

#include "Namespace.h"

#include <wtypes.h>

namespace yet_another_game_engine
{
	class Window
	{
	public:
		struct Dimensions 
		{
			LONG width;
			LONG height;
		};

		explicit Window(LPCTSTR name, LONG width, LONG height, HINSTANCE instanceHandle = nullptr);

		HWND GetRawHandle() const;
		HINSTANCE GetRawInstanceHandle() const;

		RECT GetRect() const;
		Dimensions GetDimensions() const;

		RECT GetClientRect() const;
		Dimensions GetClientDimensions() const;

		bool SetTitle(LPCTSTR title);

	private:
		HWND handle;
	};
}
