extern char* MovieToLoad;	//Contains the filename of the savestate specified in the command line arguments
extern char* StateToLoad;	//Contains the filename of the movie file specified in the command line arguments
extern char* ConfigToLoad;	//Contains the filename of the config file specified in the command line arguments
extern char* LuaToLoad;		//Contains the filename of the lua script specified in the command line arguments
extern char* PaletteToLoad; //Contains the filename of the palette file specified in teh comman line arguments
extern bool replayReadOnlySetting;
extern int replayStopFrameSetting;
extern int PauseAfterLoad;
char *ParseArgies(int argc, char *argv[]);
