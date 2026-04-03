package data

import (
	"log"
	"sync"
)

type TimeMachine struct {
	mu sync.Mutex
}

var (
	tmInstance *TimeMachine
	tmOnce     sync.Once
)

func GetTimeMachine() *TimeMachine {
	tmOnce.Do(func() {
		tmInstance = &TimeMachine{}
	})
	return tmInstance
}

// CommitState saves a file snapshot to the Go-native SQLite ledger.
func (tm *TimeMachine) CommitState(path string, content string) error {
	db := GetDatabase()
	_, err := db.conn.Exec("INSERT INTO _omni_timemachine (filepath, content) VALUES (?, ?)", path, content)
	if err != nil {
		return err
	}
	log.Printf("OmniTimeMachine Go: State committed for %s", path)
	return nil
}

// GetStateCount returns the number of historical snapshots in Go.
func (tm *TimeMachine) GetStateCount(path string) (int, error) {
	db := GetDatabase()
	var count int
	err := db.conn.QueryRow("SELECT COUNT(*) FROM _omni_timemachine WHERE filepath = ?", path).Scan(&count)
	return count, err
}
