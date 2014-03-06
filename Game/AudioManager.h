#pragma once

#include <map>
#include <string>
#include <vector>
#include "Sound.h"
#include "Music.h"

/*
 * The AUdio Manager class manages all audio playback of the game, be it just a sound effect or a continuously
 * playing music. During gameplay, only one song is allowed to play at once, but multiple sounds can play at the
 * same time. A music can loop or play only once, can have face in/out effects and volume control, as well as precise
 * play/pause controls. A sound is far more simple, it allows the media file to be played from start to finish, the
 * control that it has over the playback is over the volume and stopping the playback (with no resume).
 */
class AudioManager {
public:

	AudioManager(void);
	AudioManager(const AudioManager &copy);
	~AudioManager(void);

	AudioManager &operator=(const AudioManager &other);

	Sound *loadSound(const char *fileName);
	Music *loadMusic(const char *fileName);

	void addSound(Sound &sound, std::string name);
	Sound *getSound(std::string name);
	Sound *removeSound(std::string name);

	void playSound(std::string name);
	void stopSound(std::string name);

	void setCurrentMusic(Music *music);
	Music *getCurrentMusic() { return currentMusic; }

	/* Plays a music from where it was paused, or from the start, with an optional fade in effect */
	void playMusic(unsigned fadeInMillis);
	/* Pauses a music, with an optional fade out effect */
	void pauseMusic(unsigned fadeOutMillis);
	/* Moves the "cursor" of the music to the time specified, so the music will start at a specific time */
	void setMusicAt(unsigned millis);

protected:

	std::map<std::string, Sound*> *sounds;
	Music *currentMusic;

};