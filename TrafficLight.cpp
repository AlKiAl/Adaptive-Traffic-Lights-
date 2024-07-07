#include "TrafficLight.h"
#include <iostream>
#include <algorithm>

TrafficLight::TrafficLight(int id, bool isPedestrian)
    : id(id), green(false), carQueue(0), pedestrianQueue(0), isPedestrian(isPedestrian) {}

void TrafficLight::addCars(int count) {
    std::lock_guard<std::mutex> lock(mtx);
    carQueue += count;
}

void TrafficLight::addPedestrians(int count) {
    std::lock_guard<std::mutex> lock(mtx);
    pedestrianQueue += count;
}

void TrafficLight::setGreen(bool green) {
    std::lock_guard<std::mutex> lock(mtx);
    this->green = green;
}

bool TrafficLight::isGreen() const {
    std::lock_guard<std::mutex> lock(mtx);
    return green;
}

int TrafficLight::getCarQueueSize() const {
    std::lock_guard<std::mutex> lock(mtx);
    return carQueue;
}

int TrafficLight::getPedestrianQueueSize() const {
    std::lock_guard<std::mutex> lock(mtx);
    return pedestrianQueue;
}

void TrafficLight::processGreenLight(int avgQueueSize) {
    std::lock_guard<std::mutex> lock(mtx);
    if (green/* && carQueue > 0*/) {
        int More = std::min(100, static_cast<int>(0.35 * avgQueueSize));
        int Below = std::min(100, static_cast<int>(0.25 * avgQueueSize));
        int carsToProcess = carQueue > avgQueueSize ? More : Below;
        std::cout << "Light " << id << " processing green light. Cars before: " << carQueue;
        if (carQueue <= 15) { carsToProcess = carQueue; }
        if (carsToProcess <= 15 && carQueue > 15) { carsToProcess = 15; }

        carQueue -= std::min(carsToProcess, carQueue);
        std::cout << ", Cars after: " << carQueue << std::endl;
    }
}

void TrafficLight::processGreenLightForPedestrians(int avgQueueSize) {
    std::lock_guard<std::mutex> lock(mtx);
    if (green /* && pedestrianQueue > 0*/) {
        int More = std::min(50, static_cast<int>(0.35 * avgQueueSize));
        int Below = std::min(50, static_cast<int>(0.25 * avgQueueSize));
        int pedestriansToProcess = pedestrianQueue > avgQueueSize ? More : Below;
        std::cout << "Pedestrian Light " << id -4<< " processing green light. Pedestrians before: " << pedestrianQueue;

        if (pedestrianQueue <= 10) { pedestriansToProcess = pedestrianQueue; }
        if (pedestriansToProcess <= 10 &&  pedestrianQueue > 10) { pedestriansToProcess = 10; }

        pedestrianQueue -= std::min(pedestriansToProcess, pedestrianQueue);
        std::cout << ", Pedestrians after: " << pedestrianQueue << std::endl;
    }
}
