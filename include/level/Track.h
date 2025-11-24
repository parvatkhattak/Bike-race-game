#ifndef TRACK_H
#define TRACK_H

#include "raylib.h"
#include "Checkpoint.h"
#include "Obstacle.h"
#include <vector>
#include <string>
#include <memory>

struct TrackData {
    std::string name;
    int difficulty; // 1 = Beginner, 2 = Intermediate, 3 = Advanced, 4 = Expert
    int requiredLaps;
    Vector3 player1SpawnPoint;
    Vector3 player2SpawnPoint;
    Vector3 spawnDirection;
};

class Track {
public:
    Track();
    ~Track() = default;

    bool LoadTrack(const std::string& trackName);
    void Update(float deltaTime);
    void Render() const;
    void RenderDebug() const;

    // Checkpoint management
    bool CheckCheckpoint(int playerID, Vector3 bikePosition, int currentCheckpoint);
    int GetTotalCheckpoints() const { return checkpoints.size(); }
    
    // Track info
    TrackData GetTrackData() const { return trackData; }
    std::string GetName() const { return trackData.name; }
    int GetDifficulty() const { return trackData.difficulty; }
    int GetRequiredLaps() const { return trackData.requiredLaps; }

    // Spawn points
    Vector3 GetSpawnPoint(int playerID) const;
    Vector3 GetSpawnDirection() const { return trackData.spawnDirection; }

    // Collections accessors
    const std::vector<std::unique_ptr<Checkpoint>>& GetCheckpoints() const { return checkpoints; }
    const std::vector<std::unique_ptr<Obstacle>>& GetObstacles() const { return obstacles; }

private:
    void CreateBeginnerTrack();
    void CreateIntermediateTrack();
    void CreateAdvancedTrack();
    void LoadTrackModel();

    TrackData trackData;
    Model trackModel;
    bool modelLoaded;

    std::vector<std::unique_ptr<Checkpoint>> checkpoints;
    std::vector<std::unique_ptr<Obstacle>> obstacles;

    // Track mesh for collision
    BoundingBox trackBounds;
};

#endif // TRACK_H
