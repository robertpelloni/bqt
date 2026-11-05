package net

import (
	"testing"
)

func TestDiscoveryBeaconInitialization(t *testing.T) {
	beacon := GetDiscoveryBeacon()
	if beacon == nil {
		t.Fatal("Expected GetDiscoveryBeacon to return non-nil instance")
	}
	if !beacon.Active {
		t.Errorf("Expected DiscoveryBeacon to be initially active")
	}
}
