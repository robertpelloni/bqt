package ui

import (
	"sync"
	"testing"
	"time"
)

func TestEventLoopPosting(t *testing.T) {
	el := GetEventLoop()

	go el.Run()

	var wg sync.WaitGroup
	var result int
	var mu sync.Mutex

	for i := 0; i < 5; i++ {
		wg.Add(1)
		val := i
		el.Post(func() {
			mu.Lock()
			result += val
			mu.Unlock()
			wg.Done()
		})
	}

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		mu.Lock()
		if result != 10 { // 0+1+2+3+4 = 10
			t.Errorf("Expected result 10, got %d", result)
		}
		mu.Unlock()
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for EventLoop tasks to process")
	}
}
