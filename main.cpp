#include "TrafficLightSimulator.h"
#include <iostream>

int main() {
    TrafficLightSimulator simulator;
    int cars, pedestrians;

    while (true) {
        for (int i = 0; i < 4; ++i) {
            std::cout << "Enter the number of cars arriving at light " << i << ": ";
            std::cin >> cars;
            simulator.addCarsToLight(i, cars);
        }

        for (int i = 0; i < 8; ++i) {
            std::cout << "Enter the number of pedestrians arriving at pedestrian light " << i << ": ";
            std::cin >> pedestrians;
            simulator.addPedestriansToLight(i, pedestrians);
        }

        simulator.processTraffic();
    }

    return 0;
}
