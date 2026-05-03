package kernel

import (
	"testing"
	"time"
)

func TestWatcherBasic(t *testing.T) {
	w := NewWatcher()
	w.AddPath("test.txt")
	
	w.Start(10 * time.Millisecond)
	defer w.Stop()

	select {
	case path := <-w.OnChanged:
		if path != "test.txt" {
			t.Fatalf("expected changed path 'test.txt', got %q", path)
		}
	case <-time.After(500 * time.Millisecond):
		t.Fatal("timed out waiting for watcher change event")
	}

	w.RemovePath("test.txt")
	// Note: in a real implementation we'd check that events stop firing.
}
