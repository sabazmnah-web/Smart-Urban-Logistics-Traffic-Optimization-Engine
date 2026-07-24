import java.util.ArrayList;
import java.util.List;

class DeliveryVehicle {
    private String vehicleId;
    private String driverName;
    private String status;

    public DeliveryVehicle(String vehicleId, String driverName) {
        this.vehicleId = vehicleId;
        this.driverName = driverName;
        this.status = "IDLE";
    }

    public String getVehicleId() { return vehicleId; }
    public String getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }

    @Override
    public String toString() {
        return "Vehicle [ID=" + vehicleId + ", Driver=" + driverName + ", Status=" + status + "]";
    }
}

public class FleetManager {
    private List<DeliveryVehicle> fleet;

    public FleetManager() {
        this.fleet = new ArrayList<>();
    }

    public void registerVehicle(DeliveryVehicle vehicle) {
        fleet.add(vehicle);
    }

    public void dispatchVehicle(String vehicleId) {
        for (DeliveryVehicle vehicle : fleet) {
            if (vehicle.getVehicleId().equals(vehicleId)) {
                vehicle.setStatus("DISPATCHED");
                System.out.println("Dispatched vehicle: " + vehicleId);
                return;
            }
        }
        System.out.println("Vehicle not found!");
    }

    public void printFleetStatus() {
        System.out.println("\n--- Java Fleet Management Service ---");
        for (DeliveryVehicle vehicle : fleet) {
            System.out.println(vehicle);
        }
    }

    public static void main(String[] args) {
        FleetManager manager = new FleetManager();
        
        manager.registerVehicle(new DeliveryVehicle("V-101", "Alex"));
        manager.registerVehicle(new DeliveryVehicle("V-102", "Sam"));

        manager.dispatchVehicle("V-101");
        manager.printFleetStatus();
    }
}
