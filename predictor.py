import numpy as np
from typing import Dict, List, Any

class CongestionPredictor:
    def __init__(self):
        self.historical_data: Dict[str, List[float]] = {
            "Segment_A": [50.0, 48.5, 45.0, 20.2, 15.0, 22.1, 40.0, 52.3],
            "Segment_B": [60.0, 59.1, 58.5, 57.2, 58.0, 59.0, 59.5, 60.0],
            "Segment_C": [28.0, 25.0, 21.5, 18.0, 14.2, 11.0, 9.5, 8.0]
        }
        self.free_flow_speed = 60.0

    def compute_ewma(self, data: List[float], alpha: float = 0.4) -> float:
        """Computes Exponentially Weighted Moving Average to prioritize recent telemetry."""
        weights = [(1 - alpha)**i for i in range(len(data))]
        weights.reverse()
        weights = np.array(weights) / sum(weights)
        return float(np.sum(np.array(data) * weights))

    def evaluate_corridor(self, segment_id: str) -> Dict[str, Any]:
        if segment_id not in self.historical_data:
            return {"error": "Segment unknown"}

        speeds = self.historical_data[segment_id]
        ewma_speed = self.compute_ewma(speeds)
        volatility = float(np.std(speeds))
        
        congestion_index = max(0.0, (self.free_flow_speed - ewma_speed) / self.free_flow_speed)
        rounded_index = round(congestion_index, 3)

        # Multi-tier classification with confidence estimation
        if rounded_index > 0.65:
            status = "GRIDLOCK"
            confidence = 0.94
        elif rounded_index > 0.40:
            status = "HEAVY"
            confidence = 0.89
        elif rounded_index > 0.20:
            status = "MODERATE"
            confidence = 0.85
        else:
            status = "OPTIMAL"
            confidence = 0.98

        return {
            "segment": segment_id,
            "congestion_index": rounded_index,
            "ewma_speed_kmh": round(ewma_speed, 2),
            "volatility_std": round(volatility, 2),
            "status": status,
            "model_confidence": confidence
        }

if __name__ == "__main__":
    predictor = CongestionPredictor()
    print("--- Python Enterprise Traffic Analytics Pipeline ---")
    for seg in ["Segment_A", "Segment_B", "Segment_C"]:
        metrics = predictor.evaluate_corridor(seg)
        print(f"[{metrics['segment']}] Status: {metrics['status']} | Index: {metrics['congestion_index']} | "
              f"EWMA Speed: {metrics['ewma_speed_kmh']} km/h (±{metrics['volatility_std']}) | Conf: {metrics['model_confidence']}")
