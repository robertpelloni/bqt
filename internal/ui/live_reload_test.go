package ui

import (
	"testing"
	"time"
)

func TestLiveReloader_Lifecycle(t *testing.T) {
	reloader := GetLiveReloader()
	if reloader == nil {
		t.Fatalf("expected GetLiveReloader to return non-nil instance")
	}

	if !reloader.Active {
		t.Errorf("expected reloader to be active by default")
	}

	reloader.StartWatcher()

	// Allow goroutine to start and then deactivate
	time.Sleep(20 * time.Millisecond)
	reloader.Active = false
}
