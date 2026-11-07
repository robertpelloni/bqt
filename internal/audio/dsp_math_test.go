package audio

import (
	"testing"
)

func TestFilterNode_Name(t *testing.T) {
	node := &FilterNode{Cutoff: 1000.0, Resonance: 0.707}
	if node.Name() != "OmniFilter-Go" {
		t.Errorf("Expected name OmniFilter-Go, got %s", node.Name())
	}
}

func TestFilterNode_Process(t *testing.T) {
	node := &FilterNode{
		Cutoff:    1000.0,
		Resonance: 0.707,
	}

	buffer := []float32{1.0, 0.5, -0.5, -1.0, 0.0, 0.5}
	orig := make([]float32, len(buffer))
	copy(orig, buffer)

	node.Process(buffer)

	// Filtered buffer should be modified by the TPT algorithm
	modified := false
	for i := range buffer {
		if buffer[i] != orig[i] {
			modified = true
			break
		}
	}

	if !modified {
		t.Errorf("Expected audio buffer to be modified by FilterNode.Process")
	}
}
