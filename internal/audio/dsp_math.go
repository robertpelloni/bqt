package audio

import "math"

type FilterNode struct {
	Cutoff    float32
	Resonance float32
	s1, s2    float32 // State variables
}

func (fn *FilterNode) Name() string { return "OmniFilter-Go" }

// Process executes the JUCE-parity DSP math in Go.
func (fn *FilterNode) Process(buffer []float32) {
	// Simple TPT Filter Approximation (Go Port)
	g := float32(math.Tan(float64(fn.Cutoff) * math.Pi / 44100.0))
	k := 1.0 / fn.Resonance

	for i := 0; i < len(buffer); i++ {
		v0 := buffer[i]
		v3 := v0 - fn.s2
		v1 := (fn.s1 + g*v3) / (1.0 + g*(g+k))
		v2 := fn.s2 + g*v1
		
		fn.s1 = 2.0*v1 - fn.s1
		fn.s2 = 2.0*v2 - fn.s2
		
		buffer[i] = v2 // Low-pass output
	}
}
