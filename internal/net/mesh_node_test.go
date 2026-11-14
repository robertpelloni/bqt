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

func TestMeshNodePeerCount(t *testing.T) {
 mn := &MeshNode{peers: make(map[*websocket.Conn]bool)}
 if mn.PeerCount() != 0 { t.Fatalf("expected 0 peers, got %d", mn.PeerCount()) }
 var nilNode *MeshNode
 if nilNode.PeerCount() != 0 { t.Fatal("nil node must report 0") }
}

