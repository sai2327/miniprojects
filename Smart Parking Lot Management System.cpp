#include <bits/stdc++.h>
using namespace std;

// ==================== Vehicle Hierarchy ====================

enum class VehicleType { CAR, BIKE, TRUCK };

class Vehicle {
protected:
    string licensePlate;
    string ownerName;
    time_t entryTime;
    VehicleType type;

public:
    Vehicle(const string& plate, const string& owner, VehicleType vType)
        : licensePlate(plate), ownerName(owner), type(vType) {
        entryTime = time(nullptr);
    }

    virtual ~Vehicle() = default;

    // Getters
    string getLicensePlate() const { return licensePlate; }
    string getOwnerName() const { return ownerName; }
    time_t getEntryTime() const { return entryTime; }
    VehicleType getType() const { return type; }

    // Pure virtual function for polymorphism
    virtual double calculateParkingFee(time_t exitTime) const = 0;
    virtual string getVehicleTypeName() const = 0;

    // Display vehicle information
    void displayInfo() const {
        cout << "Type: " << getVehicleTypeName() 
             << " | License: " << licensePlate 
             << " | Owner: " << ownerName << endl;
    }
};

class Car : public Vehicle {
private:
    static constexpr double RATE_PER_HOUR = 20.0;

public:
    Car(const string& plate, const string& owner)
        : Vehicle(plate, owner, VehicleType::CAR) {}

    double calculateParkingFee(time_t exitTime) const override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return max(hours * RATE_PER_HOUR, RATE_PER_HOUR); // Minimum 1 hour charge
    }

    string getVehicleTypeName() const override { return "Car"; }
};

class Bike : public Vehicle {
private:
    static constexpr double RATE_PER_HOUR = 10.0;

public:
    Bike(const string& plate, const string& owner)
        : Vehicle(plate, owner, VehicleType::BIKE) {}

    double calculateParkingFee(time_t exitTime) const override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return max(hours * RATE_PER_HOUR, RATE_PER_HOUR);
    }

    string getVehicleTypeName() const override { return "Bike"; }
};

class Truck : public Vehicle {
private:
    static constexpr double RATE_PER_HOUR = 30.0;

public:
    Truck(const string& plate, const string& owner)
        : Vehicle(plate, owner, VehicleType::TRUCK) {}

    double calculateParkingFee(time_t exitTime) const override {
        double hours = difftime(exitTime, entryTime) / 3600.0;
        return max(hours * RATE_PER_HOUR, RATE_PER_HOUR);
    }

    string getVehicleTypeName() const override { return "Truck"; }
};

// ==================== Parking Slot ====================

class ParkingSlot {
private:
    int slotNumber;
    bool isOccupied;
    shared_ptr<Vehicle> parkedVehicle;

public:
    ParkingSlot(int number) : slotNumber(number), isOccupied(false), parkedVehicle(nullptr) {}

    bool parkVehicle(shared_ptr<Vehicle> vehicle) {
        if (isOccupied || !vehicle) {
            return false;
        }
        parkedVehicle = vehicle;
        isOccupied = true;
        return true;
    }

    pair<shared_ptr<Vehicle>, double> removeVehicle() {
        if (!isOccupied || !parkedVehicle) {
            return {nullptr, 0.0};
        }
        
        time_t exitTime = time(nullptr);
        double fee = parkedVehicle->calculateParkingFee(exitTime);
        auto vehicle = parkedVehicle;
        
        parkedVehicle = nullptr;
        isOccupied = false;
        
        return {vehicle, fee};
    }

    bool isAvailable() const { return !isOccupied; }
    int getSlotNumber() const { return slotNumber; }
    shared_ptr<Vehicle> getVehicle() const { return parkedVehicle; }
};

// ==================== Parking Lot Management System ====================

class ParkingLotSystem {
private:
    vector<ParkingSlot> slots;
    priority_queue<int, vector<int>, greater<int>> availableSlots; // Min-heap for slot assignment
    unordered_map<string, int> vehicleToSlot; // License plate -> slot number mapping
    int totalSlots;
    double totalRevenue;

public:
    ParkingLotSystem(int numSlots) : totalSlots(numSlots), totalRevenue(0.0) {
        slots.reserve(numSlots);
        for (int i = 1; i <= numSlots; ++i) {
            slots.emplace_back(i);
            availableSlots.push(i);
        }
    }

    // Park a vehicle in the nearest available slot
    bool parkVehicle(shared_ptr<Vehicle> vehicle) {
        if (!vehicle) {
            cout << "❌ Invalid vehicle!" << endl;
            return false;
        }

        string licensePlate = vehicle->getLicensePlate();
        
        // Check if vehicle is already parked
        if (vehicleToSlot.find(licensePlate) != vehicleToSlot.end()) {
            cout << "❌ Vehicle " << licensePlate << " is already parked at slot " 
                 << vehicleToSlot[licensePlate] << endl;
            return false;
        }

        // Check if parking is full
        if (availableSlots.empty()) {
            cout << "❌ Parking lot is full!" << endl;
            return false;
        }

        // Assign nearest available slot
        int slotNumber = availableSlots.top();
        availableSlots.pop();

        if (slots[slotNumber - 1].parkVehicle(vehicle)) {
            vehicleToSlot[licensePlate] = slotNumber;
            cout << "✅ Vehicle parked successfully at slot " << slotNumber << endl;
            vehicle->displayInfo();
            return true;
        }

        // Rollback if parking failed
        availableSlots.push(slotNumber);
        return false;
    }

