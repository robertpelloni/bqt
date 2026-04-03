package kernel

import (
	"log"
	"sync"
)

type UndoCommand struct {
	UserID      string
	Description string
	Undo        func()
	Redo        func()
}

// UndoStack manages distributed collaborative history in Go.
type UndoStack struct {
	mu       sync.Mutex
	commands []UndoCommand
}

var (
	undoInstance *UndoStack
	undoOnce     sync.Once
)

func GetUndoStack() *UndoStack {
	undoOnce.Do(func() {
		undoInstance = &UndoStack{
			commands: make([]UndoCommand, 0),
		}
	})
	return undoInstance
}

// Push adds a new action to the Go history ledger.
func (us *UndoStack) Push(cmd UndoCommand) {
	us.mu.Lock()
	defer us.mu.Unlock()
	
	us.commands = append(us.commands, cmd)
	if len(us.commands) > 500 {
		us.commands = us.commands[1:]
	}
	log.Printf("OMNI UNDO: Recorded [%s] for User [%s]", cmd.Description, cmd.UserID)
}

// Undo reverts the last action for a specific user.
func (us *UndoStack) Undo(userID string) {
	us.mu.Lock()
	defer us.mu.Unlock()

	for i := len(us.commands) - 1; i >= 0; i-- {
		if us.commands[i].UserID == userID {
			cmd := us.commands[i]
			cmd.Undo()
			us.commands = append(us.commands[:i], us.commands[i+1:]...)
			log.Printf("OMNI UNDO: Reverted [%s] for User [%s]", cmd.Description, userID)
			return
		}
	}
}
