#ifndef PLAYER_H
#define PLAYER_H

#include "Bike.h"
#include <string>
#include <memory>

struct PlayerStats {
    int totalRacesWon;
    int totalRacesPlayed;
    float bestLapTime;
    int upgradesUnlocked;
    int currentPoints;
};

class Player {
public:
    Player(int id, const std::string& playerName);
    ~Player() = default;

    void Initialize(Vector3 startPosition, Color bikeColor);
    void Update(float deltaTime);
    void Render() const;

    // Race state
    void StartRace();
    void FinishLap(float lapTime);
    void FinishRace(int position);
    void ResetRace();

    // Getters
    int GetID() const { return playerID; }
    std::string GetName() const { return name; }
    Bike* GetBike() const { return bike.get(); }
    int GetCurrentLap() const { return currentLap; }
    int GetCheckpointsPassed() const { return checkpointsPassed; }
    float GetCurrentLapTime() const { return currentLapTime; }
    float GetTotalRaceTime() const { return totalRaceTime; }
    PlayerStats GetStats() const { return stats; }
    int GetRacePosition() const { return racePosition; }

    // Setters
    void SetCheckpointsPassed(int count) { checkpointsPassed = count; }
    void SetCurrentLap(int lap) { currentLap = lap; }
    void SetRacePosition(int position) { racePosition = position; }
    void AddPoints(int points) { stats.currentPoints += points; }

    // Control
    void ProcessInput(float accelerateInput, float brakeInput, float turnInput, bool nitroPressed);

private:
    int playerID;
    std::string name;
    std::unique_ptr<Bike> bike;

    // Race state
    int currentLap;
    int checkpointsPassed;
    float currentLapTime;
    float totalRaceTime;
    int racePosition;
    bool raceFinished;

    // Statistics
    PlayerStats stats;
};

#endif // PLAYER_H
