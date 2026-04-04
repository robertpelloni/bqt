package kernel

import "log"

type ComputeJob struct {
	ID      string `json:"id"`
	Payload []byte `json:"payload"`
}

// DispatchJob records intent to distribute a heavy computation.
// The verified Go baseline keeps this package free of networking imports to avoid
// package cycles; mesh dispatch should be orchestrated by a higher-level coordinator.
func DispatchJob(id string, data []byte) {
	log.Printf("OMNICOMPUTE Go: queued compute job [%s] (%d bytes) for mesh dispatch", id, len(data))
}

// HandleRemoteTask is a verified local stub. Real distributed execution should be
// coordinated by the networking layer and then handed back into the audio/compute kernels.
func HandleRemoteTask(peerID, jobID string, data []byte) {
	log.Printf("OMNICOMPUTE Go: received remote task [%s] from %s (%d bytes)", jobID, peerID, len(data))
}
