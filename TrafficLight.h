#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <mutex>

class TrafficLight {
public:
    TrafficLight(int id, bool isPedestrian = false);
    void addCars(int count);
    void addPedestrians(int count);
    void setGreen(bool green);
    bool isGreen() const;
    int getCarQueueSize() const;
    int getPedestrianQueueSize() const;
    void processGreenLight(int avgQueueSize);
    void processGreenLightForPedestrians(int avgQueueSize);

private:
    int id;
    bool green;
    int carQueue;
    int pedestrianQueue;
    bool isPedestrian;
    mutable std::mutex mtx;
};

#endif // TRAFFIC_LIGHT_H
