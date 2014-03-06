#pragma once

#include <string>

class Music {
public:

	Music(void);
	Music(Music &copy);
	~Music(void);

	Music &operator=(const Music &other);

protected:

	bool loop;
	int volume;
	int playbackCursor;
	std::string filename;
};

