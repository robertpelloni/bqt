package net

import (
	"log"
)

// BroadcastAudioShards sends a raw PCM block to the Go mesh.
func BroadcastAudioShards(buffer []float32) {
	mesh := GetMeshNode()
	payload := map[string]interface{}{
		"type": "audio_shard",
		"data": buffer,
	}
	mesh.Broadcast(payload)
}

// HandleRemoteAudio handles incoming DSP buffers from P2P peers.
func HandleRemoteAudio(peerID string, buffer []interface{}) {
	// Reassemble shards for the local Go Audio Graph
	log.Printf("OMNIMESH Go: Received %d audio samples from %s", len(buffer), peerID)
}
