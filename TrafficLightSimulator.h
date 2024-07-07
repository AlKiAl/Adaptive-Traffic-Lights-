#ifndef TRAFFIC_LIGHT_SIMULATOR_H
#define TRAFFIC_LIGHT_SIMULATOR_H

#include "TrafficLight.h"
#include <condition_variable>
#include <vector>

class TrafficLightSimulator {
public:
    TrafficLightSimulator();
    void addCarsToLight(int lightId, int count);
    void addPedestriansToLight(int lightId, int count);
    void processTraffic();

private:
    TrafficLight* lights[4];
    TrafficLight* pedestrianLights[8];
    std::mutex mtx;
    std::condition_variable cv;

    void synchronizeLights();
    void runLight(TrafficLight* light, int avgQueueSize);
    void runPedestrianLight(TrafficLight* light, int avgQueueSize);
    void printTrafficStatus() const;
    int calculateAverageQueueSize() const;
    int calculateAveragePedestrianQueueSize() const;
};

#endif // TRAFFIC_LIGHT_SIMULATOR_H
