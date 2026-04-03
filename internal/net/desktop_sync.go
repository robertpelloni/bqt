package net

import (
	"log"
)

type FrameShard struct {
	WindowID string `json:"window_id"`
	Data     []byte `json:"data"`
}

// BroadcastDesktopFrame captures and blasts the GPU buffer to Go peers.
func BroadcastDesktopFrame(windowID string, data []byte) {
	mesh := GetMeshNode()
	payload := map[string]interface{}{
		"type":      "desktop_mirror",
		"window_id": windowID,
		"data":      data,
	}
	mesh.Broadcast(payload)
}

// HandleRemoteDesktop mirrors a remote peer's window into the local Go UI.
func HandleRemoteDesktop(peerID string, windowID string, data []byte) {
	// Reassemble the shard and push to the local Gio GPU context
	log.Printf("OMNIMESH Go: Received %d bytes of remote desktop from %s", len(data), peerID)
}
