#pragma once

/*
Manages the window and rendering of our application.
This class follows the CRTP to avoid virtual function call overhead.
*/

template <typename Impl, typename RenderTarget>
class IRenderer {
public:
	typedef RenderTarget RenderTarget;

	void init() {
		static_cast<Impl*>(this)->init();
	};

	void renderFrame() {
		static_cast<Impl*>(this)->renderFrame();
	};
private:
	RenderTarget *rendertarget;
};
