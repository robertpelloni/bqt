package kernel

import (
	"log"
	"sync"
)

type Action struct {
	ID       string
	Label    string
	Shortcut string
	Handler  func(userID string)
}

type ActionRegistry struct {
	mu      sync.RWMutex
	actions map[string]Action
}

var (
	actionInstance *ActionRegistry
	actionOnce     sync.Once
)

func GetActionRegistry() *ActionRegistry {
	actionOnce.Do(func() {
		actionInstance = &ActionRegistry{
			actions: make(map[string]Action),
		}
	})
	return actionInstance
}

// RegisterAction binds a global command to the Go kernel.
func (ar *ActionRegistry) RegisterAction(id, label, shortcut string, handler func(string)) {
	ar.mu.Lock()
	defer ar.mu.Unlock()
	ar.actions[id] = Action{ID: id, Label: label, Shortcut: shortcut, Handler: handler}
}

// Trigger executes the action for a specific user ID.
func (ar *ActionRegistry) Trigger(id, userID string) {
	ar.mu.RLock()
	action, ok := ar.actions[id]
	ar.mu.RUnlock()

	if ok {
		log.Printf("OMNI ACTION: [%s] triggered by User [%s]", action.Label, userID)
		action.Handler(userID)
	}
}
