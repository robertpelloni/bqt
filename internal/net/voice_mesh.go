package net

import (
	"log"
	"sync"
)

type VoiceMesh struct {
	mu           sync.RWMutex
	localBuffer  []byte
	peerBuffers  map[string][]byte // PeerID -> AudioBuffer
}

var (
	voiceInstance *VoiceMesh
	voiceOnce     sync.Once
)

func GetVoiceMesh() *VoiceMesh {
	voiceOnce.Do(func() {
		voiceInstance = &VoiceMesh{
			peerBuffers: make(map[string][]byte),
		}
	})
	return voiceInstance
}

// BroadcastVoice shacks local PCM data and sends it through the MeshNode.
func (vm *VoiceMesh) BroadcastVoice(data []byte) {
	mesh := GetMeshNode()
	payload := map[string]interface{}{
		"type": "voice_stream",
		"data": data,
	}
	mesh.Broadcast(payload)
}

// HandleRemoteVoice reassembles incoming shards for local playback.
func (vm *VoiceMesh) HandleRemoteVoice(peerID string, data []byte) {
	vm.mu.Lock()
	defer vm.mu.Unlock()
	
	log.Printf("OmniMesh Go: Received %d bytes of voice from %s", len(data), peerID)
	vm.peerBuffers[peerID] = data
	// Hook to Go-native audio device output here
}
