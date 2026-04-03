package kernel

import (
	"log"
	"github.com/robertpelloni/bobui/internal/net"
)

type ComputeJob struct {
	ID      string `json:"id"`
	Payload []byte `json:"payload"`
}

// DispatchJob shards a heavy computation to the global Go mesh.
func DispatchJob(id string, data []byte) {
	mesh := net.GetMeshNode()
	payload := map[string]interface{}{
		"type":    "compute_task",
		"id":      id,
		"content": data,
	}
	mesh.Broadcast(payload)
	log.Printf("OMNICOMPUTE Go: Compute Job [%s] sharded to the Planetary Mesh.", id)
}

// HandleRemoteTask processes a job from a peer and returns the result.
func HandleRemoteTask(peerID, jobID string, data []byte) {
	log.Printf("OMNICOMPUTE Go: Executing remote task for %s", peerID)
	// Process natively...
	
	// Broadcast result back
	net.GetMeshNode().Broadcast(map[string]interface{}{
		"type":   "compute_result",
		"id":     jobID,
		"result": []byte{0x01, 0x02}, // Mock result
	})
}
