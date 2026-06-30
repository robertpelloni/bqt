package audio

import (
	"testing"
)

func TestAudioGraphNativeLinking(t *testing.T) {
	graph := GetAudioGraph()

	// Clear previous state if any (for tests)
	graph.mu.Lock()
	graph.nodes = make(map[string]AudioNode)
	graph.links = make(map[string][]string)
	graph.mu.Unlock()

	synth := NewSynthesizer()
	gain := NewOmniGain()

	graph.AddNode(synth)
	graph.AddNode(gain)

	err := graph.Connect(synth.Name(), gain.Name())
	if err != nil {
		t.Fatalf("Failed to connect synth to gain: %v", err)
	}

	synth.NoteOn(1, 60, 100)

	buffer := make([]float32, 256)
	graph.ProcessBlock(buffer)

	nonZeroFound := false
	for _, sample := range buffer {
		if sample != 0.0 {
			nonZeroFound = true
			break
		}
	}

	if !nonZeroFound {
		t.Fatalf("Expected buffer to contain audio data after graph processed block")
	}

	// We only verify the synth produces audio and process block processes nodes.
	// Since ProcessBlock currently processes all nodes independently (not sequentially through links),
	// we are just testing the structure exists and doesn't crash,
	// and that nodes can be added and "connected" logically.
}
