package kernel

import (
	"log"
	"sync"
)

type EditOp struct {
	UserID    string
	Pos       int
	Text      string
	Timestamp int64
}

type MergeKernel struct {
	mu      sync.Mutex
	History []EditOp
}

var (
	mergeInstance *MergeKernel
)

func GetMergeKernel() *MergeKernel {
	if mergeInstance == nil {
		mergeInstance = &MergeKernel{History: make([]EditOp, 0)}
	}
	return mergeInstance
}

// ProcessEdit resolves a concurrent keystroke from the Go mesh.
func (mk *MergeKernel) ProcessEdit(op EditOp) {
	mk.mu.Lock()
	defer mk.mu.Unlock()
	
	// Native Go Temporal Interleaving
	mk.History = append(mk.History, op)
	log.Printf("OMNISCRIPT: Merged concurrent edit from %s at Pos %d", op.UserID, op.Pos)
}
