package net

import (
	"bytes"
	"image"
	"image/jpeg"
	"log"
)

type VideoFrame struct {
	SourceID string `json:"source_id"`
	Payload  []byte `json:"payload"`
}

// BroadcastFrame shards a local GPU buffer to the global mesh.
func BroadcastFrame(sourceID string, img image.Image) {
	buf := new(bytes.Buffer)
	// High-speed, low-latency JPEG encoding natively in Go
	jpeg.Encode(buf, img, &jpeg.Options{Quality: 60})
	
	payload := map[string]interface{}{
		"type":    "stream_shard",
		"source":  sourceID,
		"content": buf.Bytes(),
	}
	GetMeshNode().Broadcast(payload)
	log.Printf("OMNISTREAM Go: Broadcasted UI Frame Shard (%d bytes)", buf.Len())
}

// HandleRemoteStream reconstructs the peer's UI on the local GPU.
func HandleRemoteStream(peerID string, data []byte) {
	log.Printf("OMNISTREAM Go: Syncing remote frame from %s", peerID)
	// Decode and push to local internal/ui/widgets/rhi_node.go...
}
