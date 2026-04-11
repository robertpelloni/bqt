package net

import (
	"bytes"
	"encoding/gob"
	"log"
)

type MemoryShard struct {
	Type    string
	Payload []byte
}

// TeleportState serializes a Go memory heap and blasts it to the mesh.
func TeleportState(obj interface{}) error {
	var buf bytes.Buffer
	enc := gob.NewEncoder(&buf)
	if err := enc.Encode(obj); err != nil {
		return err
	}

	payload := map[string]interface{}{
		"type":    "memory_teleport",
		"content": buf.Bytes(),
	}
	GetMeshNode().Broadcast(payload)
	log.Printf("OMNITELEPORT Go: Memory Heap (%d bytes) sharded to Mesh.", buf.Len())
	return nil
}

// HandleRemoteTeleport re-materializes a peer's memory state locally.
func HandleRemoteTeleport(data []byte, target interface{}) error {
	dec := gob.NewDecoder(bytes.NewReader(data))
	return dec.Decode(target)
}
