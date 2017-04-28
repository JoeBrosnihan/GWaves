#pragma once

#include <string>

class IDisplay {
public:
	IDisplay(int width, int height, const std::string &title)
		: width(width), height(height), closed(false) {};
	virtual void update() = 0;
	virtual ~IDisplay() {}
	virtual void handleInput() = 0;
	bool isClosed() { return closed; }
	int getWidth() { return width; }
	int getHeight() { return height; }
private:
	int width;
	int height;
protected:
	bool closed;
};
