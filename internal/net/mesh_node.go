package net

import (
	"log"
	"net/http"
	"sync"
	"github.com/gorilla/websocket"
	"github.com/robertpelloni/bobui/internal/kernel"
)

var upgrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool { return true },
}

type MeshNode struct {
	mu    sync.Mutex
	peers map[*websocket.Conn]bool
}

var (
	nodeInstance *MeshNode
	nodeOnce     sync.Once
)

func GetMeshNode() *MeshNode {
	nodeOnce.Do(func() {
		nodeInstance = &MeshNode{
			peers: make(map[*websocket.Conn]bool),
		}
	})
	return nodeInstance
}

// StartNode begins the P2P Go Server.
func (mn *MeshNode) StartNode(port string) {
	http.HandleFunc("/", mn.handlePeer)
	go func() {
		log.Printf("OmniMesh Go: Listening on %s", port)
		if err := http.ListenAndServe(":"+port, nil); err != nil {
			log.Fatal(err)
		}
	}()
}

func (mn *MeshNode) handlePeer(w http.ResponseWriter, r *http.Request) {
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		return
	}
	mn.mu.Lock()
	mn.peers[conn] = true
	mn.mu.Unlock()

	defer func() {
		mn.mu.Lock()
		delete(mn.peers, conn)
		mn.mu.Unlock()
		conn.Close()
	}()

	for {
		var msg map[string]interface{}
		if err := conn.ReadJSON(&msg); err != nil {
			break
		}
		// Route remote input into the Go kernel
		if msg["type"] == "cursor" {
			id := msg["deviceId"].(string)
			x := msg["x"].(float64)
			y := msg["y"].(float64)
			kernel.GetInputManager().UpdateCursor("net-"+id, x, y)
		}
	}
}

// Broadcast blasts a payload to all Go P2P peers.
func (mn *MeshNode) Broadcast(payload interface{}) {
	mn.mu.Lock()
	defer mn.mu.Unlock()
	for peer := range mn.peers {
		peer.WriteJSON(payload)
	}
}
