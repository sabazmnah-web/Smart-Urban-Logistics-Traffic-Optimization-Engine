import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

class DeliveryVehicle {
    private final String vehicleId;
    private final String driverName;
    private volatile String status;
    private volatile double currentSpeedKmh;
    private volatile double latitude;
    private volatile double longitude;

    public DeliveryVehicle(String vehicleId, String driverName, double lat, double lon) {
        this.vehicleId = vehicleId;
        this.driverName = driverName;
        this.status = "IDLE";
        this.currentSpeedKmh = 0.0;
        this.latitude = lat;
        this.longitude = lon;
    }

    public String getVehicleId() { return vehicleId; }
    public String getDriverName() { return driverName; }
    public String getStatus() { return status; }
    public void setStatus(String status) { this.status = status; }
    public double getCurrentSpeedKmh() { return currentSpeedKmh; }
    public void setCurrentSpeedKmh(double speed) { this.currentSpeedKmh = speed; }
    public void updateCoordinates(double lat, double lon) {
        this.latitude = lat;
        this.longitude = lon;
    }

    @Override
    public String toString() {
        return String.format("Vehicle [ID=%s, Driver=%s, Status=%s, Speed=%.1f km/h, Pos=(%.4f, %.4f)]", 
                vehicleId, driverName, status, currentSpeedKmh, latitude, longitude);
    }
}

public class FleetManager {
    private final Map<String, DeliveryVehicle> fleetRegistry;

    public FleetManager() {
        this.fleetRegistry = new ConcurrentHashMap<>();
    }

    public void registerVehicle(DeliveryVehicle vehicle) {
        fleetRegistry.put(vehicle.getVehicleId(), vehicle);
    }

    public void dispatchVehicle(String vehicleId, double initialSpeed) {
        fleetRegistry.computeIfPresent(vehicleId, (id, vehicle) -> {
            vehicle.setStatus("DISPATCHED");
            vehicle.setCurrentSpeedKmh(initialSpeed);
            System.out.println(">> [Concurrency Safe] Dispatched unit: " + id);
            return vehicle;
        });
    }

    public void updateTelemetry(String vehicleId, double speed, double lat, double lon) {
        fleetRegistry.computeIfPresent(vehicleId, (id, vehicle) -> {
            vehicle.setCurrentSpeedKmh(speed);
            vehicle.updateCoordinates(lat, lon);
            return vehicle;
        });
    }

    public List<DeliveryVehicle> queryVehiclesByStatus(String status) {
        return fleetRegistry.values().stream()
                .filter(v -> v.getStatus().equalsIgnoreCase(status))
                .collect(Collectors.toList());
    }

    public void renderFleetDashboard() {
        System.out.println("\n--- Java Concurrent Fleet Telemetry Dashboard ---");
        fleetRegistry.values().forEach(v -> System.out.println("   " + v));
        System.out.println("--------------------------------------------------\n");
    }

    public static void main(String[] args) {
        FleetManager manager = new FleetManager();
        
        manager.registerVehicle(new DeliveryVehicle("V-101", "Alex", 23.8103, 90.4125));
        manager.registerVehicle(new DeliveryVehicle("V-102", "Sam", 23.7937, 90.4066));
        manager.registerVehicle(new DeliveryVehicle("V-103", "Jordan", 23.7461, 90.3742));

        manager.dispatchVehicle("V-101", 48.5);
        manager.dispatchVehicle("V-103", 53.2);

        manager.updateTelemetry("V-101", 52.0, 23.8150, 90.4180);

        manager.renderFleetDashboard();
        System.out.println("Active Deployed Units Count: " + manager.queryVehiclesByStatus("DISPATCHED").size());
    }
}
