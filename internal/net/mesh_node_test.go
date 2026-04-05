package net

import (
	"testing"

	"github.com/gorilla/websocket"
)

func TestGetMeshNodeReturnsSingleton(t *testing.T) {
	a := GetMeshNode()
	b := GetMeshNode()
	if a == nil || b == nil {
		t.Fatal("expected mesh node singleton instances to be non-nil")
	}
	if a != b {
		t.Fatal("expected GetMeshNode to return the same singleton instance")
	}
}

func TestMeshNodeBroadcastWithNoPeersDoesNotPanic(t *testing.T) {
	mn := &MeshNode{peers: make(map[*websocket.Conn]bool)}
	mn.Broadcast(map[string]any{"type": "ping"})
}
