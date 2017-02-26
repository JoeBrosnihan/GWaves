#pragma once

/*
Manages the window and rendering of our application.
This class follows the CRTP to avoid virtual function call overhead.
*/

template <typename Impl>
class IRenderer {
public:
	void init() {
		static_cast<Impl*>(this)->init();
	};

	void renderFrame() {
	};
};
