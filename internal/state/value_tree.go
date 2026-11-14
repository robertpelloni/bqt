package state

import (
	"sort"
	"sync"
)

type ParameterChange struct {
	Key   string      `json:"key"`
	Value interface{} `json:"value"`
}

// ValueTree manages hierarchical global state in the Go Port.
type ValueTree struct {
	mu         sync.RWMutex
	params     map[string]interface{}
	listeners  []chan ParameterChange
}

var (
	treeInstance *ValueTree
	treeOnce     sync.Once
)

func GetValueTree() *ValueTree {
	treeOnce.Do(func() {
		treeInstance = &ValueTree{
			params: make(map[string]interface{}),
		}
	})
	return treeInstance
}

// SetParameter updates a key and broadcasts to all Go channels.
func (vt *ValueTree) SetParameter(key string, value interface{}) {
	vt.mu.Lock()
	vt.params[key] = value
	vt.mu.Unlock()

	// Broadcast to listeners
	vt.mu.RLock()
	change := ParameterChange{Key: key, Value: value}
	for _, ch := range vt.listeners {
		select {
		case ch <- change:
		default: // Non-blocking if channel is full
		}
	}
	vt.mu.RUnlock()
}

// ParameterCount reports how many parameters are set (dashboard readout).
func (vt *ValueTree) ParameterCount() int {
	if vt == nil {
		return 0
	}
	vt.mu.RLock()
	defer vt.mu.RUnlock()
	return len(vt.params)
}

// Keys lists set parameter keys in sorted order (dashboard readout).
func (vt *ValueTree) Keys() []string {
	if vt == nil {
		return nil
	}
	vt.mu.RLock()
	defer vt.mu.RUnlock()
	keys := make([]string, 0, len(vt.params))
	for k := range vt.params {
		keys = append(keys, k)
	}
	sort.Strings(keys)
	return keys
}

// Subscribe returns a channel that receives state updates.
func (vt *ValueTree) Subscribe() chan ParameterChange {
	vt.mu.Lock()
	defer vt.mu.Unlock()
	ch := make(chan ParameterChange, 100)
	vt.listeners = append(vt.listeners, ch)
	return ch
}
