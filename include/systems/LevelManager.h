#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "../level/Track.h"
#include "../entities/Player.h"
#include <memory>
#include <vector>

enum class RaceState {
    NOT_STARTED,
    COUNTDOWN,
    RACING,
    FINISHED
};

class LevelManager {
public:
    LevelManager();
    ~LevelManager() = default;

    void Initialize();
    void LoadLevel(int levelID);
    void Update(float deltaTime);
    void Render() const;

    // Race management
    void StartRace();
    void EndRace();
    void PauseRace();
    void ResumeRace();
    void RestartRace();

    // Player management
    void AddPlayer(int playerID, const std::string& name);
    Player* GetPlayer(int playerID) const;
    int GetWinner() const;

    // Race state
    RaceState GetRaceState() const { return raceState; }
    Track* GetCurrentTrack() const { return currentTrack.get(); }
    bool IsRaceFinished() const { return raceState == RaceState::FINISHED; }

    // Difficulty management
    void UnlockLevel(int levelID);
    bool IsLevelUnlocked(int levelID) const;

private:
    void UpdateRaceProgress(float deltaTime);
    void UpdatePlayerPositions();
    void CheckCheckpoints();
    void CheckCollisions();

    std::unique_ptr<Track> currentTrack;
    std::vector<std::unique_ptr<Player>> players;

    RaceState raceState;
    float countdownTimer;
    float raceTime;
    int currentLevelID;

    std::vector<bool> unlockedLevels;
};

#endif // LEVELMANAGER_H
