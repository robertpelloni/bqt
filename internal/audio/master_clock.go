package audio

import (
	"log"
	"sync"
	"time"
)

type MasterClock struct {
	mu         sync.Mutex
	BPM        float64
	IsRunning  bool
	TickChan   chan int
	stopSignal chan struct{}
}

var (
	clockInstance *MasterClock
	clockOnce     sync.Once
)

func GetMasterClock() *MasterClock {
	clockOnce.Do(func() {
		clockInstance = &MasterClock{
			BPM:      120.0,
			TickChan: make(chan int, 100),
		}
	})
	return clockInstance
}

// Start begins the Go-native high-priority timing loop.
func (mc *MasterClock) Start() {
	mc.mu.Lock()
	if mc.IsRunning { mc.mu.Unlock(); return }
	mc.IsRunning = true
	mc.stopSignal = make(chan struct{})
	mc.mu.Unlock()

	go func() {
		// Calculate 16th note interval
		interval := time.Duration(float64(time.Minute) / (mc.BPM * 4))
		ticker := time.NewTicker(interval)
		defer ticker.Stop()

		step := 0
		log.Printf("OmniClock Go: Temporal Heartbeat started at %f BPM.", mc.BPM)

		for {
			select {
			case <-ticker.C:
				select {
				case mc.TickChan <- step:
				default: // Channel full, skip to maintain real-time parity
				}
				step = (step + 1) % 16
			case <-mc.stopSignal:
				return
			}
		}
	}()
}

func (mc *MasterClock) Stop() {
	mc.mu.Lock()
	defer mc.mu.Unlock()
	if mc.IsRunning {
		close(mc.stopSignal)
		mc.IsRunning = false
		log.Println("OmniClock Go: Temporal Heartbeat stopped.")
	}
}
