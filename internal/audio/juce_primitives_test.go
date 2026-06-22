package audio

import "testing"

func TestJUCEGraphAdapter(t *testing.T) {
	adapter := NewJUCEAudioGraphAdapter("main_bus")
	adapter.MapJUCENode("juce_reverb")

	graph := GetAudioGraph()
	graph.mu.RLock()
	defer graph.mu.RUnlock()

	if _, exists := graph.nodes["juce_reverb"]; !exists {
		t.Fatalf("Expected 'juce_reverb' to be mapped into the Go AudioGraph")
	}
}
