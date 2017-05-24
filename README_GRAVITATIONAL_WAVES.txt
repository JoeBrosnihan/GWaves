Gravitational Waves in Virtual Reality
-Joseph Brosnihan May 2017

The source code for the G-wave visualizations is located in samples/hellovr_opengl. (The visualizations were built from a Valve "hello world" vr project. The original source code is in hellovr_opengl_main_valve.cpp.)

This project contains two visualizations. To switch between them, edit the source and choose which function to call in the main method in hellovr_opengl_main.cpp:

int main(int argc, char *argv[])
{
	GFieldVisualization();
	//or
	//SpaceWarpVisualization();

	return 0;
}