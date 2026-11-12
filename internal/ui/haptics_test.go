package ui

import (
	"testing"

	"github.com/robertpelloni/bqt/internal/kernel"
)

func TestHapticsEngine_ProcessProximity(t *testing.T) {
	engine := &HapticsEngine{Enabled: false}

	// Should safely return early when disabled
	engine.ProcessProximity()

	engine.Enabled = true
	im := kernel.GetInputManager()

	// Register devices close to each other (< 100px)
	im.RegisterDevice("local_user", "Local User", kernel.Mouse)
	im.UpdateCursor("local_user", 10.0, 10.0)

	im.RegisterDevice("peer_user_close", "Peer Close", kernel.Mouse)
	im.UpdateCursor("peer_user_close", 15.0, 20.0)

	// Should calculate proximity and log drone trigger without panic
	engine.ProcessProximity()

	// Register device far away (> 100px)
	im.RegisterDevice("peer_user_far", "Peer Far", kernel.Mouse)
	im.UpdateCursor("peer_user_far", 500.0, 500.0)
	engine.ProcessProximity()
}
