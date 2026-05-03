package kernel

import (
	"log"
	"sync"
)

type UndoAction struct {
	UserID      string
	Description string
	Undo        func()
	Redo        func()
}

// UndoStack manages the Go-native multi-user history ledger.
type UndoStack struct {
	mu       sync.Mutex
	actions  []UndoAction
	redoPool []UndoAction
}

var (
	undoInstance *UndoStack
	undoOnce     sync.Once
)

func GetUndoStack() *UndoStack {
	undoOnce.Do(func() {
		undoInstance = &UndoStack{
			actions:  make([]UndoAction, 0),
			redoPool: make([]UndoAction, 0),
		}
	})
	return undoInstance
}

// Push appends a new multi-user action to the Go temporal stack.
func (us *UndoStack) Push(action UndoAction) {
	us.mu.Lock()
	defer us.mu.Unlock()
	
	us.actions = append(us.actions, action)
	// Limit history to 500 actions to prevent Go heap bloat
	if len(us.actions) > 500 { us.actions = us.actions[1:] }
	
	log.Printf("OMNI UNDO: Recorded [%s] for user [%s]", action.Description, action.UserID)
}

// Undo specifically reverts the last action for a given UserID.
func (us *UndoStack) Undo(userID string) {
	us.mu.Lock()
	defer us.mu.Unlock()

	for i := len(us.actions) - 1; i >= 0; i-- {
		if us.actions[i].UserID == userID {
			action := us.actions[i]
			action.Undo()
			us.redoPool = append(us.redoPool, action)
			us.actions = append(us.actions[:i], us.actions[i+1:]...)
			log.Printf("OMNI UNDO: Reverted action for user [%s]", userID)
			return
		}
	}
}
