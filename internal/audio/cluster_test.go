package audio

import (
	"testing"
)

func TestClusterDSPDistribution(t *testing.T) {
	buf := []float32{0.05, -0.1, 0.25, -0.3}

	// Verify DistributeDSPTask safely broadcasts to mesh without panic
	DistributeDSPTask("omni_synth_node_1", buf)

	// Verify HandleRemoteDSP processes block and broadcasts response without panic
	HandleRemoteDSP("peer_mesh_alpha", "omni_synth_node_1", buf)
}
