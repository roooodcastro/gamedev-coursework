#include "Leaderboard.h"

Leaderboard *Leaderboard::instance = NULL;

Leaderboard::Leaderboard(void) {
	// I know this is horrible code, I'd do it better if I had the time :S
	first = ConfigurationManager::getInstance()->readInt("leaderboard1", 0);
	second = ConfigurationManager::getInstance()->readInt("leaderboard2", 0);
	third = ConfigurationManager::getInstance()->readInt("leaderboard3", 0);
	fourth = ConfigurationManager::getInstance()->readInt("leaderboard4", 0);
	fifth = ConfigurationManager::getInstance()->readInt("leaderboard5", 0);
}

Leaderboard::~Leaderboard(void) {
}

Leaderboard *Leaderboard::getInstance() {
	if (instance == NULL) {
		instance = new Leaderboard();
	}
	return instance;
}

int Leaderboard::pushNewScore(int score) {
	if (score > first) {
		fifth = fourth;
		fourth = third;
		third = second;
		second = first;
		first = score;
		updateFile();
		return 1;
	} else if (score > second) {
		fifth = fourth;
		fourth = third;
		third = second;
		second = score;
		updateFile();
		return 2;
	} else if (score > third) {
		fifth = fourth;
		fourth = third;
		third = score;
		updateFile();
		return 3;
	} else if (score > fourth) {
		fifth = fourth;
		fourth = score;
		updateFile();
		return 4;
	} else if (score > fifth) {
		fifth = score;
		updateFile();
		return 5;
	}
	return 0;
}

int Leaderboard::getScore(int position) {
	switch (position) {
	case 1:	return first;
	case 2: return second;
	case 3: return third;
	case 4: return fourth;
	case 5: return fifth;
	}
	return 0;
}

void Leaderboard::updateFile() {
	ConfigurationManager::getInstance()->writeConfig("leaderboard1", first);
	ConfigurationManager::getInstance()->writeConfig("leaderboard2", second);
	ConfigurationManager::getInstance()->writeConfig("leaderboard3", third);
	ConfigurationManager::getInstance()->writeConfig("leaderboard4", fourth);
	ConfigurationManager::getInstance()->writeConfig("leaderboard5", fifth);
}