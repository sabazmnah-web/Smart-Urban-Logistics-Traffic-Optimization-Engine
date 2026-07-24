import numpy as np

class CongestionPredictor:
    def __init__(self):
        self.historical_data = {
            "Segment_A": [50, 48, 45, 20, 15, 22, 40, 52], # Congestion during middle indices
            "Segment_B": [60, 58, 55, 52, 50, 55, 58, 60]  # Smooth traffic
        }

    def predict_congestion_index(self, segment_id: str) -> float:
        """
        Calculates a congestion factor (0.0 = clear, 1.0 = heavy traffic)
        based on moving speed averages.
        """
        if segment_id not in self.historical_data:
            return 0.0

        speeds = self.historical_data[segment_id]
        avg_speed = np.mean(speeds)
        
        free_flow_speed = 60.0
        congestion_index = max(0.0, (free_flow_speed - avg_speed) / free_flow_speed)
        return round(float(congestion_index), 2)

if __name__ == "__main__":
    predictor = CongestionPredictor()
    print("--- Python Traffic Analytics Engine ---")
    
    for segment in ["Segment_A", "Segment_B"]:
        index = predictor.predict_congestion_index(segment)
        status = "HIGH CONGESTION" if index > 0.4 else "CLEAR"
        print(f"Segment: {segment} | Congestion Index: {index} ({status})")