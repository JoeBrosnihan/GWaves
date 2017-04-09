#pragma once

#include <string>

class IDisplay {
public:
	IDisplay(int width, int height, const std::string &title)
		: width(width), height(height), closed(false) {};
	virtual void update() = 0;
	virtual ~IDisplay() {}
	bool isClosed() { return closed; }
private:
	int width;
	int height;
protected:
	bool closed;
};