    // Remove vehicle from parking lot
    bool removeVehicle(const string& licensePlate) {
        auto it = vehicleToSlot.find(licensePlate);
        if (it == vehicleToSlot.end()) {
            cout << "❌ Vehicle " << licensePlate << " not found in parking lot!" << endl;
            return false;
        }

        int slotNumber = it->second;
        auto [vehicle, fee] = slots[slotNumber - 1].removeVehicle();

        if (!vehicle) {
            cout << "❌ Error removing vehicle from slot " << slotNumber << endl;
            return false;
        }

        // Calculate parking duration
        time_t duration = time(nullptr) - vehicle->getEntryTime();
        int hours = duration / 3600;
        int minutes = (duration % 3600) / 60;

        cout << "\n=== Parking Receipt ===" << endl;
        vehicle->displayInfo();
        cout << "Slot Number: " << slotNumber << endl;
        cout << "Duration: " << hours << " hour(s) " << minutes << " minute(s)" << endl;
        cout << fixed << setprecision(2);
        cout << "Parking Fee: ₹" << fee << endl;
        cout << "======================\n" << endl;

        totalRevenue += fee;
        vehicleToSlot.erase(it);
        availableSlots.push(slotNumber);

        return true;
    }

    // Display current parking status
    void displayStatus() const {
        cout << "\n========== Parking Lot Status ==========" << endl;
        cout << "Total Slots: " << totalSlots << endl;
        cout << "Available: " << availableSlots.size() << endl;
        cout << "Occupied: " << (totalSlots - availableSlots.size()) << endl;
        cout << fixed << setprecision(2);
        cout << "Total Revenue: ₹" << totalRevenue << endl;
        cout << "========================================\n" << endl;
    }

    // Display all parked vehicles
    void displayParkedVehicles() const {
        cout << "\n===== Parked Vehicles =====" << endl;
        bool anyParked = false;
        
        for (const auto& slot : slots) {
            if (!slot.isAvailable()) {
                auto vehicle = slot.getVehicle();
                cout << "Slot " << slot.getSlotNumber() << ": ";
                vehicle->displayInfo();
                anyParked = true;
            }
        }
        
        if (!anyParked) {
            cout << "No vehicles currently parked." << endl;
        }
        cout << "==========================\n" << endl;
    }

    // Search for a vehicle by license plate
    void searchVehicle(const string& licensePlate) const {
        auto it = vehicleToSlot.find(licensePlate);
        if (it == vehicleToSlot.end()) {
            cout << "❌ Vehicle " << licensePlate << " not found!" << endl;
            return;
        }

        int slotNumber = it->second;
        auto vehicle = slots[slotNumber - 1].getVehicle();
        
        cout << "\n=== Vehicle Found ===" << endl;
        cout << "Slot Number: " << slotNumber << endl;
        vehicle->displayInfo();
        
        time_t duration = time(nullptr) - vehicle->getEntryTime();
        cout << "Parked for: " << (duration / 3600) << " hour(s) " 
             << ((duration % 3600) / 60) << " minute(s)" << endl;
        cout << "===================\n" << endl;
    }

    // Get available slots count
    int getAvailableCount() const {
        return availableSlots.size();
    }
};

// ==================== Main Application ====================

void displayMenu() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   PARKING LOT MANAGEMENT SYSTEM        ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "1. Park a Vehicle" << endl;
    cout << "2. Remove a Vehicle" << endl;
    cout << "3. Display Parking Status" << endl;
    cout << "4. Display All Parked Vehicles" << endl;
    cout << "5. Search Vehicle" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int totalSlots;
    cout << "Enter total number of parking slots: ";
    cin >> totalSlots;

    if (totalSlots <= 0) {
        cout << "Invalid number of slots!" << endl;
        return 1;
    }

    ParkingLotSystem parkingLot(totalSlots);
    int choice;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: {
                string licensePlate, ownerName;
                int vehicleType;

                cout << "\nVehicle Type (1-Car, 2-Bike, 3-Truck): ";
                cin >> vehicleType;
                cin.ignore();

                cout << "License Plate: ";
                getline(cin, licensePlate);
                
                cout << "Owner Name: ";
                getline(cin, ownerName);

                shared_ptr<Vehicle> vehicle;
                
                switch (vehicleType) {
                    case 1:
                        vehicle = make_shared<Car>(licensePlate, ownerName);
                        break;
                    case 2:
                        vehicle = make_shared<Bike>(licensePlate, ownerName);
                        break;
                    case 3:
                        vehicle = make_shared<Truck>(licensePlate, ownerName);
                        break;
                    default:
                        cout << "❌ Invalid vehicle type!" << endl;
                        continue;
                }

                parkingLot.parkVehicle(vehicle);
                break;
            }

            case 2: {
                string licensePlate;
                cout << "\nEnter License Plate: ";
                getline(cin, licensePlate);
                parkingLot.removeVehicle(licensePlate);
                break;
            }

            case 3:
                parkingLot.displayStatus();
                break;

            case 4:
                parkingLot.displayParkedVehicles();
                break;

            case 5: {
                string licensePlate;
                cout << "\nEnter License Plate: ";
                getline(cin, licensePlate);
                parkingLot.searchVehicle(licensePlate);
                break;
            }

            case 6:
                cout << "\nThank you for using the Parking Lot Management System!" << endl;
                break;

            default:
                cout << "❌ Invalid choice! Please try again." << endl;
        }

    } while (choice != 6);

    return 0;
}
