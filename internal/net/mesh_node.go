package net

import (
	"net/http"
	"sync"
	"gioui.org/f32"
	"github.com/gorilla/websocket"
	"github.com/robertpelloni/bqt/internal/kernel"
)

var upgrader = websocket.Upgrader{CheckOrigin: func(r *http.Request) bool { return true }}

type MeshNode struct {
	mu      sync.Mutex
	peers   map[*websocket.Conn]bool
	started bool
	port    string
}

// IsRunning reports whether StartNode has taken effect, and on which port.
func (mn *MeshNode) IsRunning() (bool, string) {
	if mn == nil {
		return false, ""
	}
	mn.mu.Lock()
	defer mn.mu.Unlock()
	return mn.started, mn.port
}

var (
	nodeInstance *MeshNode
	nodeOnce     sync.Once
)

func GetMeshNode() *MeshNode {
	nodeOnce.Do(func() { nodeInstance = &MeshNode{peers: make(map[*websocket.Conn]bool)} })
	return nodeInstance
}

func (mn *MeshNode) StartNode(port string, spawnCallback func(string, string, f32.Point, f32.Point, bool), ledgerSync func(string, string)) {
	// Guard: previously this registered on the global mux, so a second call
	// panicked on a duplicate pattern. Now idempotent with a private mux.
	mn.mu.Lock()
	if mn.started {
		mn.mu.Unlock()
		return
	}
	mn.started = true
	mn.port = port
	if mn.peers == nil {
		mn.peers = make(map[*websocket.Conn]bool)
	}
	mn.mu.Unlock()
	mux := http.NewServeMux()
	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		conn, err := upgrader.Upgrade(w, r, nil)
		if err != nil { return }
		mn.mu.Lock(); mn.peers[conn] = true; mn.mu.Unlock()
		defer func() { mn.mu.Lock(); delete(mn.peers, conn); mn.mu.Unlock(); conn.Close() }()

		for {
			var msg map[string]interface{}
			if err := conn.ReadJSON(&msg); err != nil { break }
			
			// Comma-ok throughout: unchecked assertions on peer input used
			// to panic the handler; main passes nil callbacks.
			mType, _ := msg["type"].(string)
			if mType == "cursor" {
				dev, _ := msg["deviceId"].(string)
				x, _ := msg["x"].(float64)
				y, _ := msg["y"].(float64)
				kernel.GetInputManager().UpdateCursor("net-"+dev, x, y)
			} else if mType == "ledger_update" && ledgerSync != nil {
				p, _ := msg["path"].(string)
				c, _ := msg["content"].(string)
				ledgerSync(p, c)
			}
		}
	})
	go http.ListenAndServe(":"+port, mux)
}

// PeerCount reports live mesh connections (dashboard Node Health readout).
func (mn *MeshNode) PeerCount() int {
	if mn == nil {
		return 0
	}
	mn.mu.Lock()
	defer mn.mu.Unlock()
	return len(mn.peers)
}

func (mn *MeshNode) Broadcast(payload interface{}) {
	mn.mu.Lock(); defer mn.mu.Unlock()
	for peer := range mn.peers { peer.WriteJSON(payload) }
}
