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

func TestDiscoveryBeaconSingletonAndIdempotentStart(t *testing.T) {
 a, b := GetDiscoveryBeacon(), GetDiscoveryBeacon()
 if a != b { t.Fatal("expected shared beacon singleton") }
 a.StartBroadcasting(8081)
 a.StartBroadcasting(8081)
 if !a.IsBroadcasting() { t.Fatal("expected broadcasting after start") }
 a.StopBroadcasting()
 if a.IsBroadcasting() { t.Fatal("expected stopped after StopBroadcasting") }
}

