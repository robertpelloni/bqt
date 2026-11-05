package state

import (
	"testing"
	"time"
)

func TestValueTreeSingleton(t *testing.T) {
	a := GetValueTree()
	b := GetValueTree()
	if a == nil || b == nil {
		t.Fatal("Expected GetValueTree to return non-nil instance")
	}
	if a != b {
		t.Fatal("Expected GetValueTree to return the same singleton instance")
	}
}

func TestValueTreeSetParameterAndSubscribe(t *testing.T) {
	vt := GetValueTree()
	ch := vt.Subscribe()

	vt.SetParameter("master_volume", 0.75)

	select {
	case change := <-ch:
		if change.Key != "master_volume" {
			t.Errorf("Expected key 'master_volume', got %s", change.Key)
		}
		if val, ok := change.Value.(float64); !ok || val != 0.75 {
			t.Errorf("Expected value 0.75, got %v", change.Value)
		}
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for ParameterChange on subscribed channel")
	}
}
