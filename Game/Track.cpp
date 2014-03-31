#include "Track.h"

Track::Track(void) {
	pieces = new std::vector<TrackPiece*>();
	loadedConfigurations = new std::vector<TrackPieceConfiguration*>();
}

Track::~Track(void) {
	pieces->clear();
	delete pieces;
	for (unsigned i = 0; i < loadedConfigurations->size(); i++) {
		delete (*loadedConfigurations)[i];
	}
	delete loadedConfigurations;
	loadedConfigurations = NULL;
}

TrackPiece *Track::generateNextPiece(Vector3 shipPos) {
	TrackPiece *last = pieces->at(pieces->size() - 1);
	Vector3 *lastPos = last->getPhysicalBody()->getPosition();
	if (lastPos->z - shipPos.z <= 1080.0f) {
		//TrackPiece *next = new TrackPiece(Vector3(0, 0, lastPos->z + 180));
		int index = generateRandom(0, loadedConfigurations->size());
		TrackPiece *next = new TrackPiece(Vector3(0, 0, lastPos->z + 180), (*loadedConfigurations)[index]);
		pieces->emplace_back(next);
		return next;
	}
	return NULL;
}

TrackPiece *Track::deleteOldPiece(Vector3 shipPos) {
	TrackPiece *first = (*(pieces->begin()));
	Vector3 *firstPos = first->getPhysicalBody()->getPosition();
	if (firstPos->z - shipPos.z <= -180.0f) {
		pieces->erase(pieces->begin());
		return first;
	}
	return NULL;
}

void Track::generateStarterPieces() {
	for (int i = 0; i < 5; i++) {
		TrackPiece *piece = new TrackPiece(Vector3(0, 0, 180.0f * i));
		pieces->emplace_back(piece);
	}
}

void Track::loadTrackPiecesFromFile(const char *fileName) {
	// First check if we have a valid file
	std::ifstream file(fileName);
	if (!file.is_open()) {
		std::cout << "ERROR: Cannot open the TrackPieCe file" << std::endl;
		return;
	}
	std::vector<std::string> lines;
	char lineBuffer[255];
	while(!file.eof()) {
		file.getline(lineBuffer, 255);
		lines.emplace_back(lineBuffer);
	}
	if (lines.size() == 0) {
		std::cout << "ERROR: TrackPieCe file is empty!" << std::endl;
		return;
	}

	if (loadedConfigurations == NULL) {
		loadedConfigurations = new std::vector<TrackPieceConfiguration*>();
	}
	TrackPieceConfiguration *currentConfig = NULL;
	DoorsetConfiguration *currentDoorset = NULL;
	for (unsigned i = 0; i < lines.size(); i++) {
		string line = lines[i];
		if (line.size() < 2) {
			continue;
		}
		// Trim the whitespaces at the beginning of the line, if present
		line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		char firstChar = line[0];

		if (firstChar == 'n') {
			// New track piece
			int difficulty = -1;
			sscanf_s(line.c_str(), "n %d", &difficulty);
			if (difficulty >= 0) {
				if (currentDoorset != NULL) {
					currentConfig->doorsets.emplace_back(currentDoorset);
					currentDoorset = NULL;
				}
				if (currentConfig != NULL) {
					loadedConfigurations->emplace_back(currentConfig);
				}
				currentConfig = new TrackPieceConfiguration(difficulty);
			}
		}
		if (firstChar == 's') {
			// New doorset
			float position = 0;
			sscanf_s(line.c_str(), "s %f", &position);
			if (currentDoorset != NULL && currentConfig != NULL) {
				currentConfig->doorsets.emplace_back(currentDoorset);
			}
			currentDoorset = new DoorsetConfiguration(position);
		}
		if (firstChar == 'o') {
			float open1 = 100, open2 = 100, open3 = 100, open4 = 100, open5 = 100, open6 = 100;
			sscanf_s(line.c_str(), "o %f %f %f %f %f %f", &open1, &open2, &open3, &open4, &open5, &open6);
			if (currentDoorset != NULL) {
				currentDoorset->openness[0] = open1;
				currentDoorset->openness[1] = open2;
				currentDoorset->openness[2] = open3;
				currentDoorset->openness[3] = open4;
				currentDoorset->openness[4] = open5;
				currentDoorset->openness[5] = open6;
			}
		}
	}
	if (currentConfig != NULL) {
		if (currentDoorset != NULL) {
			currentConfig->doorsets.emplace_back(currentDoorset);
			currentDoorset = NULL;
		}
		loadedConfigurations->emplace_back(currentConfig);
	}
}