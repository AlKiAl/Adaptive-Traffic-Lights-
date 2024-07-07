#include "TrafficLightSimulator.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

TrafficLightSimulator::TrafficLightSimulator() {
    for (int i = 0; i < 4; ++i) {
        lights[i] = new TrafficLight(i);
    }
    for (int i = 0; i < 8; ++i) {
        pedestrianLights[i] = new TrafficLight(i + 4, true);
    }
}

void TrafficLightSimulator::addCarsToLight(int lightId, int count) {
    if (lightId >= 0 && lightId < 4) {
        lights[lightId]->addCars(count);
    }
}

void TrafficLightSimulator::addPedestriansToLight(int lightId, int count) {
    if (lightId >= 0 && lightId < 8) {
        pedestrianLights[lightId]->addPedestrians(count);
    }
}

void TrafficLightSimulator::processTraffic() {
    synchronizeLights();
    int avgQueueSize = calculateAverageQueueSize();
    int avgPedestrianQueueSize = calculateAveragePedestrianQueueSize();
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.push_back(std::thread(&TrafficLightSimulator::runLight, this, lights[i], avgQueueSize));
    }
    for (int i = 0; i < 8; ++i) {
        threads.push_back(std::thread(&TrafficLightSimulator::runPedestrianLight, this, pedestrianLights[i], avgPedestrianQueueSize));
    }
    for (auto& th : threads) {
        th.join();
    }
    printTrafficStatus();
}

void TrafficLightSimulator::runLight(TrafficLight* light, int avgQueueSize) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [light]() { return light->getCarQueueSize() >= 0; });
    light->setGreen(true);
    light->processGreenLight(avgQueueSize);
    light->setGreen(false);
}

void TrafficLightSimulator::runPedestrianLight(TrafficLight* light, int avgQueueSize) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [light]() { return light->getPedestrianQueueSize() >= 0; });
    light->setGreen(true);
    light->processGreenLightForPedestrians(avgQueueSize);
    light->setGreen(false);
}

void TrafficLightSimulator::printTrafficStatus() const {
    for (int i = 0; i < 4; ++i) {
        std::cout << "Light " << i << " has " << lights[i]->getCarQueueSize() << " cars left." << std::endl;
    }
    for (int i = 0; i < 8; ++i) {
        std::cout << "Pedestrian Light " << i << " has " << pedestrianLights[i]->getPedestrianQueueSize() << " pedestrians left." << std::endl;
    }
}

int TrafficLightSimulator::calculateAverageQueueSize() const {
    int totalQueueSize = 0;
    int totalLights = 4; // only car lights for average
    for (int i = 0; i < 4; ++i) {
        totalQueueSize += lights[i]->getCarQueueSize();
    }
    return totalQueueSize / totalLights;
}

int TrafficLightSimulator::calculateAveragePedestrianQueueSize() const {
    int totalQueueSize = 0;
    int totalLights = 8; // only pedestrian lights for average
    for (int i = 0; i < 8; ++i) {
        totalQueueSize += pedestrianLights[i]->getPedestrianQueueSize();
    }
    return totalQueueSize / totalLights;
}

void TrafficLightSimulator::synchronizeLights() {
    std::unique_lock<std::mutex> lock(mtx);


    for (int i = 0; i < 4; ++i) {
        lights[i]->setGreen(false);
    }
    for (int i = 0; i < 8; ++i) {
        pedestrianLights[i]->setGreen(false);
    }


    for (int i = 0; i < 4; ++i) {
        lights[i]->setGreen(true);

        pedestrianLights[(i * 2) ]->setGreen(true);
        pedestrianLights[(i * 2 + 1)]->setGreen(true);


        std::this_thread::sleep_for(std::chrono::seconds(2));

        lights[i]->setGreen(false);
        pedestrianLights[(i * 2)]->setGreen(false);
        pedestrianLights[(i * 2 + 1)]->setGreen(false);
    }



    for (int i = 0; i < 8; ++i) {
        pedestrianLights[i]->setGreen(true);

        lights[i / 2]->setGreen(true);

        std::this_thread::sleep_for(std::chrono::seconds(2));

        pedestrianLights[i]->setGreen(false);
        lights[i / 2]->setGreen(false);
    }
}
