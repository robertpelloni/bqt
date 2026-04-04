package net

import (
	"log"
	"runtime"
	"time"
)

type NodeHealth struct {
	CPUUsage    float64 `json:"cpu"`
	MemoryUsage uint64  `json:"ram"`
	Goroutines  int     `json:"goroutines"`
}

// StartPulse broadcasts the node's biological health to the multiverse.
func StartPulse() {
	go func() {
		for {
			var m runtime.MemStats
			runtime.ReadMemStats(&m)
			
			health := NodeHealth{
				MemoryUsage: m.Alloc / 1024 / 1024, // MB
				Goroutines:  runtime.NumGoroutine(),
			}

			payload := map[string]interface{}{
				"type":    "node_health",
				"content": health,
			}
			GetMeshNode().Broadcast(payload)
			
			log.Printf("OMNIPULSE Go: Biological stats broadcasted to Mesh.")
			time.Sleep(10 * time.Second)
		}
	}()
}
