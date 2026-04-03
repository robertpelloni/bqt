package main

import (
	"log"
	"time"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/net"
	"github.com/robertpelloni/bobui/internal/state"
)

func main() {
	log.Println("OmniUI Go: Initializing Kernel Singularity...")

	// 1. Setup Kernel Managers
	im := kernel.GetInputManager()
	um := kernel.GetUserManager()
	vt := state.GetValueTree()
	mesh := net.GetMeshNode()

	// 2. Create Default System Identity
	um.CreateUser("sys-admin", "Administrator", "#00F0FF")
	im.RegisterDevice("sys-mouse-0", "System Mouse", kernel.Mouse)

	// 3. Start Distributed Mesh Node
	mesh.StartNode("8081")

	// 4. Reactive State Subscription (Go Parity with JavaFX)
	changes := vt.Subscribe()
	go func() {
		for change := range changes {
			log.Printf("OMNI STATE: Param [%s] shifted to %v", change.Key, change.Value)
			// Broadcast state change to P2P peers
			mesh.Broadcast(change)
		}
	}()

	// 5. Simulation Loop (OS Heartbeat)
	log.Println("OmniUI Go: OS is now operational. Awaiting Multi-User Input...")
	for {
		// Mock physical hardware sync
		im.UpdateCursor("sys-mouse-0", 100, 200)
		time.Sleep(1 * time.Second)
	}
}
